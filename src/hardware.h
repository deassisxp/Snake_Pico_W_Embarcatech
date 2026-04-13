#ifndef HARDWARE_H
#define HARDWARE_H

#include "pico/stdlib.h"

// Definições de Pinos (BitDogLab)
#define JOY_X_PIN    26
#define JOY_Y_PIN    27
#define BUTTON_A     5
#define BUTTON_B     6
#define BUZZER_PIN   21  // PWM para som

extern volatile bool flag_timer_jogo;

void hardware_init();
void read_joystick(int *dx, int *dy);
void play_beep();
void play_game_over_sound();

#endif
