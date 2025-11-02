#include <stdio.h>
#include <stdlib.h>

char board[3][3]; // 游戏棋盘

// 初始化棋盘
void initBoard() {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

// 显示棋盘
void displayBoard() {
    printf("\n");
    printf(" %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
    printf("\n");
}

// 检查是否有玩家获胜
char checkWin() {
    // 检查行
    for(int i = 0; i < 3; i++) {
        if(board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
            return board[i][0];
        }
    }
    
    // 检查列
    for(int i = 0; i < 3; i++) {
        if(board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
            return board[0][i];
        }
    }
    
    // 检查对角线
    if(board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
        return board[0][0];
    }
    if(board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
        return board[0][2];
    }
    
    // 检查是否平局
    int isFull = 1;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(board[i][j] == ' ') {
                isFull = 0;
            }
        }
    }
    if(isFull) return 'D'; // 平局
    
    return ' '; // 游戏继续
}

int main() {
    int row, col;
    char player = 'X';
    char winner = ' ';
    
    initBoard();
    
    printf("井字棋游戏开始！\n");
    printf("玩家X先手，玩家O后手\n");
    printf("输入行和列(0-2)，用空格分隔\n");
    
    while(winner == ' ') {
        displayBoard();
        printf("玩家%c的回合: ", player);
        scanf("%d %d", &row, &col);
        
        if(row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ') {
            printf("无效移动，请重试\n");
            continue;
        }
        
        board[row][col] = player;
        winner = checkWin();
        
        // 切换玩家
        player = (player == 'X') ? 'O' : 'X';
    }
    
    displayBoard();
    
    if(winner == 'D') {
        printf("平局！\n");
    } else {
        printf("玩家%c获胜！\n", winner);
    }
    
    return 0;
}
