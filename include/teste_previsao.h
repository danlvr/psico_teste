#ifndef TESTE_PREVISAO_H
#define TESTE_PREVISAO_H

#include "pico/stdlib.h"

/**
 * @brief Função para mover o LED de exame na cena de obstáculos
 */
void mover_led_previsao(int direcao, int *x, int *y, bool direcao_positiva);

/**
 * @brief Inicia o teste psicotécnico de antecipação
 */
void teste_psicotecnico_previsao(void);

#endif // TESTE_PREVISAO_H