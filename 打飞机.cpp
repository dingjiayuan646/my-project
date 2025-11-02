#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define WIDTH 60
#define HEIGHT 20
#define ENEMY_COUNT 5

typedef struct {
    int x, y;
    int isActive;
} Enemy;

typedef struct {
    int x, y;
    int bullets[10];
    int bulletCount;
} Player;

// 全局变量
Player player;
Enemy enemies[ENEMY_COUNT];
int score = 0;
int gameOver = 0;

// 初始化游戏
void setup() {
    player.x = WIDTH / 2;
    player.y = HEIGHT - 2;
    player.bulletCount = 0;
    
    // 初始化敌人
    for (int i = 0; i < ENEMY_COUNT; i++) {
        enemies[i].x = rand() % WIDTH;
        enemies[i].y = rand() % (HEIGHT / 2);
        enemies[i].isActive = 1;
    }
}

// 绘制游戏界面
void draw() {
    system("cls"); // 清屏
    
    // 绘制顶部边界
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");
    
    // 绘制游戏区域
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0)
                printf("#"); // 左边界
                
            // 绘制玩家飞机
            if (i == player.y && j == player.x)
                printf("A");
            // 绘制敌人
            else {
                int enemyDrawn = 0;
                for (int k = 0; k < ENEMY_COUNT; k++) {
                    if (enemies[k].isActive && i == enemies[k].y && j == enemies[k].x) {
                        printf("E");
                        enemyDrawn = 1;
                        break;
                    }
                }
                // 绘制子弹
                if (!enemyDrawn) {
                    int bulletDrawn = 0;
                    for (int k = 0; k < player.bulletCount; k++) {
                        if (i == player.bullets[k] && j == player.x) {
                            printf("|");
                            bulletDrawn = 1;
                            break;
                        }
                    }
                    if (!bulletDrawn)
                        printf(" ");
                }
            }
            
            if (j == WIDTH - 1)
                printf("#"); // 右边界
        }
        printf("\n");
    }
    
    // 绘制底部边界和分数
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");
    printf("得分: %d\n", score);
    printf("按 'a' 和 'd' 移动，按 '空格' 射击，按 'x' 退出\n");
}

// 处理输入
void input() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'a':
                if (player.x > 0) player.x--;
                break;
            case 'd':
                if (player.x < WIDTH - 1) player.x++;
                break;
            case ' ': // 空格键射击
                if (player.bulletCount < 10) {
                    player.bullets[player.bulletCount] = player.y - 1;
                    player.bulletCount++;
                }
                break;
            case 'x':
                gameOver = 1;
                break;
        }
    }
}

// 更新游戏状态
void update() {
    // 移动子弹
    for (int i = 0; i < player.bulletCount; i++) {
        player.bullets[i]--;
        
        // 检查子弹是否击中敌人
        for (int j = 0; j < ENEMY_COUNT; j++) {
            if (enemies[j].isActive && 
                player.bullets[i] == enemies[j].y && 
                player.x == enemies[j].x) {
                score += 10;
                enemies[j].isActive = 0;
                // 重新生成敌人
                enemies[j].x = rand() % WIDTH;
                enemies[j].y = rand() % (HEIGHT / 2);
                enemies[j].isActive = 1;
                
                // 移除子弹
                for (int k = i; k < player.bulletCount - 1; k++) {
                    player.bullets[k] = player.bullets[k + 1];
                }
                player.bulletCount--;
                break;
            }
        }
        
        // 移除超出屏幕的子弹
        if (player.bullets[i] < 0) {
            for (int j = i; j < player.bulletCount - 1; j++) {
                player.bullets[j] = player.bullets[j + 1];
            }
            player.bulletCount--;
        }
    }
    
    // 移动敌人
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (enemies[i].isActive) {
            enemies[i].y++;
            
            // 检查敌人是否到达底部
            if (enemies[i].y >= HEIGHT) {
                enemies[i].isActive = 0;
                // 重新生成敌人
                enemies[i].x = rand() % WIDTH;
                enemies[i].y = rand() % (HEIGHT / 2);
                enemies[i].isActive = 1;
                
                // 减少分数
                if (score > 0) score -= 5;
            }
            
            // 检查敌人是否撞到玩家
            if (enemies[i].y == player.y && enemies[i].x == player.x) {
                gameOver = 1;
            }
        }
    }
    
    // 稍微延迟一下，控制游戏速度 
    Sleep(100);
}

// 主函数
int main() {
    srand(time(0)); // 初始化随机数生成器
    
    setup();
    
    while (!gameOver) {
        draw();
        input();
        update();
    }
    
    system("cls");
    printf("游戏结束！最终得分: %d\n", score);
    printf("按任意键退出...\n");
    _getch();
    
    return 0;
}
