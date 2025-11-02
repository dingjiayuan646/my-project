#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ATTEMPTS 3
#define PASSWORD_LENGTH 20

// 函数声明
void setPassword(char *password);
int verifyPassword(const char *password);
void clearInputBuffer();

int main() {
    char password[PASSWORD_LENGTH] = {0};
    int attempts = 0;
    int verified = 0;
    
    printf("========== 密码设置与验证系统 ==========\n");
    
    // 设置密码
    setPassword(password);
    printf("密码设置成功！\n\n");
    
    // 验证密码
    printf("请验证您的密码：\n");
    
    while (attempts < MAX_ATTEMPTS && !verified) {
        verified = verifyPassword(password);
        
        if (!verified) {
            attempts++;
            printf("密码错误！您还有 %d 次尝试机会。\n", MAX_ATTEMPTS - attempts);
            
            if (attempts >= MAX_ATTEMPTS) {
                printf("\n==========================================\n");
                printf("错误次数过多，账户已被锁定！\n");
                printf("请联系系统管理员解锁。\n");
                printf("==========================================\n");
                break;
            }
        } else {
            printf("\n==========================================\n");
            printf("密码验证成功！欢迎访问系统。\n");
            printf("==========================================\n");
        }
    }
    
    return 0;
}

// 设置密码函数
void setPassword(char *password) {
    char tempPassword[PASSWORD_LENGTH];
    char confirmPassword[PASSWORD_LENGTH];
    int match = 0;
    
    while (!match) {
        printf("请设置您的密码（最多%d个字符）: ", PASSWORD_LENGTH - 1);
        fgets(tempPassword, PASSWORD_LENGTH, stdin);
        tempPassword[strcspn(tempPassword, "\n")] = '\0'; // 移除换行符
        
        printf("请再次确认密码: ");
        fgets(confirmPassword, PASSWORD_LENGTH, stdin);
        confirmPassword[strcspn(confirmPassword, "\n")] = '\0'; // 移除换行符
        
        if (strcmp(tempPassword, confirmPassword) == 0) {
            strcpy(password, tempPassword);
            match = 1;
        } else {
            printf("两次输入的密码不匹配，请重新设置！\n");
        }
    }
}

// 验证密码函数
int verifyPassword(const char *password) {
    char input[PASSWORD_LENGTH];
    
    printf("请输入密码: ");
    fgets(input, PASSWORD_LENGTH, stdin);
    input[strcspn(input, "\n")] = '\0'; // 移除换行符
    
    return (strcmp(input, password) == 0);
}

// 清空输入缓冲区
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
