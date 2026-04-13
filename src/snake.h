#ifndef SNAKE_H
#define SNAKE_H

typedef struct {
    int x;
    int y;
} Point;

// Funções de lógica
void init_game();
void move_snake(int dx, int dy);
int check_collision(int width, int height);
extern Point snake[100]; // Permite que o display.c veja a cobra para desenhar
extern int snake_len;
extern Point food;

#endif