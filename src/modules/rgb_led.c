#include "modules/rgb_led.h"
#include "common.h"
#include "hardware/pwm.h"


volatile uint8_t rgb_brightness = LED_RGB_BRILHO; 

// Mapeamento de brightness (0 a 4) para brightness_level
const uint16_t rgb_brightness_mapping[] = {
    0,   // 0: Brilho 0% (desligado)
    32,  // 1: Brilho 60%
    64,  // 2: Brilho 75%
    128, // 3: Brilho 90%
    256  // 4: Brilho 100% (máximo)
};

void rgb_led_init()
{
    uint slice_11 = pwm_gpio_to_slice_num(LED_VERDE_PIN); // Verde - Slice 5B ** MESMO CLOCK DO BUZZER DIREITO **
    uint slice_12 = pwm_gpio_to_slice_num(LED_AZUL_PIN);  // Azul - Slice 6A ** MESMO CLOCK DO LED VERMELHO **
    uint slice_13 = pwm_gpio_to_slice_num(LED_VERMELHO_PIN);   // Vermelho - Slice 6B ** MESMO CLOCK DO LED AZUL **

    // Configura pinos
    gpio_set_function(LED_VERDE_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LED_AZUL_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LED_VERMELHO_PIN, GPIO_FUNC_PWM);

    // Configura Slice 5B (GPIO11)
    pwm_config green_pwm_config = pwm_get_default_config();
    pwm_config_set_clkdiv(&green_pwm_config, 4.f); // 31.25MHz
    pwm_init(slice_11, &green_pwm_config, true);

    // Configura Slice 6A (GPIO12)
    pwm_config blue_pwm_config = pwm_get_default_config();
    pwm_config_set_clkdiv(&blue_pwm_config, 4.f); // 31.25MHz
    pwm_init(slice_12, &blue_pwm_config, true);

    // Configura Slice 6B (GPIO13)
    pwm_config red_pwm_config = pwm_get_default_config();
    pwm_config_set_clkdiv(&red_pwm_config, 4.f); // 31.25MHz
    pwm_init(slice_13, &red_pwm_config, true);

    pwm_set_gpio_level(LED_VERMELHO_PIN, 0);
    pwm_set_gpio_level(LED_VERDE_PIN, 0);
    pwm_set_gpio_level(LED_AZUL_PIN, 0);
}

void rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t r = red;
    uint8_t g = green;
    uint8_t b = blue;
    
    rgb_led_apply_brightness(&r, &g, &b, rgb_brightness);
    
    pwm_set_gpio_level(LED_VERMELHO_PIN, r);
    pwm_set_gpio_level(LED_VERDE_PIN, g);
    pwm_set_gpio_level(LED_AZUL_PIN, b);
}

// Função para aplicar o brilho aos valores RGB
void rgb_led_apply_brightness(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t brightness)
{
    if (brightness > 4)
        brightness = 4;

    uint16_t brightness_level = rgb_brightness_mapping[brightness];
    
    if (brightness_level > 255)
        brightness_level = 255;

    *r = (uint8_t)((*r * brightness_level) / 255);
    *g = (uint8_t)((*g * brightness_level) / 255);
    *b = (uint8_t)((*b * brightness_level) / 255);
}


// Função para aumentar o brilho
void rgb_led_increase_brightness()
{
    if (rgb_brightness < 4) {
        rgb_brightness++;
    }
}

// Função para diminuir o brilho
void rgb_led_decrease_brightness()
{
    if (rgb_brightness > 1) {
        rgb_brightness--;
    }
}