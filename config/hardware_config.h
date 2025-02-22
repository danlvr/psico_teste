#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

// // // // // // // // // // // //
//  Define os pinos de hardware  //
// // // // // // // // // // // //

#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15

#define JOYSTICK_VRY_PIN 26
#define JOYSTICK_VRX_PIN 27
#define JOYSTICK_BTN_PIN 22 // Botão Vermelho

#define BTN_A_PIN 5 // Botão Amarelo
#define BTN_B_PIN 6 // Botão Verde

#define LED_VERDE_PIN 11
#define LED_AZUL_PIN 12
#define LED_VERMELHO_PIN 13

#define LED_MATRIX_PIN 7

#define BUZZER_ESQ_PIN 21
#define BUZZER_DIR_PIN 10

// // // // // // // // // // // //
//   Configurações dos módulos   //
// // // // // // // // // // // //

// Define o tempo de debounce dos botões
#define BTN_DEBOUNCE_TIME_MS 200

// Configurações do display OLED
#define I2C_PORT i2c1
#define I2C_ADDRESS 0x3C
#define I2C_FREQUENCY 400000
#define OLED_WIDTH 128 // 128 pixels de largura
#define OLED_HEIGHT 64 // 64 pixels de altura

// Configurações do joystick
#define ADC_CHANNEL_0 0 // Canal ADC para o eixo Y do joystick
#define ADC_CHANNEL_1 1 // Canal ADC para o eixo X do joystick
#define JOYSTICK_TRIGGER_THRESHOLD 1000 // Limiar para acionar o joystick

// Configurações da matriz de LEDs
#define LED_MATRIX_COUNT 25 // Quantidade de LEDs na matriz
#define LED_MATRIX_BRILHO 2 // Brilho inicial da matriz de LEDs - 0 (desligado) a 8 (máximo)

// Configurações do LED RGB
#define LED_RGB_BRILHO 3 // Brilho inicial do LEDs RGB - 0 (desligado) a 4 (máximo)

// Configurações do buzzer
#define BUZZER_VOLUME 2 // Volume inicial dos buzzers - 0 (desligado) a 4 (máximo)

// Configurações NTP (Network Time Protocol) para sincronização de tempo
// Lembrar de ciar o arquivo credentials.h com as credenciais de rede (ver exemplo credentials_example.h)
#define NTP_SERVER "pool.ntp.org"
#define NTP_MSG_LEN 48
#define NTP_PORT 123
#define NTP_DELTA 2208988800 // seconds between 1 Jan 1900 and 1 Jan 1970
#define NTP_TEST_TIME (30 * 1000)
#define NTP_RESEND_TIME (10 * 1000)

#endif