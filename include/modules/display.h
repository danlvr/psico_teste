#ifndef DISPLAY_H
#define DISPLAY_H

/**
 * @brief Configura pinos GPIO para comunicação I2C e inicializa o display
 */
void display_init(void);

/**
 * @brief Limpa o display OLED
 */
void display_clear(void);

/**
 * @brief Exibe texto no display OLED na posição especificada
 */
void display_text_at_location(const char *text, int size, int x, int y);

/**
 * @brief Desenha um quadrado no display OLED
 */
void draw_square(int x, int y, int width, int height);

/**
 * @brief Desenha um pixel no display OLED
 */
void display_draw_pixel(int x, int y);

/**
 * @brief Limpa um pixel no display OLED
 */
void display_clear_pixel(int x, int y);

/**
 * @brief Exibe o conteúdo do buffer no display OLED
 */
void display_show(void);

#endif // DISPLAY_H