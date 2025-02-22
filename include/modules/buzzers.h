#ifndef BUZZERS_H
#define BUZZERS_H

#include "pico/stdlib.h"

/**
 * @brief Inicializa os buzzers
 */
void buzzers_init(void);

/**
 * @brief Toca um tom nos dois buzzers 
 * @param frequency Frequência do tom 
 * @param duration Duração do tom
 */
void buzzers_play_tone_both(uint frequency, uint duration);

/**
 * @brief Aplica o volume ao buzzer
 */
int buzzers_apply_volume(int top);

/**
 * @brief Toca o som de introdução
 */
void play_intro_sound(uintptr_t);

/**
 * @brief Toca o som de conclusão do teste
 */
void play_finish_sound(void);

/**
 * @brief Toca um som curto de feedback positivo
 */
void play_success_sound(void);

/**
 * @brief Toca um som de feedback negativo
 */
void play_error_sound(void);

#endif // BUZZERS_H