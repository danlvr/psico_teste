# Psico Teste - Sistema Embarcado para Aplicação de Testes Psicotécnicos

Este projeto foi desenvolvido como trabalho final da primeira fase do programa de capacitação EmbarcaTech, implementando um sistema de avaliação psicotécnica utilizando conceitos de sistemas embarcados. O hardware final será implementado com o Raspberry Pi Pico W, sendo todo o desenvolvimento e testes realizados na placa BitDogLab, que fornece todos os periféricos necessários para a aplicação.

## Sobre o Projeto

O sistema implementa quatro testes psicotécnicos distintos, cada um avaliando diferentes aspectos cognitivos e motores:

- **Teste de Cores:** Avalia tempo de reação e reconhecimento de cores, registrando acertos, erros específicos por cor e tempo médio de resposta.
- **Teste de Obstáculos:** Analisa coordenação motora através de navegação em percursos, registrando colisões e tempo total.
- **Teste de Previsão:** Avalia capacidade de antecipação em dois modos (visível/invisível), com diferentes velocidades e direções.
- **Teste de Lógica:** Testa memória e raciocínio através de sequências, medindo acertos e erros perseverativos.

## Hardware Utilizado

O desenvolvimento foi realizado na placa BitDogLab, que integra:
- Raspberry Pi Pico W (RP2040)
- Display OLED SSD1306
- Matriz LED WS2812B 5x5
- Joystick analógico
- 2 Botões + botão do joystick
- LED RGB
- 2 Buzzers
- Conectividade WiFi

## Funcionalidades e Aspectos Técnicos

### Interface e Controle
- Menu de navegação via display OLED
- Controle por joystick analógico (ADC)
- Botões com tratamento de interrupção e debounce
- Feedback visual através de display e matriz LED
- Feedback sonoro via buzzers controlados por PWM

### Processamento
- Sistema multicore
  - Core 0: Interface e execução dos testes
  - Core 1: WiFi

### Hardware Abstraction Layer
- I2C para display OLED
- PIO + DMA para controle eficiente da matriz LED
- ADC para leitura do joystick
- PWM para LED RGB e buzzers
- GPIO com interrupções para botões

### Conectividade
- WiFi para sincronização NTP
- Protocolo UDP para comunicação NTP

## Configuração WiFi

Para configurar o WiFi:
1. Copie o arquivo `credentials.example.h` para `credentials.h`
2. Edite `credentials.h` com suas credenciais de rede:
```c
#define WIFI_SSID "seu_ssid"
#define WIFI_PASSWORD "sua_senha"
```
## Configurações Gerais

- hardware_config.h - Configurações do Hardware 
- project_config.h - Configurações do Projeto
- credentials.h - Credenciais Wifi (siga orientação acima)

## Desenvolvimento

Projeto desenvolvido utilizando:
- Visual Studio Code
- Raspberry Pi Pico SDK
- CMake para build system
- Bibliotecas:
  - pico-ssd1306 para display OLED, disponível em [pico-ssd1306 GitHub](https://github.com/daschr/pico-ssd1306)
  - Drivers customizados para periféricos

## Sobre o EmbarcaTech

Este projeto foi desenvolvido como parte do programa EmbarcaTech, que visa capacitar desenvolvedores em sistemas embarcados.

## Sobre a BitDogLab

A BitDogLab é uma placa de desenvolvimento educacional projetada especificamente para o aprendizado de sistemas embarcados. Baseada no Raspberry Pi Pico W, ela integra diversos componentes essenciais como display OLED, matriz LED, joystick, botões e buzzers em uma única plataforma. Esta integração permite o desenvolvimento rápido de protótipos e projetos, sendo ideal para estudantes e entusiastas. O projeto é open-source e pode ser encontrado em [BitDogLab GitHub](https://github.com/BitDogLab/BitDogLab), onde estão disponíveis esquemáticos, documentação e exemplos de uso.

## Agradecimentos

- Equipe EmbarcaTech pelo suporte e conhecimento compartilhado
- Professor Dario Pintor pela mentoria
- Comunidade de sistemas embarcados
- BitDogLab pelo hardware de desenvolvimento

---

*Projeto desenvolvido para fins educacionais como parte do programa EmbarcaTech.*