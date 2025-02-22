#include "teste_cores.h"
#include "common.h"
#include "testes_psi.h"

// Estrutura para armazenar métricas do teste de Cores para futura exportação
typedef struct
{
    char timestamp[32];   // Timestamp do teste
    int acertos;          // Total de acertos
    int total_rodadas;    // Total de rodadas
    uint64_t tempo_medio; // Tempo médio de resposta em ms
    int erros_vermelho;   // Erros específicos com a cor vermelha
    int erros_verde;      // Erros específicos com a cor verde
    int erros_amarelo;     // Erros específicos com a cor amarela
} MetricasCores;

// Teste Psicotécnico - Cores
void teste_psicotecnico_cores()
{
    teste_iniciado();
    
    display_clear();
    display_text_at_location("Aperte", 2, 2, 2);
    display_text_at_location("o botao", 2, 2, 20);
    display_text_at_location("da cor", 2, 2, 38);
    sleep_ms(3000);

    MetricasCores metricas;
    strcpy(metricas.timestamp, get_timestamp());
    metricas.total_rodadas = 30;
    metricas.tempo_medio = 0;
    metricas.acertos = 0;
    metricas.erros_vermelho = 0;
    metricas.erros_verde = 0;
    metricas.erros_amarelo = 0;

    uint64_t soma_tempo = 0;
    uint64_t inicio, fim;
    uint64_t tempos[30];

    for (int i = 0; i < metricas.total_rodadas; i++)
    {
        int cor = get_rand_32() % 3;
        led_matrix_clear();
        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                if (cor == 0) // Vermelho
                    led_matrix_set_pixel(x, y, 255, 0, 0);
                else if (cor == 1) // Verde
                    led_matrix_set_pixel(x, y, 0, 255, 0);
                else // Amarelo
                    led_matrix_set_pixel(x, y, 255, 255, 0);
            }
        }
        led_matrix_write();

        inicio = to_ms_since_boot(get_absolute_time());
        bool acertou = false;        
        while ((to_ms_since_boot(get_absolute_time()) - inicio) < 2000)
        {
            // Lê estados apenas UMA vez no loop
            bool pressedA = botao_a_pressionado();
            bool pressedB = botao_b_pressionado();
            bool pressedJoy = botao_joystick_pressionado();

            if (pressedA && cor == 2)
            {
                acertou = true;
                play_success_sound();
                break;
            }
            else if (pressedB && cor == 1)
            {
                acertou = true;
                play_success_sound();
                break;
            }
            else if (pressedJoy && cor == 0)
            {
                acertou = true;
                play_success_sound();
                break;
            }
            else if (pressedA || pressedB || pressedJoy)
            {
                // Botão pressionado incorreto

                play_error_sound();

                if (cor == 0)
                    metricas.erros_vermelho++;
                if (cor == 1)
                    metricas.erros_verde++;
                if (cor == 2)
                    metricas.erros_amarelo++;
                break;
            }
            sleep_ms(10);
        }

        fim = to_ms_since_boot(get_absolute_time());
        tempos[i] = fim - inicio;
        if (acertou)
        {
            metricas.acertos++;
        }

        // Apaga matriz ao final de cada rodada de teste
        led_matrix_clear();
        led_matrix_write();

        // Intervalo aleatório entre estímulos
        int intervalo_aleatorio = 1000 + (get_rand_32() % 701);
        sleep_ms(intervalo_aleatorio);
    }

    // Estatísticas básicas
    for (int i = 0; i < metricas.total_rodadas; i++)
    {
        soma_tempo += tempos[i];
    }
    metricas.tempo_medio = soma_tempo / metricas.total_rodadas;
    strcpy(metricas.timestamp, get_timestamp());

    // Exibe resultados no console
    printf("\nTeste CORES concluído!\n");
    printf("\n%s\n", metricas.timestamp);
    printf("\n=== Metricas do Teste ===\n");
    printf("Acertos: %d/%d\n", metricas.acertos, metricas.total_rodadas);
    printf("Tempo médio: %llu ms\n", metricas.tempo_medio);
    printf("Erros Vermelho: %d\n", metricas.erros_vermelho);
    printf("Erros Verde: %d\n", metricas.erros_verde);
    printf("Erros Amarelo: %d\n", metricas.erros_amarelo);
    printf("=====================\n");    

    teste_concluido();
}