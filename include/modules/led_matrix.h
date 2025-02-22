#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "pico/stdlib.h"

/**
 * @brief Inicializa a matriz de LEDs com DMA e PIO
 */
void led_matrix_init(void);

/**
 * @brief Atribui uma cor RGB a um LED 
 * @param index Indice do LED 
 * @param r Vermelho 0-255 
 * @param g Verde 0-255
 * @param b Azul 0-255
 */
void led_matrix_set_LED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);

/**
 * @brief Limpa o buffer de pixels
 */
void led_matrix_clear(void);

/**
 * @brief Escreve os dados do buffer nos LEDs
 */
void led_matrix_write(void);

/**
 * @brief Converte coordenadas x e y para o indice do LED
 * @param x Posição X 0-4      
 * @param y Posição Y 0-4
 * @return Indice do LED 
 */
int led_matrix_get_index(int x, int y);

/**
 * @brief Aplica o brilho a um pixel
 */
void led_matrix_apply_brightness(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t brightness);

/**
 * @brief Aumenta o brilho da matriz de LEDs
 */
void led_matrix_increase_brightness(void);

/**
 * @brief Diminui o brilho da matriz de LEDs
 */
void led_matrix_decrease_brightness(void);

/**
 * @brief Seta um pixel na matriz de LEDs
 * @param x Posição X 0-4 
 * @param y Posição Y 0-4
 * @param r Vermelho 0-255
 * @param g Verde 0-255
 * @param b Azul 0-255
 */
void led_matrix_set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);

#endif // LED_MATRIX_H