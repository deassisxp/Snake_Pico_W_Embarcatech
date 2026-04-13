#include "display.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c1
#define ADDR 0x3C // Endereço padrão do SSD1306

// Buffer de imagem (128x64 pixels / 8 bits por byte)
static uint8_t buffer[1024];

void ssd1306_command(uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};
    i2c_write_blocking(I2C_PORT, ADDR, buf, 2, false);
}

void display_init() {
    // Inicializa I2C a 400Khz (Fast Mode)
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(14, GPIO_FUNC_I2C); // SDA
    gpio_set_function(15, GPIO_FUNC_I2C); // SCL
    gpio_pull_up(14);
    gpio_pull_up(15);

    // Sequência de inicialização padrão SSD1306
    ssd1306_command(0xAE); // Display OFF
    ssd1306_command(0xD5); ssd1306_command(0x80); // Clock div
    ssd1306_command(0xA8); ssd1306_command(63);   // Multiplex
    ssd1306_command(0xD3); ssd1306_command(0x00); // Offset
    ssd1306_command(0x40); // Start line
    ssd1306_command(0x8D); ssd1306_command(0x14); // Charge pump
    ssd1306_command(0x20); ssd1306_command(0x00); // Memory mode (Horizontal)
    ssd1306_command(0xA1); // Segment remap
    ssd1306_command(0xC8); // COM scan direction
    ssd1306_command(0xDA); ssd1306_command(0x12); // COM pins
    ssd1306_command(0x81); ssd1306_command(0xCF); // Contrast
    ssd1306_command(0xD9); ssd1306_command(0xF1); // Pre-charge
    ssd1306_command(0xDB); ssd1306_command(0x40); // VCOM detect
    ssd1306_command(0xA4); // Resume RAM content
    ssd1306_command(0xA6); // Normal display
    ssd1306_command(0xAF); // Display ON
    
    display_clear();
    display_show();
}

void display_clear() {
    for (int i = 0; i < 1024; i++) buffer[i] = 0;
}

void display_draw_pixel(int x, int y, bool on) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) return;
    if (on)
        buffer[x + (y / 8) * 128] |= (1 << (y % 8));
    else
        buffer[x + (y / 8) * 128] &= ~(1 << (y % 8));
}

void display_draw_rect(int x, int y, int w, int h, bool on) {
    for (int i = x; i < x + w; i++)
        for (int j = y; j < y + h; j++)
            display_draw_pixel(i, j, on);
}

void display_show() {
    // Configura a janela de escrita para o display inteiro
    ssd1306_command(0x21); ssd1306_command(0); ssd1306_command(127); // Column addr
    ssd1306_command(0x22); ssd1306_command(0); ssd1306_command(7);   // Page addr

    uint8_t payload[1025];
    payload[0] = 0x40; // Indicador de dados de data
    for (int i = 0; i < 1024; i++) payload[i+1] = buffer[i];

    i2c_write_blocking(I2C_PORT, ADDR, payload, 1025, false);
}