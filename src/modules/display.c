#include "modules/display.h"
#include "common.h"
#include "drivers/ssd1306.h"

const uint8_t num_chars_per_disp[] = {7, 7, 7, 5}; // Número de caracteres por display
static ssd1306_t disp; // Display OLED

// Inicializa o display OLED I2C
void display_init(void)
{
    i2c_init(I2C_PORT, I2C_FREQUENCY);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    disp.external_vcc = false;
    ssd1306_init(&disp, OLED_WIDTH, OLED_HEIGHT, I2C_ADDRESS, I2C_PORT);
    ssd1306_clear(&disp);
}

// Limpa o display
void display_clear(void)
{
    ssd1306_clear(&disp);
    ssd1306_show(&disp);
}

// Exibe um texto no display na posição (x, y)
void display_text_at_location(const char *text, int size, int x, int y)
{
    ssd1306_draw_string(&disp, x, y, size, text);
    ssd1306_show(&disp);
}

// Desenha um quadrado
void draw_square(int x, int y, int width, int height)
{
    ssd1306_draw_empty_square(&disp, x, y, width, height);
    ssd1306_show(&disp);
}

// Desenha um pixel na posição (x, y)
void display_draw_pixel(int x, int y)
{
    ssd1306_draw_pixel(&disp, x, y);
}

// Limpa um pixel na posição (x, y)
void display_clear_pixel(int x, int y)
{
    ssd1306_clear_pixel(&disp, x, y);
}

// Exibe o buffer no display
void display_show(void)
{
    ssd1306_show(&disp);
}