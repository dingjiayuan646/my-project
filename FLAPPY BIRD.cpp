#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

// 游戏常量
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GRAVITY 0.5
#define JUMP_STRENGTH -10
#define PIPE_WIDTH 80
#define PIPE_GAP 200
#define PIPE_SPEED 3
#define PIPE_SPAWN_TIME 1500
#define BIRD_SIZE 30

// 颜色定义
#define SKY_COLOR RGB(135, 206, 235)
#define BIRD_COLOR RGB(255, 255, 0)
#define PIPE_COLOR RGB(0, 128, 0)
#define GROUND_COLOR RGB(139, 69, 19)
#define TEXT_COLOR RGB(0, 0, 0)

// 游戏结构体
typedef struct {
    float x, y;
    float velocity;
} Bird;

typedef struct {
    float x;
    int height;
    bool passed;
} Pipe;

typedef struct {
    Bird bird;
    Pipe* pipes;
    int pipeCount;
    int score;
    bool gameOver;
    DWORD lastPipeSpawn;
} GameState;

// 全局变量
GameState game;
HWND hwnd;
HDC hdc;
HPEN skyPen, birdPen, pipePen, groundPen, textPen;
HBRUSH skyBrush, birdBrush, pipeBrush, groundBrush;

// 初始化游戏
void initGame() {
    game.bird.x = SCREEN_WIDTH / 4;
    game.bird.y = SCREEN_HEIGHT / 2;
    game.bird.velocity = 0;
    
    game.pipes = NULL;
    game.pipeCount = 0;
    game.score = 0;
    game.gameOver = false;
    game.lastPipeSpawn = 0;
}

// 添加管道
void addPipe() {
    game.pipeCount++;
    game.pipes = realloc(game.pipes, game.pipeCount * sizeof(Pipe));
    
    int minHeight = 50;
    int maxHeight = SCREEN_HEIGHT - PIPE_GAP - minHeight;
    int height = minHeight + rand() % (maxHeight - minHeight);
    
    game.pipes[game.pipeCount - 1].x = SCREEN_WIDTH;
    game.pipes[game.pipeCount - 1].height = height;
    game.pipes[game.pipeCount - 1].passed = false;
}

// 更新游戏状态
void updateGame() {
    if (game.gameOver) return;
    
    // 更新小鸟
    game.bird.velocity += GRAVITY;
    game.bird.y += game.bird.velocity;
    
    // 检查地面碰撞
    if (game.bird.y + BIRD_SIZE > SCREEN_HEIGHT - 50) {
        game.bird.y = SCREEN_HEIGHT - 50 - BIRD_SIZE;
        game.gameOver = true;
    }
    
    // 检查天花板碰撞
    if (game.bird.y < 0) {
        game.bird.y = 0;
        game.bird.velocity = 0;
    }
    
    // 生成新管道
    DWORD currentTime = GetTickCount();
    if (currentTime - game.lastPipeSpawn > PIPE_SPAWN_TIME) {
        addPipe();
        game.lastPipeSpawn = currentTime;
    }
    
    // 更新管道
    for (int i = 0; i < game.pipeCount; i++) {
        game.pipes[i].x -= PIPE_SPEED;
        
        // 检查碰撞
        if (game.bird.x + BIRD_SIZE > game.pipes[i].x &&
            game.bird.x < game.pipes[i].x + PIPE_WIDTH) {
            if (game.bird.y < game.pipes[i].height ||
                game.bird.y + BIRD_SIZE > game.pipes[i].height + PIPE_GAP) {
                game.gameOver = true;
            }
        }
        
        // 检查是否通过管道
        if (!game.pipes[i].passed && game.bird.x > game.pipes[i].x + PIPE_WIDTH) {
            game.pipes[i].passed = true;
            game.score++;
        }
        
        // 移除屏幕外的管道
        if (game.pipes[i].x + PIPE_WIDTH < 0) {
            for (int j = i; j < game.pipeCount - 1; j++) {
                game.pipes[j] = game.pipes[j + 1];
            }
            game.pipeCount--;
            game.pipes = realloc(game.pipes, game.pipeCount * sizeof(Pipe));
            i--;
        }
    }
}

// 绘制游戏
void drawGame() {
    // 绘制天空
    SelectObject(hdc, skyBrush);
    SelectObject(hdc, skyPen);
    Rectangle(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // 绘制地面
    SelectObject(hdc, groundBrush);
    SelectObject(hdc, groundPen);
    Rectangle(hdc, 0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // 绘制管道
    SelectObject(hdc, pipeBrush);
    SelectObject(hdc, pipePen);
    for (int i = 0; i < game.pipeCount; i++) {
        // 上管道
        Rectangle(hdc, game.pipes[i].x, 0, game.pipes[i].x + PIPE_WIDTH, game.pipes[i].height);
        
        // 下管道
        Rectangle(hdc, game.pipes[i].x, game.pipes[i].height + PIPE_GAP, 
                 game.pipes[i].x + PIPE_WIDTH, SCREEN_HEIGHT);
    }
    
    // 绘制小鸟
    SelectObject(hdc, birdBrush);
    SelectObject(hdc, birdPen);
    Ellipse(hdc, game.bird.x, game.bird.y, game.bird.x + BIRD_SIZE, game.bird.y + BIRD_SIZE);
    
    // 绘制分数
    char scoreText[20];
    sprintf(scoreText, "Score: %d", game.score);
    
    SelectObject(hdc, textPen);
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, 10, 10, scoreText, strlen(scoreText));
    
    // 游戏结束提示
    if (game.gameOver) {
        TextOut(hdc, SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 10, 
               "Game Over! Press R to restart", 30);
    }
}

// 创建画笔和画刷
void createBrushesAndPens() {
    skyPen = CreatePen(PS_SOLID, 1, SKY_COLOR);
    skyBrush = CreateSolidBrush(SKY_COLOR);
    
    birdPen = CreatePen(PS_SOLID, 1, BIRD_COLOR);
    birdBrush = CreateSolidBrush(BIRD_COLOR);
    
    pipePen = CreatePen(PS_SOLID, 1, PIPE_COLOR);
    pipeBrush = CreateSolidBrush(PIPE_COLOR);
    
    groundPen = CreatePen(PS_SOLID, 1, GROUND_COLOR);
    groundBrush = CreateSolidBrush(GROUND_COLOR);
    
    textPen = CreatePen(PS_SOLID, 1, TEXT_COLOR);
}

// 释放画笔和画刷
void releaseBrushesAndPens() {
    DeleteObject(skyPen);
    DeleteObject(skyBrush);
    DeleteObject(birdPen);
    DeleteObject(birdBrush);
    DeleteObject(pipePen);
    DeleteObject(pipeBrush);
    DeleteObject(groundPen);
    DeleteObject(groundBrush);
    DeleteObject(textPen);
}

// 窗口过程函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            drawGame();
            EndPaint(hwnd, &ps);
            return 0;
            
        case WM_KEYDOWN:
            switch (wParam) {
                case VK_SPACE:
                    if (!game.gameOver) {
                        game.bird.velocity = JUMP_STRENGTH;
                    }
                    break;
                    
                case 'R':
                case 'r':
                    if (game.gameOver) {
                        free(game.pipes);
                        initGame();
                    }
                    break;
                    
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
            
        case WM_TIMER:
            updateGame();
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// 主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    srand(time(NULL));
    
    // 注册窗口类
    const char CLASS_NAME[] = "FlappyBirdWindowClass";
    
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    RegisterClass(&wc);
    
    // 创建窗口
    hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Flappy Bird",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    
    if (hwnd == NULL) {
        return 0;
    }
    
    // 固定窗口大小
    SetWindowPos(hwnd, NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 
                SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
    
    // 创建画笔和画刷
    createBrushesAndPens();
    
    // 初始化游戏
    initGame();
    
    // 显示窗口
    ShowWindow(hwnd, nCmdShow);
    
    // 设置定时器
    SetTimer(hwnd, 1, 16, NULL); // 约60FPS
    
    // 消息循环
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    // 清理资源
    releaseBrushesAndPens();
    free(game.pipes);
    KillTimer(hwnd, 1);
    
    return msg.wParam;
}
