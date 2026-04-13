#ifndef HARDWARE_H
#define HARDWARE_H

#include "pico/stdlib.h"

// Definições de Pinos (BitDogLab)
#define JOYSTICK_X_PIN 26
#define JOYSTICK_Y_PIN 27
#define BUTTON_A 5 
#define BUTTON_B 6 
#define BUZZER_PIN 21

void hardware_init();
void play_sound(uint freq, uint duration_ms);
uint16_t read_joystick_x();
uint16_t read_joystick_y();

#endif