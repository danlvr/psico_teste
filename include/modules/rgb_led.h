#ifndef RGB_LED_H
#define RGB_LED_H

#include "pico/stdlib.h"

/**
 * @brief Inicializa o LED RGB com PWM para controle de brilho
 */
void rgb_led_init(void);

/**
 * @brief Seta a cor do LED RGB 
 * @param red Valor de vermelho 0-255 
 * @param green Valor de verde 0-255 
 * @param blue Valor de azul 0-255 
 */
void rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief Aplica o brilho ao LED RGB 
 * @param r Ponteiro para o valor de vermelho
 * @param g Ponteiro para o valor de verde
 * @param b Ponteiro para o valor de azul
 * @param brightness Brilho a ser aplicado 0-4
 */
void rgb_led_apply_brightness(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t brightness);

/**
 * @brief Aumenta o brilho do LED RGB
 */
void rgb_led_increase_brightness(void);

/**
 * @brief Diminui o brilho do LED RGB
 */
void rgb_led_decrease_brightness(void);

#endif /* RGB_LED_H */