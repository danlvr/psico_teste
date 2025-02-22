#include "teste_previsao.h"
#include "common.h"
#include "testes_psi.h"

// Constantes para o teste de previsão
#define CENTRO_X 2
#define CENTRO_Y 2
#define LED_BRANCO 255, 255, 255

// Velocidades de movimento do LED
#define VELOCIDADE_1 200 // ms entre movimentos (lento)
#define VELOCIDADE_2 150 // ms entre movimentos (médio)
#define VELOCIDADE_3 100 // ms entre movimentos (rápido)

// Estrutura para armazenar métricas do teste de Antecipação para futura exportação
typedef struct
{
    char timestamp[32];             // Timestamp do teste
    int acertos_visivel;            // Acertos no modo visível
    int acertos_invisivel;          // Acertos no modo invisível
    int total_tentativas_visivel;   // Total de tentativas modo visível
    int total_tentativas_invisivel; // Total de tentativas modo invisível
    uint64_t tempo_medio_visivel;   // Tempo médio de resposta visível em ms
    uint64_t tempo_medio_invisivel; // Tempo médio de resposta invisível em ms
    float taxa_acerto_visivel;      // Taxa de acerto modo visível (%)
    float taxa_acerto_invisivel;    // Taxa de acerto modo invisível (%)
    float indice_adaptacao;         // Índice de adaptação (%)
} MetricasPrevisao;

// Função auxiliar para mover o LED de acordo com a direção
void mover_led_previsao(int direcao, int *x, int *y, bool direcao_positiva)
{
    switch (direcao)
    {
    case 0: // Horizontal
        *x = direcao_positiva ? (*x + 1) : (*x - 1);
        break;
    case 1: // Vertical
        *y = direcao_positiva ? (*y + 1) : (*y - 1);
        break;
    case 2: // Diagonal
        *x = direcao_positiva ? (*x + 1) : (*x - 1);
        *y = direcao_positiva ? (*y + 1) : (*y - 1);
        break;
    }
}

// Teste Psicotécnico - Previsão
void teste_psicotecnico_previsao()
{
    teste_iniciado();

    display_clear();
    display_text_at_location("Aperte", 2, 2, 2);
    display_text_at_location("o botao", 2, 2, 20);
    display_text_at_location("no centro", 2, 2, 38);
    sleep_ms(3000);

    MetricasPrevisao metricas;
    strcpy(metricas.timestamp, get_timestamp());   
    metricas.acertos_visivel = 0;            
    metricas.acertos_invisivel = 0;          
    metricas.total_tentativas_visivel = 0;   
    metricas.total_tentativas_invisivel = 0; 
    metricas.tempo_medio_visivel = 0;   
    metricas.tempo_medio_invisivel = 0; 
    metricas.taxa_acerto_visivel = 0;      
    metricas.taxa_acerto_invisivel = 0;    
    metricas.indice_adaptacao = 0;    

    uint64_t soma_tempo_visivel = 0;
    uint64_t soma_tempo_invisivel = 0;

    // Direções: 0=horizontal, 1=vertical, 2=diagonal
    for (int direcao = 0; direcao < 3; direcao++)
    {
        // Loop de velocidade com 3 velocidades:
        for (int velocidade = 0; velocidade < 3; velocidade++)
        {
            // Primeiro modo visível, depois invisível
            for (int modo = 0; modo < 2; modo++)
            {
                // Ajuste o delay baseado na velocidade
                int delay;
                switch (velocidade)
                {
                case 0:
                    delay = VELOCIDADE_1;
                    break;
                case 1:
                    delay = VELOCIDADE_2;
                    break;
                case 2:
                    delay = VELOCIDADE_3;
                    break;
                }

                bool modo_invisivel = (modo == 1);

                led_matrix_clear();
                led_matrix_write();

                int led_x = 0, led_y = 0;
                bool direcao_positiva = true;
                bool acertou = false;
                uint64_t tempo_reacao = 0;

                // Define posição inicial baseada na direção
                switch (direcao)
                {
                case 0: // Horizontal
                    led_x = 0;
                    led_y = CENTRO_Y;
                    break;
                case 1: // Vertical
                    led_x = CENTRO_X;
                    led_y = 0;
                    break;
                case 2: // Diagonal
                    led_x = 0;
                    led_y = 0;
                    break;
                }

                // Loop de movimento do LED
                while (!acertou)
                {
                    led_matrix_clear();

                    // Verifica se LED deve estar visível
                    bool led_visivel = !modo_invisivel ||
                                       (abs(led_x - CENTRO_X) > 1 || abs(led_y - CENTRO_Y) > 1) ||
                                       (led_x == CENTRO_X && led_y == CENTRO_Y); // Acende no centro mesmo invisível

                    if (led_visivel)
                    {
                        led_matrix_set_pixel(led_x, led_y, LED_BRANCO);
                    }
                    led_matrix_write();

                    uint64_t inicio_movimento = to_ms_since_boot(get_absolute_time());
                    bool botao_pressionado = false;

                    // Aguarda delay ou botão
                    while ((to_ms_since_boot(get_absolute_time()) - inicio_movimento) < delay)
                    {
                        if (botao_b_pressionado())
                        {
                            botao_pressionado = true;
                            tempo_reacao = to_ms_since_boot(get_absolute_time()) - inicio_movimento;
                            break;
                        }
                        sleep_ms(10);
                    }

                    // Se pressionou botão, verifica acerto
                    if (botao_pressionado)
                    {
                        if (led_x == CENTRO_X && led_y == CENTRO_Y)
                        {
                            acertou = true;
                            play_success_sound();
                        }
                        else
                        {
                            play_error_sound();
                        }
                        break;
                    }

                    // Move LED
                    int old_x = led_x;
                    int old_y = led_y;
                    mover_led_previsao(direcao, &led_x, &led_y, direcao_positiva);

                    // Verifica limites e inverte direção se necessário
                    if (led_x < 0 || led_x > 4 || led_y < 0 || led_y > 4)
                    {
                        led_x = old_x;
                        led_y = old_y;
                        direcao_positiva = !direcao_positiva;
                        mover_led_previsao(direcao, &led_x, &led_y, direcao_positiva);
                    }

                    sleep_ms(delay);
                }

                if (modo_invisivel)
                {
                    metricas.total_tentativas_invisivel++;
                    if (acertou)
                    {
                        metricas.acertos_invisivel++;
                        soma_tempo_invisivel += tempo_reacao;
                    }
                }
                else
                {
                    metricas.total_tentativas_visivel++;
                    if (acertou)
                    {
                        metricas.acertos_visivel++;
                        soma_tempo_visivel += tempo_reacao;
                    }
                }
            }
        }
    }

    // Calcula médias
    metricas.tempo_medio_visivel = soma_tempo_visivel / (metricas.total_tentativas_visivel > 0 ? metricas.total_tentativas_visivel : 1);
    metricas.tempo_medio_invisivel = soma_tempo_invisivel / (metricas.total_tentativas_invisivel > 0 ? metricas.total_tentativas_invisivel : 1);
    metricas.taxa_acerto_visivel = (float)metricas.acertos_visivel / metricas.total_tentativas_visivel * 100;
    metricas.taxa_acerto_invisivel = (float)metricas.acertos_invisivel / metricas.total_tentativas_invisivel * 100;
    metricas.indice_adaptacao = metricas.taxa_acerto_invisivel / metricas.taxa_acerto_visivel * 100;

    strcpy(metricas.timestamp, get_timestamp());

    printf("\nTeste PREVISÃO concluído!\n");
    printf("\n%s\n", metricas.timestamp);
    printf("\n=== Metricas do Teste ===\n");
    printf("Acertos (Visivel): %d/%d (%.1f%%)\n",
        metricas.acertos_visivel, metricas.total_tentativas_visivel, metricas.taxa_acerto_visivel);
    printf("Acertos (Invisivel): %d/%d (%.1f%%)\n",
        metricas.acertos_invisivel, metricas.total_tentativas_invisivel, metricas.taxa_acerto_invisivel);
    printf("Tempo medio (Visivel): %llu ms\n", metricas.tempo_medio_visivel);
    printf("Tempo medio (Invisivel): %llu ms\n", metricas.tempo_medio_invisivel);
    printf("Indice de adaptacao: %.1f%%\n", metricas.indice_adaptacao);
    printf("=====================\n");

    teste_concluido();
}