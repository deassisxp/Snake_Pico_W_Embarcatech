#include "hardware.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "pico/time.h"

volatile bool flag_timer_jogo = false;
static uint16_t joy_center_x = 2048;
static uint16_t joy_center_y = 2048;

static uint16_t read_adc_avg(uint channel, int samples) {
    uint32_t acc = 0;
    int i;

    for (i = 0; i < samples; i++) {
        adc_select_input(channel);
        acc += adc_read();
        sleep_us(300);
    }

    return (uint16_t)(acc / (uint32_t)samples);
}

bool timer_callback(struct repeating_timer *t) {
    flag_timer_jogo = true;
    return true;
}

void hardware_init() {
    const uint x_adc = JOY_X_PIN - 26;
    const uint y_adc = JOY_Y_PIN - 26;

    adc_init();
    adc_gpio_init(JOY_X_PIN);
    adc_gpio_init(JOY_Y_PIN);
    joy_center_x = read_adc_avg(x_adc, 32);
    joy_center_y = read_adc_avg(y_adc, 32);

    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_clkdiv(slice, 125.0f);
    pwm_set_wrap(slice, 255);
    pwm_set_enabled(slice, true);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    static struct repeating_timer timer;
    add_repeating_timer_ms(100, timer_callback, NULL, &timer);
}

void read_joystick(int *dx, int *dy) {
    int x_centered;
    int y_centered;
    int ax;
    int ay;
    const int deadzone = 450;
    const uint x_adc = JOY_X_PIN - 26;
    const uint y_adc = JOY_Y_PIN - 26;

    *dx = 0;
    *dy = 0;

    adc_select_input(x_adc);
    x_centered = (int)adc_read() - (int)joy_center_x;
    adc_select_input(y_adc);
    y_centered = (int)adc_read() - (int)joy_center_y;

    ax = x_centered < 0 ? -x_centered : x_centered;
    ay = y_centered < 0 ? -y_centered : y_centered;

    if (ax < deadzone && ay < deadzone) {
        return;
    }

    if (ax >= ay) {
        // X fisico controla eixo vertical (cima/baixo)
        *dx = 0;
        *dy = (x_centered > 0) ? -1 : 1;
    } else {
        // Y fisico controla eixo horizontal (esquerda/direita), invertido
        *dx = (y_centered > 0) ? 1 : -1;
        *dy = 0;
    }
}

void play_beep() {
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_gpio_level(BUZZER_PIN, 127);
    sleep_ms(50);
    pwm_set_gpio_level(BUZZER_PIN, 0);
}

void play_game_over_sound() {
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);

    pwm_set_gpio_level(BUZZER_PIN, 127);
    sleep_ms(120);
    pwm_set_gpio_level(BUZZER_PIN, 0);
    sleep_ms(40);

    pwm_set_gpio_level(BUZZER_PIN, 127);
    sleep_ms(180);
    pwm_set_gpio_level(BUZZER_PIN, 0);
    sleep_ms(40);

    pwm_set_gpio_level(BUZZER_PIN, 127);
    sleep_ms(260);
    pwm_set_gpio_level(BUZZER_PIN, 0);
}
