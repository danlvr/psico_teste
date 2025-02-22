#ifndef BUTTONS_H
#define BUTTONS_H

#include "pico/stdlib.h"

/**
 * @brief Função de callback para interrupção dos botões
 */
void gpio_callback(uint gpio, uint32_t events);

/**
 * @brief Inicializa os pinos GPIO dos botões
 */
void buttons_init(void);

/**
 * @brief Retorna a flag para botão A pressionado
 */
bool botao_a_pressionado(void);

/**
 * @brief Retorna a flag para botão B pressionado
 */
bool botao_b_pressionado(void);

/**
 * @brief Retorna a flag para botão do joystick pressionado
 */
bool botao_joystick_pressionado(void);

#endif // BUTTONS_H