#include "main.h"
#include "common.h" // Include commum do Projeto
#include "menu.h"

// Core 0
int main()
{ 
    stdio_init_all();    

    display_init();    // Inicializa o display OLED
    joystick_init();   // Inicializa o joystick
    buttons_init();    // Inicializa os botões
    led_matrix_init(); // Inicializa a matriz de LEDs
    rgb_led_init();    // Inicializa o LED RGB
    buzzers_init();    // Inicializa os buzzers
    multicore_init();  // Inicializa o handler para receber e executar funções no Core 1

    mostrar_intro(); // Exibe a tela de introdução do projeto
    multicore_send_to_core_1(wifi_init);  // Inicializa o módulo WiFi pelo Core 1 para evitar lockups
    
    sleep_ms(4000); // Delay para tempo de leitura do texto de boas-vindas
    
    display_menu(); // Exibe o menu principal que inicia a aplicação

    while (1)
    {
        tight_loop_contents(); // Loop vazio para evitar travamento
    }
    
    return 0;
}

void mostrar_intro()
{
    // "Limpa" todos os módulos visuais
    display_clear();
    sleep_ms(25);
    rgb_led_set_color(0, 0, 0);
    sleep_ms(25);
    led_matrix_clear();
    sleep_ms(25);
    led_matrix_write();
    sleep_ms(25);

    // Exibe o nome do projeto com som no Core 1 para sincronização
    multicore_send_to_core_1(play_intro_sound);
    draw_square(0, 0, 127, 63);
    display_text_at_location("Psico", 2, 35, 15);
    display_text_at_location("Teste", 2, 35, 35);

    sleep_ms(2000);

    display_clear();

    // Texto de boas-vindas
    display_text_at_location("Bem-vindo", 1, 10, 4);
    display_text_at_location("ao Psico Teste!", 1, 10, 14);
    display_text_at_location("Teste psicotecnico", 1, 10, 34);
    display_text_at_location("Selecione o teste", 1, 10, 54);
}