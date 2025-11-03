#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define BOARD_SIZE 9
#define MINE_COUNT 10

// 游戏板格子状态
typedef struct {
    bool is_mine;       // 是否是地雷
    bool is_revealed;   // 是否已揭开
    bool is_flagged;    // 是否被标记
    int adjacent_mines; // 相邻地雷数量
} Cell;

Cell board[BOARD_SIZE][BOARD_SIZE];
int remaining_cells; // 剩余未揭开的非地雷格子数量

// 初始化游戏板
void initialize_board() {
    // 初始化所有格子
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j].is_mine = false;
            board[i][j].is_revealed = false;
            board[i][j].is_flagged = false;
            board[i][j].adjacent_mines = 0;
        }
    }
    
    // 随机放置地雷
    int mines_placed = 0;
    while (mines_placed < MINE_COUNT) {
        int x = rand() % BOARD_SIZE;
        int y = rand() % BOARD_SIZE;
        
        if (!board[x][y].is_mine) {
            board[x][y].is_mine = true;
            mines_placed++;
        }
    }
    
    // 计算每个格子相邻的地雷数量
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (!board[i][j].is_mine) {
                int count = 0;
                
                // 检查所有相邻格子
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int nx = i + dx;
                        int ny = j + dy;
                        
                        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                            if (board[nx][ny].is_mine) {
                                count++;
                            }
                        }
                    }
                }
                
                board[i][j].adjacent_mines = count;
            }
        }
    }
    
    remaining_cells = BOARD_SIZE * BOARD_SIZE - MINE_COUNT;
}

// 显示游戏板
void display_board(bool show_mines) {
    printf("\n  ");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].is_flagged) {
                printf("F ");
            } else if (!board[i][j].is_revealed) {
                printf(". ");
            } else if (board[i][j].is_mine) {
                printf("* ");
            } else if (board[i][j].adjacent_mines > 0) {
                printf("%d ", board[i][j].adjacent_mines);
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
}

// 揭开格子
void reveal_cell(int x, int y) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE || 
        board[x][y].is_revealed || board[x][y].is_flagged) {
        return;
    }
    
    board[x][y].is_revealed = true;
    
    if (board[x][y].is_mine) {
        return;
    }
    
    remaining_cells--;
    
    // 如果相邻地雷数为0，自动揭开相邻格子
    if (board[x][y].adjacent_mines == 0) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                reveal_cell(x + dx, y + dy);
            }
        }
    }
}

// 标记格子
void flag_cell(int x, int y) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE || 
        board[x][y].is_revealed) {
        return;
    }
    
    board[x][y].is_flagged = !board[x][y].is_flagged;
}

int main() {
    srand(time(NULL));
    
    printf("欢迎来到扫雷游戏！\n");
    printf("指令说明：\n");
    printf("  R x y - 揭开格子 (x,y)\n");
    printf("  F x y - 标记/取消标记格子 (x,y)\n");
    printf("  Q - 退出游戏\n");
    
    initialize_board();
    
    bool game_over = false;
    bool won = false;
    
    while (!game_over) {
        display_board(false);
        
        printf("请输入指令: ");
        char command;
        scanf(" %c", &command);
        
        if (command == 'Q' || command == 'q') {
            break;
        }
        
        int x, y;
        scanf("%d %d", &x, &y);
        
        if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
            printf("无效的坐标！请重新输入。\n");
            continue;
        }
        
        if (command == 'R' || command == 'r') {
            if (board[x][y].is_mine) {
                game_over = true;
                printf("游戏结束！你踩到了地雷。\n");
                display_board(true);
            } else {
                reveal_cell(x, y);
                
                if (remaining_cells == 0) {
                    game_over = true;
                    won = true;
                    printf("恭喜你赢了！\n");
                    display_board(true);
                }
            }
        } else if (command == 'F' || command == 'f') {
            flag_cell(x, y);
        } else {
            printf("无效的指令！请重新输入。\n");
        }
    }
    
    return 0;
}
