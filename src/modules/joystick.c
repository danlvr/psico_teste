#include "modules/joystick.h"
#include "common.h"
#include "hardware/adc.h"

// Função para configurar o joystick (pinos de leitura e ADC)
void joystick_init()
{
  adc_init();                      
  adc_gpio_init(JOYSTICK_VRY_PIN); 
  adc_gpio_init(JOYSTICK_VRX_PIN); 
}

// Função para ler os valores dos eixos X e Y do joystick
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value)
{
  adc_select_input(ADC_CHANNEL_0); 
  sleep_ms(50);                    
  *vry_value = adc_read();         // Lê o valor do eixo X (0-4095)

  adc_select_input(ADC_CHANNEL_1); 
  sleep_ms(50);                     
  *vrx_value = adc_read();         // Lê o valor do eixo Y (0-4095) 
}

// Função para verificar se o joystick está pressionado para cima, para baixo ou no centro
int joystick_is_up_down(uint16_t vry_value)
{  
  if (vry_value > 4095 - JOYSTICK_TRIGGER_THRESHOLD)
    return 1; // Cima
  if (vry_value < 0 + JOYSTICK_TRIGGER_THRESHOLD)
    return -1; // Baixo
  return 0;    // Centro
}

// Função para verificar se o joystick está pressionado para a esquerda, para a direita ou no centro
int joystick_is_left_right(uint16_t vrx_value)
{
  if (vrx_value > 4095 - JOYSTICK_TRIGGER_THRESHOLD)
    return 1; // Direita
  if (vrx_value < 0 + JOYSTICK_TRIGGER_THRESHOLD)
    return -1; // Esquerda
  return 0;   // Centro
}