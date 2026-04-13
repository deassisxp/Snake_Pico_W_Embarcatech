#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "src/hardware.h"
#include "src/display.h"
#include "src/snake.h"
#include "src/wifi_client.h"
#include "src/wifi_config.h"

#define GRID_W 32
#define GRID_H 16
#define CELL_SIZE 4

bool jogo_ativo = false;
int score = 0;
static int dir_x = 1;
static int dir_y = 0;

static void draw_game(void) {
    int i;

    display_clear();
    for (i = 0; i < snake_len; i++) {
        display_draw_rect(snake[i].x * CELL_SIZE, snake[i].y * CELL_SIZE, CELL_SIZE, CELL_SIZE, true);
    }
    display_draw_rect(food.x * CELL_SIZE, food.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, true);
    display_show();
}

int main() {
    stdio_init_all();
    hardware_init();
    display_init();

    if (cyw43_arch_init()) {
        printf("Erro Wi-Fi\n");
    }
    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 15000)) {
        printf("Falha ao conectar no Wi-Fi\n");
    } else {
        printf("Wi-Fi conectado\n");
    }

    while (true) {
        if (!gpio_get(BUTTON_A) && !jogo_ativo) {
            jogo_ativo = true;
            score = 0;
            dir_x = 1;
            dir_y = 0;
            init_game();
            draw_game();
            printf("Jogo Iniciado!\n");
            play_beep();
            sleep_ms(200);
        }

        if (!gpio_get(BUTTON_B) && jogo_ativo) {
            jogo_ativo = false;
            display_clear();
            display_show();
            printf("Jogo Parado.\n");
            sleep_ms(200);
        }

        if (flag_timer_jogo) {
            if (jogo_ativo) {
                int dx = 0;
                int dy = 0;
                int prev_food_x;
                int prev_food_y;
                read_joystick(&dx, &dy);

                if (!(dx == -dir_x && dy == -dir_y) && (dx != 0 || dy != 0)) {
                    dir_x = dx;
                    dir_y = dy;
                }

                prev_food_x = food.x;
                prev_food_y = food.y;
                move_snake(dir_x, dir_y);

                if (snake[0].x == prev_food_x && snake[0].y == prev_food_y) {
                    score += 10;
                    play_beep();
                }

                if (check_collision(GRID_W, GRID_H)) {
                    jogo_ativo = false;
                    play_game_over_sound();
                    send_score_to_server(score);
                    printf("Game Over! Score: %d\n", score);
                }

                draw_game();
            }
            flag_timer_jogo = false;
        }

        cyw43_arch_poll();
    }

    return 0;
}
