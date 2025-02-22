#include "testes_psi.h"
#include "common.h"
#include "menu.h"

// Testes
#include "teste_cores.h"
#include "teste_obstaculos.h"
#include "teste_previsao.h"
#include "teste_logica.h"

// Função para exibir menu de seleção de testes psicotécnicos
void teste_iniciado()
{
    rgb_led_set_color(255, 255, 255); // LED branco durante todo o teste
    for (int contagem = 3; contagem > 0; contagem--)
    {
        display_clear();
        char texto[16];
        sprintf(texto, "%d", contagem);
        display_text_at_location(texto, 9, 40, 2);
        buzzers_play_tone_both(440, 200); // Toca nota Lá (440Hz) por 200ms
        sleep_ms(800); // Completa o restante do segundo
    }
    display_clear();
}

// Função para exibir menu de seleção de testes psicotécnicos
void teste_concluido()
{
    rgb_led_set_color(0, 255, 0);      // LED verde no final
    play_finish_sound(); // Efeito sonoro
    led_matrix_clear();
    led_matrix_write();
    display_clear();
    display_text_at_location("Teste", 2, 2, 2);
    display_text_at_location("concluido", 2, 2, 20);
    sleep_ms(3000);
    rgb_led_set_color(0, 0, 0);
    display_menu();
}

// Função para iniciar um teste psicotécnico
void iniciar_teste_psicotecnico(int tipo_teste)
{
    switch (tipo_teste)
    {
    case 0:
        teste_psicotecnico_cores();
        break;
    case 1:
        teste_psicotecnico_obstaculos();
        break;
    case 2:
        teste_psicotecnico_previsao();
        break;
    case 3:
        teste_psicotecnico_logica();
        break;
    }
}