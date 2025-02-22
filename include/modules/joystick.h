#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "pico/stdlib.h"

/**
 * @brief Configura pinos GPIO para leitura do joystick (eixos X e Y) e inicializa o ADC.
 */
void joystick_init(void);

/**
 * @brief Lê os valores dos eixos do joystick (X e Y).
 *
 * Esta função lê os valores analógicos dos eixos X e Y do joystick
 * e os armazena nos ponteiros fornecidos.
 *
 * @param vrx_value Ponteiro para armazenar o valor do eixo X (0-4095)
 * @param vry_value Ponteiro para armazenar o valor do eixo Y (0-4095)
 */
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value);

/**
 * @brief Verifica se o joystick está na posição UP ou DOWN.
 */
int joystick_is_up_down(uint16_t vry_value);

/**
 * @brief Verifica se o joystick está na posição LEFT ou RIGHT.
 */
int joystick_is_left_right(uint16_t vrx_value);

#endif // JOYSTICK_H