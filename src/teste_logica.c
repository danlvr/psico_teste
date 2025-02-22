#include "teste_logica.h"
#include "common.h"
#include "testes_psi.h"

// Estrutura para armazenar métricas do teste de Lógica para futura exportação
typedef struct
{
    char timestamp[32];            // Timestamp do teste
    int acertos;                   // Total de acertos
    int total_tentativas;          // Total de tentativas realizadas
    float taxa_acerto;             // Taxa de acerto (%)
    uint64_t tempo_medio_resposta; // Tempo médio de resposta em ms
    uint64_t tempo_total;          // Tempo total do teste em ms
    int erros_perseverativos;      // Total de erros perseverativos
} MetricasLogica;

// Arrays com todas as formas possíveis
static int setas[4][5][5] = {
    {// Seta direita →
     {0, 0, 0, 0, 0},
     {0, 0, 0, 1, 0},
     {1, 1, 1, 1, 1},
     {0, 0, 0, 1, 0},
     {0, 0, 0, 0, 0}},
    {// Seta cima ↑
     {0, 0, 1, 0, 0},
     {0, 1, 1, 1, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0}},
    {// Seta esquerda ←
     {0, 0, 0, 0, 0},
     {0, 1, 0, 0, 0},
     {1, 1, 1, 1, 1},
     {0, 1, 0, 0, 0},
     {0, 0, 0, 0, 0}},
    {// Seta baixo ↓
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 1, 1, 1, 0},
     {0, 0, 1, 0, 0}}};

static int quadrados[4][5][5] = {
    {// Superior esquerdo
     {1, 1, 0, 0, 0},
     {1, 1, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}},
    {// Superior direito
     {0, 0, 0, 1, 1},
     {0, 0, 0, 1, 1},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}},
    {// Inferior direito
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 1, 1},
     {0, 0, 0, 1, 1}},
    {// Inferior esquerdo
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {1, 1, 0, 0, 0},
     {1, 1, 0, 0, 0}}};

static int triangulos[4][5][5] = {
    {// Apontando direita
     {0, 0, 1, 0, 0},
     {0, 0, 1, 1, 0},
     {0, 0, 1, 1, 1},
     {0, 0, 1, 1, 0},
     {0, 0, 1, 0, 0}},
    {// Apontando esquerda
     {0, 0, 1, 0, 0},
     {0, 1, 1, 0, 0},
     {1, 1, 1, 0, 0},
     {0, 1, 1, 0, 0},
     {0, 0, 1, 0, 0}},
    {// Apontando baixo
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {1, 1, 1, 1, 1},
     {0, 1, 1, 1, 0},
     {0, 0, 1, 0, 0}},
    {// Apontando cima
     {0, 0, 1, 0, 0},
     {0, 1, 1, 1, 0},
     {1, 1, 1, 1, 1},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}}};

static int blocos[4][5][5] = {
    {// Topo
     {1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}},
    {// Direita
     {0, 0, 0, 1, 1},
     {0, 0, 0, 1, 1},
     {0, 0, 0, 1, 1},
     {0, 0, 0, 1, 1},
     {0, 0, 0, 1, 1}},
    {// Baixo
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1}},
    {// Esquerda
     {1, 1, 0, 0, 0},
     {1, 1, 0, 0, 0},
     {1, 1, 0, 0, 0},
     {1, 1, 0, 0, 0},
     {1, 1, 0, 0, 0}}};

static int mark[4][5][5] = {
    {// Direita
     {0, 0, 0, 0, 0},
     {0, 0, 0, 1, 0},
     {0, 0, 1, 1, 1},
     {0, 0, 0, 1, 0},
     {0, 0, 0, 0, 0}},
    {// Baixo
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 1, 1, 1, 0},
     {0, 0, 1, 0, 0}},
    {// Esquerda
     {0, 0, 0, 0, 0},
     {0, 1, 0, 0, 0},
     {1, 1, 1, 0, 0},
     {0, 1, 0, 0, 0},
     {0, 0, 0, 0, 0}},
    {// Topo
     {0, 0, 1, 0, 0},
     {0, 1, 1, 1, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}}};

// Função auxiliar para desenhar uma forma na matriz de LEDs
void desenha_forma_teste_logica(int forma[5][5])
{
    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            if (forma[y][x])
            {
                led_matrix_set_pixel(x, y, 255, 255, 255);
            }
        }
    }
}

// Teste Psicotécnico - Lógica
void teste_psicotecnico_logica()
{
    teste_iniciado();

    // Limpa a matriz de LEDs e o display no início
    led_matrix_clear();
    led_matrix_write();
    display_clear();

    MetricasLogica metricas;
    strcpy(metricas.timestamp, get_timestamp());

    metricas.acertos = 0;
    metricas.total_tentativas = 0;
    metricas.taxa_acerto = 0;
    metricas.tempo_medio_resposta = 0;
    metricas.tempo_total = 0;
    metricas.erros_perseverativos = 0;

    uint64_t soma_tempo_resposta = 0;
    uint64_t inicio_teste = to_ms_since_boot(get_absolute_time());

    // Array com respostas já tentadas para cada padrão
    int respostas_tentadas[5][4] = {0}; // [padrão][opção]

    //  Estrutura para armazenar opções de formas
    typedef struct
    {
        int tipo;   // 0=seta, 1=quadrado, 2=triangulo, 3=blocos, 4=mark
        int indice; // índice dentro do array do tipo
    } OpcaoForma;

    // Modifique o array de opções para usar a nova estrutura
    OpcaoForma opcoes[4];

    // Para cada teste
    for (int teste = 0; teste < 5; teste++)
    {
        // Zera as respostas tentadas para o teste atual
        for (int i = 0; i < 4; i++)
        {
            respostas_tentadas[teste][i] = 0;
        }

        int(*formas_atual)[5][5];
        int resposta_correta;

        switch (teste)
        {
        case 0:
            formas_atual = setas;
            resposta_correta = 3;
            break;
        case 1:
            formas_atual = quadrados;
            resposta_correta = 3;
            break;
        case 2:
            formas_atual = triangulos;
            resposta_correta = 3;
            break;
        case 3:
            formas_atual = blocos;
            resposta_correta = 3;
            break;
        case 4:
            formas_atual = mark;
            resposta_correta = 3;
            break;
        }

        display_clear();
        display_text_at_location("Preste", 2, 2, 2);
        display_text_at_location("atencao na", 2, 2, 20);
        display_text_at_location("sequencia", 2, 2, 38);
        sleep_ms(3000);

        // Mostra sequência inicial (3 primeiras formas)
        for (int i = 0; i < 3; i++)
        {
            led_matrix_clear();
            desenha_forma_teste_logica(formas_atual[i]);
            led_matrix_write();
            sleep_ms(1000);
            led_matrix_clear();
            led_matrix_write();
            sleep_ms(500);
        }

        // Fase de escolha
        // Prepara 4 opções:
        // 1. A resposta correta
        // 2. Uma forma errada do mesmo tipo
        // 3 e 4. Duas formas aleatórias de outros tipos
        int tipo_atual = (teste == 0) ? 0 : // setas
                             (teste == 1) ? 1
                                          : // quadrados
                             (teste == 2) ? 2
                                          : // triangulos
                             (teste == 3) ? 3
                                          : // blocos
                             (teste == 4) ? 4
                                          : 1; // mark (ou quadrados como fallback)

        // 1. A resposta correta
        opcoes[0].tipo = tipo_atual;
        opcoes[0].indice = resposta_correta;

        // 2. Uma forma errada do mesmo tipo
        do
        {
            opcoes[1].tipo = tipo_atual;
            opcoes[1].indice = get_rand_32() % 4;
        } while (opcoes[1].indice == resposta_correta);

        // 3 e 4. Duas formas aleatórias de outros tipos
        for (int i = 2; i < 4; i++)
        {
            int tipo_aleatorio;
            do
            {
                tipo_aleatorio = get_rand_32() % 5;
            } while (tipo_aleatorio == tipo_atual);

            opcoes[i].tipo = tipo_aleatorio;
            opcoes[i].indice = get_rand_32() % 4;
        }

        // Embaralha as opções
        for (int i = 3; i > 0; i--)
        {
            int j = get_rand_32() % (i + 1);
            OpcaoForma temp = opcoes[i];
            opcoes[i] = opcoes[j];
            opcoes[j] = temp;
        }

        led_matrix_clear();
        led_matrix_write();
        sleep_ms(500);

        // 2. Fase de escolha
        display_clear();
        display_text_at_location("Escolha a", 2, 2, 2);
        display_text_at_location("opcao", 2, 2, 20);
        display_text_at_location("correta", 2, 2, 38);
        sleep_ms(1000);

        bool respondeu_correto = false;
        int opcao_atual = 0;

        uint64_t inicio_resposta = to_ms_since_boot(get_absolute_time());

        while (!respondeu_correto)
        {
            // Calcula opções disponíveis
            int opcoes_disponiveis = 4;
            for (int i = 0; i < 4; i++)
            {
                if (respostas_tentadas[teste][i])
                {
                    if (opcoes_disponiveis > 2)
                    {
                        opcoes_disponiveis--;
                    }
                }
            }

            led_matrix_clear();

            if (opcoes[opcao_atual].indice >= 0 && opcoes[opcao_atual].indice < 4)
            {
                // Mostra opção atual baseada no tipo de forma do teste atual
                switch (opcoes[opcao_atual].tipo)
                {
                case 0:
                    desenha_forma_teste_logica(setas[opcoes[opcao_atual].indice]);
                    break;
                case 1:
                    desenha_forma_teste_logica(quadrados[opcoes[opcao_atual].indice]);
                    break;
                case 2:
                    desenha_forma_teste_logica(triangulos[opcoes[opcao_atual].indice]);
                    break;
                case 3:
                    desenha_forma_teste_logica(blocos[opcoes[opcao_atual].indice]);
                    break;
                case 4:
                    desenha_forma_teste_logica(mark[opcoes[opcao_atual].indice]);
                    break;
                }
            }

            led_matrix_write();

            // Leitura do joystick e botões
            uint16_t vx, vy;
            joystick_read_axis(&vx, &vy);

            if (joystick_is_left_right(vx) == 1 | joystick_is_up_down(vy) == 1)
            {
                do
                {
                    opcao_atual = (opcao_atual + 1) % 4;
                } while (respostas_tentadas[teste][opcao_atual] && opcoes_disponiveis > 1);
            }
            else if (joystick_is_left_right(vx) == -1 | joystick_is_up_down(vy) == -1)
            {
                do
                {
                    opcao_atual = (opcao_atual + 3) % 4;
                } while (respostas_tentadas[teste][opcao_atual] && opcoes_disponiveis > 1);
            }

            sleep_ms(100);

            if (botao_b_pressionado())
            {
                metricas.total_tentativas++;
                // Verifica se a opção escolhida é a correta
                if (opcoes[opcao_atual].tipo == tipo_atual &&
                    opcoes[opcao_atual].indice == resposta_correta)
                {
                    respondeu_correto = true;
                    metricas.acertos++;
                    soma_tempo_resposta += to_ms_since_boot(get_absolute_time()) - inicio_resposta;
                    play_success_sound();
                }
                else
                {
                    respostas_tentadas[teste][opcao_atual] = 1;

                    if (opcoes_disponiveis > 1)
                    {
                        opcao_atual = (opcao_atual + 1) % 4;
                    }

                    metricas.erros_perseverativos++;
                    play_error_sound();
                }
                led_matrix_clear();
                led_matrix_write();
            }

            sleep_ms(100);
        }
    }

    // Cálculo das métricas finais
    metricas.tempo_total = to_ms_since_boot(get_absolute_time()) - inicio_teste;
    metricas.taxa_acerto = (float)(metricas.acertos) / metricas.total_tentativas * 100;
    metricas.tempo_medio_resposta = soma_tempo_resposta / (metricas.acertos > 0 ? metricas.acertos : 1);

    strcpy(metricas.timestamp, get_timestamp());

    // Exibe resultados
    printf("\nTeste LÓGICA concluído!\n");
    printf("\n%s\n", metricas.timestamp);
    printf("\n=== Metricas do Teste ===\n");
    printf("Taxa de acerto: %.1f%% (%d/%d)\n", metricas.taxa_acerto, metricas.acertos, metricas.total_tentativas);
    printf("Tempo medio de resposta: %llu ms\n", metricas.tempo_medio_resposta);
    printf("Tempo total: %llu ms\n", metricas.tempo_total);
    printf("Erros perseverativos: %d\n", metricas.erros_perseverativos);
    printf("=====================\n");

    teste_concluido();
}