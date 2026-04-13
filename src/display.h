#ifndef DISPLAY_H
#define DISPLAY_H

#include "pico/stdlib.h"

// Dimensões para o Jogo da Cobra (escala de 4x4 pixels por unidade lógica)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

void display_init();
void display_clear();
void display_draw_pixel(int x, int y, bool on);
void display_show(); // Envia o buffer para o hardware
void display_draw_rect(int x, int y, int w, int h, bool on);

#endif