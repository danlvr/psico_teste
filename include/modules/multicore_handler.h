#ifndef MULTICORE_HANDLER_H
#define MULTICORE_HANDLER_H

#include "pico/stdlib.h"

/**
 * @brief Ponto de entrada para o core 1
 */
void core1_entry(void);

/**
 * @brief Inicializa o multicore handler
 */
void multicore_init(void);

/**
 * @brief Envia uma função para o core 1
 * @param function Função a ser executada no core 1
 */
void multicore_send_to_core_1(void(*function)(uintptr_t));

#endif // MULTICORE_HANDLER_H