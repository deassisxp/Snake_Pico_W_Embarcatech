#include "snake.h"
#include <stdlib.h>
#include <stdbool.h>

Point snake[100];
int snake_len = 3;
Point food;

static bool is_on_snake(int x, int y) {
    int i;
    for (i = 0; i < snake_len; i++) {
        if (snake[i].x == x && snake[i].y == y) {
            return true;
        }
    }
    return false;
}

static void spawn_food(void) {
    do {
        food.x = rand() % 32;
        food.y = rand() % 16;
    } while (is_on_snake(food.x, food.y));
}

void init_game() {
    int i;
    snake_len = 3;

    for (i = 0; i < snake_len; i++) {
        snake[i].x = 10 - i;
        snake[i].y = 10;
    }

    spawn_food();
}

void move_snake(int dx, int dy) {
    int i;
    Point new_head;
    bool ate_food;

    if (dx == 0 && dy == 0) {
        return;
    }

    new_head.x = snake[0].x + dx;
    new_head.y = snake[0].y + dy;
    ate_food = (new_head.x == food.x && new_head.y == food.y);

    if (ate_food && snake_len < 100) {
        for (i = snake_len; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
        snake[0] = new_head;
        snake_len++;
        spawn_food();
    } else {
        for (i = snake_len - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
        snake[0] = new_head;
    }
}

int check_collision(int width, int height) {
    int i;

    if (snake[0].x < 0 || snake[0].x >= width || snake[0].y < 0 || snake[0].y >= height) {
        return 1;
    }

    for (i = 1; i < snake_len; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return 1;
        }
    }

    return 0;
}
