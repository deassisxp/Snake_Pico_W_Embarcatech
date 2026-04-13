#include "pico/stdlib.h"
#include <stdio.h>

bool game_running = false;
int score = 0;

int main() {
    stdio_init_all();
    hardware_init();
    
    while (true) {
        // Botão A: Start
        if (!gpio_get(BUTTON_A)) {
            init_game();
            game_running = true;
            score = 0;
            printf("Jogo Iniciado!\n");
        }

        // Botão B: Stop
        if (!gpio_get(BUTTON_B) && game_running) {
            game_running = false;
            printf("Jogo Interrompido.\n");
        }

        if (game_running) {
            // 1. Lê Joystick
            // 2. Move Cobra
            // 3. Verifica Colisão
            if (check_collision(32, 16)) {
                game_running = false;
                play_sound(200, 500); // Som de Game Over
                send_score_to_server(score);
            }
            sleep_ms(150); // Velocidade do jogo
        }
    }
}