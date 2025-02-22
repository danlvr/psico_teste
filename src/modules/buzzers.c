#include "modules/buzzers.h"
#include "common.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Volume do buzzer
volatile int buzzer_volume = BUZZER_VOLUME;

// Notas e durações da música de introdução

const uint intro_sound_notes[] = {
    523,  // C5
    587,  // D5
    659,  // E5
    698,  // F5
    784,  // G5
    880,  // A5
    988,  // B5
    1047  // C6
};

const uint intro_sound_duration[] = {
    200,  
    200,
    200,
    200,
    200,
    200,
    200,
    400   
};

// Notas e durações da música de conclusão
const uint finish_sound_notes[] = {
    659,  // E5
    784,  // G5
    988,  // B5
    1047  // C6
};

const uint finish_sound_duration[] = {
    150,  // Duração mais curta para ser mais dinâmica
    150,
    150,
    300   // Última nota mais longa
};


// Mapeamento de volume para o buzzer
const int buzzer_volume_mapping[] = {
    0,     // 0: Desligado
    1500,  // 1: Volume muito baixo 
    1000,  // 2: Volume baixo 
    500,   // 3: Volume médio 
    100,   // 4: Volume alto
    2      // 5: Volume máximo 
};

// Inicializa os pinos do buzzer e configura o PWM
void buzzers_init(void)
{
    // Configure GPIO pins for PWM
    gpio_set_function(BUZZER_ESQ_PIN, GPIO_FUNC_PWM);
    gpio_set_function(BUZZER_DIR_PIN, GPIO_FUNC_PWM);

    // Conseguir o número do slice para o PWM
    uint slice_left = pwm_gpio_to_slice_num(BUZZER_ESQ_PIN); // Slice 2B ** OK **
    uint slice_right = pwm_gpio_to_slice_num(BUZZER_DIR_PIN); // Slice 5A ** MESMO CLOCK DO LED VERDE **

    pwm_config config_left = pwm_get_default_config();
    pwm_config_set_clkdiv(&config_left, 4.0f); // Ajusta divisor de clock
    pwm_init(slice_left, &config_left, true);

    pwm_config config_right = pwm_get_default_config();
    pwm_config_set_clkdiv(&config_right, 4.0f); // Ajusta divisor de clock
    pwm_init(slice_right, &config_right, true);

    pwm_set_gpio_level(slice_left, 0);  // Desliga o PWM inicialmente
    pwm_set_gpio_level(slice_right, 0); // Desliga o PWM inicialmente
}

// Toca uma nota em ambos os buzzers
void buzzers_play_tone_both(uint frequency, uint duration)
{
    uint slice_num_left = pwm_gpio_to_slice_num(BUZZER_ESQ_PIN);
    uint32_t clock_freq_left = clock_get_hz(clk_sys);
    uint32_t top_left = clock_freq_left / frequency - 1;

    uint slice_num_right = pwm_gpio_to_slice_num(BUZZER_DIR_PIN);
    uint32_t clock_freq_right = clock_get_hz(clk_sys);
    uint32_t top_right = clock_freq_right / frequency - 1;

    pwm_set_wrap(slice_num_left, top_left);
    pwm_set_wrap(slice_num_right, top_right);

    pwm_set_gpio_level(BUZZER_ESQ_PIN, buzzers_apply_volume(top_left));
    pwm_set_gpio_level(BUZZER_DIR_PIN, buzzers_apply_volume(top_right));

    sleep_ms(duration);

    pwm_set_gpio_level(BUZZER_ESQ_PIN, 0);  // Desliga o som após a duração
    pwm_set_gpio_level(BUZZER_DIR_PIN, 0); // Desliga o som após a duração
    sleep_ms(50);                            // Pausa entre notas
}

// Aplica o volume ao buzzer
int buzzers_apply_volume(int top) {
    if (buzzer_volume > 4) buzzer_volume = 4;
    int volume_scale = buzzer_volume_mapping[buzzer_volume];
    return top / volume_scale;
}

// Toca a música de introdução
void play_intro_sound(uintptr_t){
    for (int i = 0; i < sizeof(intro_sound_notes) / sizeof(intro_sound_notes[0]); i++) {
        if (intro_sound_notes[i] == 0) {
            sleep_ms(intro_sound_duration[i]);
        } else {
            buzzers_play_tone_both(intro_sound_notes[i], intro_sound_duration[i]);
        }
    }
}

// Toca a música de conclusão de testes
void play_finish_sound(void) {
    for (int i = 0; i < sizeof(finish_sound_notes) / sizeof(finish_sound_notes[0]); i++) {
        buzzers_play_tone_both(finish_sound_notes[i], finish_sound_duration[i]);
    }
}

// Toca um som de acerto (nota aguda curta)
void play_success_sound(void) {
    buzzers_play_tone_both(1047, 150);  // C6 - nota aguda e curta
}

// Toca um som de erro (nota grave mais longa)
void play_error_sound(void) {
    buzzers_play_tone_both(220, 300);   // A3 - nota grave e mais longa
}