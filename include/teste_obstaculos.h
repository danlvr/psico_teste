#ifndef TESTE_OBSTACULOS_H
#define TESTE_OBSTACULOS_H

#include "pico/stdlib.h"

/**
 * @brief Função para desenhar a cena de obstáculos no display de LED
 */
void desenha_cena_obstaculos(const int cena[5][5], int xExam, int yExam);

/**
 * @brief Inicia o teste psicotécnico de obstáculos
 */
void teste_psicotecnico_obstaculos(void);

#endif // TESTE_OBSTACULOS_H