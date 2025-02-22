#ifndef TESTES_PSI_H
#define TESTES_PSI_H

#include "pico/stdlib.h"

/**
 * @brief Quando o teste é iniciado, o LED RGB acende em branco e um contador de 3 segundos é exibido no display 
 */
void teste_iniciado(void);

/**
 * @brief Quando o teste é concluído, o LED RGB acende em verde, um som é emitido e a mensagem "Teste concluído" é exibida no display
 */
void teste_concluido(void);

/**
 * @brief Inicia um teste psicotécnico específico
 * 
 * @param tipo_teste 0 = Cores, 1 = Obstáculos, 2 = Previsão, 3 = Lógica
 */
void iniciar_teste_psicotecnico(int tipo_teste);

#endif // TESTES_PSICOTECNICOS_H