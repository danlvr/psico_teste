#ifndef COMMON_H
#define COMMON_H

// Bibliotecas do C
#include <stdio.h> 
#include <string.h>

// Biblioteca Pico padrão
#include "pico/stdlib.h"

// Módulos
#include "modules/buttons.h" // Módulo de botões
#include "modules/buzzers.h" // Módulo de buzzers
#include "modules/display.h" // Módulo de display OLED
#include "modules/joystick.h" // Módulo de joystick
#include "modules/led_matrix.h" // Módulo de matriz de LEDs
#include "modules/multicore_handler.h" // Handler para execução de funções no Core 1
#include "modules/rgb_led.h" // Módulo de LED RGB
#include "modules/wifi_rtc.h" // Módulo de WiFi + RTC para sincronização de tempo

// Configurações
#include "hardware_config.h" // Configurações de hardware como pinagem e definições de módulos
#include "project_config.h" // Configurações do projeto
#include "credentials.h" // Credenciais de WiFi

#endif // COMMON_H