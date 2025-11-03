#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>

#define WIDTH 60
#define HEIGHT 20
#define INIT_LENGTH 3

// 方向常量
#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3

// 游戏状态
bool gameOver;
int score;

// 蛇和食物
int snakeX[100], snakeY[100];
int snakeLength;
int direction;
int foodX, foodY;

// 初始化游戏
void setup() {
    gameOver = false;
    direction = DIR_RIGHT;
    score = 0;
    snakeLength = INIT_LENGTH;
    
    // 初始化蛇的位置（居中）
    for (int i = 0; i < snakeLength; i++) {
        snakeX[i] = WIDTH / 2 - i;
        snakeY[i] = HEIGHT / 2;
    }
    
    // 生成第一个食物
    srand(time(0));
    foodX = rand() % (WIDTH - 2) + 1;
    foodY = rand() % (HEIGHT - 2) + 1;
}

// 绘制游戏界面
void draw() {
    system("cls"); // 清屏
    
    // 绘制上边界
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            // 绘制左边界
            if (j == 0)
                printf("#");
            
            // 绘制蛇头
            if (i == snakeY[0] && j == snakeX[0])
                printf("O");
            // 绘制蛇身
            else if (i == snakeY[1] && j == snakeX[1])
                printf("o");
            // 绘制食物
            else if (i == foodY && j == foodX)
                printf("*");
            else {
                // 绘制蛇身其他部分或空白
                bool printSpace = true;
                for (int k = 2; k < snakeLength; k++) {
                    if (i == snakeY[k] && j == snakeX[k]) {
                        printf("o");
                        printSpace = false;
                        break;
                    }
                }
                
                if (printSpace)
                    printf(" ");
            }
            
            // 绘制右边界
            if (j == WIDTH - 1)
                printf("#");
        }
        printf("\n");
    }
    
    // 绘制下边界
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");
    
    // 显示分数
    printf("Score: %d\n", score);
    printf("Press 'x' to quit\n");
}

// 处理输入
void input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'w':
                if (direction != DIR_DOWN)
                    direction = DIR_UP;
                break;
            case 'd':
                if (direction != DIR_LEFT)
                    direction = DIR_RIGHT;
                break;
            case 's':
                if (direction != DIR_UP)
                    direction = DIR_DOWN;
                break;
            case 'a':
                if (direction != DIR_RIGHT)
                    direction = DIR_LEFT;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

// 更新游戏状态
void logic() {
    // 移动蛇身（从尾部开始更新）
    for (int i = snakeLength - 1; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    
    // 移动蛇头
    switch (direction) {
        case DIR_UP:
            snakeY[0]--;
            break;
        case DIR_RIGHT:
            snakeX[0]++;
            break;
        case DIR_DOWN:
            snakeY[0]++;
            break;
        case DIR_LEFT:
            snakeX[0]--;
            break;
    }
    
    // 检查是否吃到食物
    if (snakeX[0] == foodX && snakeY[0] == foodY) {
        score += 10;
        
        // 生成新食物
        foodX = rand() % (WIDTH - 2) + 1;
        foodY = rand() % (HEIGHT - 2) + 1;
        
        // 增加蛇的长度
        snakeLength++;
    }
    
    // 检查是否撞墙
    if (snakeX[0] <= 0 || snakeX[0] >= WIDTH - 1 || 
        snakeY[0] <= 0 || snakeY[0] >= HEIGHT - 1) {
        gameOver = true;
    }
    
    // 检查是否撞到自己
    for (int i = 1; i < snakeLength; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
            gameOver = true;
            break;
        }
    }
    
    // 添加延迟控制游戏速度
    Sleep(100);
}

int main() {
    setup();
    
    while (!gameOver) {
        draw();
        input();
        logic();
    }
    
    printf("Game Over! Final Score: %d\n", score);
    
    return 0;
}
