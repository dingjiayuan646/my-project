#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 5
#define PLANT_COST 50
#define ZOMBIE_SPAWN_RATE 20

// 游戏元素类型
typedef enum {
    EMPTY,
    PLANT,
    ZOMBIE,
    BULLET,
    SUN
} EntityType;

// 游戏元素结构
typedef struct {
    EntityType type;
    int health;
    int value;
} Entity;

// 游戏状态
typedef struct {
    Entity grid[HEIGHT][WIDTH];
    int sun;
    int score;
    int game_over;
} GameState;

// 初始化游戏
void init_game(GameState *game) {
    game->sun = 50;
    game->score = 0;
    game->game_over = 0;
    
    // 初始化网格
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            game->grid[y][x].type = EMPTY;
            game->grid[y][x].health = 0;
            game->grid[y][x].value = 0;
        }
    }
}

// 绘制游戏界面
void draw_game(GameState *game) {
    system("cls"); // 清屏
    
    printf("植物大战僵尸 - 控制台版\n");
    printf("分数: %d | 阳光: %d\n", game->score, game->sun);
    printf("操作: P-种植植物, S-生成阳光, Q-退出\n\n");
    
    // 绘制网格
    for (int y = 0; y < HEIGHT; y++) {
        printf("%d ", y + 1);
        for (int x = 0; x < WIDTH; x++) {
            switch (game->grid[y][x].type) {
                case EMPTY:
                    printf(". ");
                    break;
                case PLANT:
                    printf("P ");
                    break;
                case ZOMBIE:
                    printf("Z ");
                    break;
                case BULLET:
                    printf("* ");
                    break;
                case SUN:
                    printf("S ");
                    break;
            }
        }
        printf("\n");
    }
    
    printf("\n");
    for (int i = 0; i < WIDTH; i++) {
        printf("%d ", (i + 1) % 10);
    }
    printf("\n");
    
    if (game->game_over) {
        printf("\n游戏结束! 僵尸吃掉了你的脑子!\n");
        printf("最终得分: %d\n", game->score);
    }
}

// 种植植物
void plant_plant(GameState *game, int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    if (game->grid[y][x].type != EMPTY) return;
    if (game->sun < PLANT_COST) return;
    
    game->grid[y][x].type = PLANT;
    game->grid[y][x].health = 100;
    game->sun -= PLANT_COST;
}

// 生成阳光
void spawn_sun(GameState *game) {
    int x = rand() % WIDTH;
    int y = rand() % HEIGHT;
    
    if (game->grid[y][x].type == EMPTY) {
        game->grid[y][x].type = SUN;
        game->grid[y][x].value = 25;
    }
}

// 收集阳光
void collect_sun(GameState *game, int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    if (game->grid[y][x].type != SUN) return;
    
    game->sun += game->grid[y][x].value;
    game->grid[y][x].type = EMPTY;
}

// 生成僵尸
void spawn_zombie(GameState *game) {
    if (rand() % 100 < ZOMBIE_SPAWN_RATE) {
        int y = rand() % HEIGHT;
        
        if (game->grid[y][WIDTH-1].type == EMPTY) {
            game->grid[y][WIDTH-1].type = ZOMBIE;
            game->grid[y][WIDTH-1].health = 100;
        }
    }
}

// 移动僵尸
void move_zombies(GameState *game) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (game->grid[y][x].type == ZOMBIE) {
                // 检查前方是否有植物
                if (x > 0 && game->grid[y][x-1].type == PLANT) {
                    // 攻击植物
                    game->grid[y][x-1].health -= 10;
                    if (game->grid[y][x-1].health <= 0) {
                        game->grid[y][x-1].type = EMPTY;
                    }
                } else if (x > 0 && game->grid[y][x-1].type == EMPTY) {
                    // 移动僵尸
                    game->grid[y][x-1] = game->grid[y][x];
                    game->grid[y][x].type = EMPTY;
                    x++; // 跳过下一个位置，避免重复处理
                } else if (x == 0) {
                    // 僵尸到达最左边，游戏结束
                    game->game_over = 1;
                }
            }
        }
    }
}

// 植物射击
void plants_shoot(GameState *game) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (game->grid[y][x].type == PLANT) {
                // 检查右侧是否有僵尸
                for (int bx = x + 1; bx < WIDTH; bx++) {
                    if (game->grid[y][bx].type == ZOMBIE) {
                        // 发射子弹
                        if (game->grid[y][x+1].type == EMPTY) {
                            game->grid[y][x+1].type = BULLET;
                        }
                        break;
                    }
                }
            }
        }
    }
}

// 移动子弹
void move_bullets(GameState *game) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = WIDTH - 1; x >= 0; x--) {
            if (game->grid[y][x].type == BULLET) {
                if (x < WIDTH - 1) {
                    // 检查下一个位置
                    if (game->grid[y][x+1].type == ZOMBIE) {
                        // 击中僵尸
                        game->grid[y][x+1].health -= 25;
                        if (game->grid[y][x+1].health <= 0) {
                            game->grid[y][x+1].type = EMPTY;
                            game->score += 10;
                        }
                        game->grid[y][x].type = EMPTY;
                    } else if (game->grid[y][x+1].type == EMPTY) {
                        // 移动子弹
                        game->grid[y][x+1] = game->grid[y][x];
                        game->grid[y][x].type = EMPTY;
                    } else {
                        // 子弹被阻挡
                        game->grid[y][x].type = EMPTY;
                    }
                } else {
                    // 子弹到达最右边
                    game->grid[y][x].type = EMPTY;
                }
            }
        }
    }
}

// 更新游戏状态
void update_game(GameState *game) {
    if (game->game_over) return;
    
    // 移动僵尸
    move_zombies(game);
    
    // 植物射击
    plants_shoot(game);
    
    // 移动子弹
    move_bullets(game);
    
    // 生成僵尸
    spawn_zombie(game);
    
    // 随机生成阳光
    if (rand() % 100 < 5) {
        spawn_sun(game);
    }
}

// 处理用户输入
void process_input(GameState *game) {
    if (_kbhit()) {
        char input = _getch();
        
        switch (input) {
            case 'p':
            case 'P': {
                printf("选择行和列来种植植物 (例如: 2 3): ");
                int y, x;
                scanf("%d %d", &y, &x);
                plant_plant(game, x - 1, y - 1);
                break;
            }
            case 's':
            case 'S':
                spawn_sun(game);
                break;
            case 'q':
            case 'Q':
                game->game_over = 1;
                break;
            default:
                // 其他输入视为收集阳光的坐标
                if (input >= '1' && input <= '5') {
                    int y = input - '1';
                    printf("选择列来收集阳光 (1-%d): ", WIDTH);
                    int x;
                    scanf("%d", &x);
                    collect_sun(game, x - 1, y);
                }
                break;
        }
    }
}

int main() {
    srand(time(NULL)); // 初始化随机数种子
    
    GameState game;
    init_game(&game);
    
    printf("植物大战僵尸 - 控制台版\n");
    printf("操作说明:\n");
    printf("P - 种植植物 (花费50阳光)\n");
    printf("S - 生成阳光\n");
    printf("1-5 - 选择行，然后输入列号收集阳光\n");
    printf("Q - 退出游戏\n");
    printf("按任意键开始游戏...\n");
    _getch();
    
    while (!game.game_over) {
        draw_game(&game);
        process_input(&game);
        update_game(&game);
        Sleep(500); // 延迟半秒
    }
    
    draw_game(&game);
    printf("按任意键退出...\n");
    _getch();
    
    return 0;
}
