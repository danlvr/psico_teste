#include "teste_obstaculos.h"
#include "common.h"
#include "testes_psi.h"

#define N_CENAS 10

// Estrutura para armazenar métricas do teste de Obstáculos para futura exportação
typedef struct
{
    char timestamp[32];   // Timestamp do teste
    int colisoes;         // Total de colisões
    int total_movimentos; // Total de movimentos realizados
    uint64_t tempo_total; // Tempo total do teste em ms
} MetricasObstaculos;

 // 1 = Parede, 2 = Obsctaculo, 9 = Próxima tela
 static int cenas[N_CENAS][5][5] = {
    // Cena1
    {
        {1, 9, 9, 9, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}},
    // Cena2
    {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 2, 9},
        {1, 0, 0, 0, 9},
        {1, 0, 2, 0, 9},
        {1, 0, 0, 0, 1}},
    // Cena3
    {
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {0, 2, 0, 0, 1},
        {0, 0, 0, 0, 1},
        {1, 2, 9, 9, 1}},
    // Cena4
    {
        {1, 0, 0, 0, 1},
        {1, 0, 0, 2, 1},
        {1, 0, 0, 0, 9},
        {1, 0, 2, 0, 9},
        {1, 1, 1, 1, 1}},
    // Cena5
    {
        {1, 1, 9, 9, 1},
        {1, 0, 2, 0, 1},
        {0, 0, 0, 0, 1},
        {0, 0, 2, 0, 1},
        {1, 1, 1, 1, 1}},
    // Cena6
    {
        {1, 1, 1, 1, 1},
        {9, 2, 0, 0, 1},
        {9, 0, 2, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 0, 0, 1}},
    // Cena7
    {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 1},
        {1, 9, 9, 9, 1}},
    // Cena8
    {
        {1, 0, 0, 0, 1},
        {1, 2, 2, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 2, 2, 1},
        {1, 9, 9, 9, 1}},
    // Cena9
    {
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 2, 2, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 9, 9, 9, 1}},
    // Cena10
    {
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {9, 9, 9, 9, 9},
        {9, 9, 9, 9, 9}}};

// Função auxiliar para desenhar uma cena com obstáculos na matriz de LEDs
void desenha_cena_obstaculos(const int cena[5][5], int xExam, int yExam)
{
    led_matrix_clear();
    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            if (x == xExam && y == yExam)
            {
                // Examinado em azul
                led_matrix_set_pixel(x, y, 0, 0, 255);
            }
            else if (cena[y][x] == 1)
            {
                // Parede em branco
                led_matrix_set_pixel(x, y, 255, 255, 255);
            }
            else if (cena[y][x] == 2)
            {
                // Obstáculo em vermelho
                led_matrix_set_pixel(x, y, 255, 0, 0);
            }
            else if (cena[y][x] == 9)
            {
                // Caminho correto em verde
                led_matrix_set_pixel(x, y, 0, 255, 0);
            }
        }
    }
    led_matrix_write();
}

// Teste Psicotécnico - Obstáculos
void teste_psicotecnico_obstaculos()
{
    teste_iniciado();

    display_clear();
    display_text_at_location("Desvie", 2, 2, 2);
    display_text_at_location("obstaculos", 2, 2, 20);
    sleep_ms(3000);

    MetricasObstaculos metricas;
    strcpy(metricas.timestamp, get_timestamp());

    metricas.colisoes = 0;
    metricas.total_movimentos = 0;
    metricas.tempo_total = 0;

    uint64_t inicio_teste = to_ms_since_boot(get_absolute_time());

    // Posição inicial do examinado na primeira cena
    int examinadoX = 2;
    int examinadoY = 3;

    for (int iCena = 0; iCena < N_CENAS; iCena++)
    {
        bool cenaEmAndamento = true;
        while (cenaEmAndamento)
        {
            desenha_cena_obstaculos(cenas[iCena], examinadoX, examinadoY);

            // Lê joystick
            uint16_t vx, vy;
            joystick_read_axis(&vx, &vy);
            int velhoX = examinadoX;
            int velhoY = examinadoY;

            // Movimenta examinado
            if (joystick_is_left_right(vx) == 1 && examinadoX < 4)
                examinadoX++;
            else if (joystick_is_left_right(vx) == -1 && examinadoX > 0)
                examinadoX--;
            if (joystick_is_up_down(vy) == 1 && examinadoY > 0)
                examinadoY--;
            else if (joystick_is_up_down(vy) == -1 && examinadoY < 4)
                examinadoY++;

            // Se não mudou posição, volta pro loop
            if (examinadoX == velhoX && examinadoY == velhoY)
            {
                sleep_ms(10);
                continue;
            }

            // Se houve movimento (posição mudou)
            if (!(examinadoX == velhoX && examinadoY == velhoY))
            {
                metricas.total_movimentos++;
            }

            // Verifica a célula-alvo
            int alvo = cenas[iCena][examinadoY][examinadoX];

            if (alvo == 1)
            {
                // Parede
                metricas.colisoes++;
                play_error_sound();
                examinadoX = velhoX;
                examinadoY = velhoY;
            }
            else if (alvo == 2)
            {
                // Obstáculo
                metricas.colisoes++;
                play_error_sound();
                examinadoX = velhoX;
                examinadoY = velhoY;
            }
            else if (alvo == 9)
            {
                // Porta de saída desta cena
                play_success_sound();
                cenaEmAndamento = false;
                if (iCena < N_CENAS - 1)
                {
                    // Guarda posição atual para determinar direção da transição
                    int saidaX = examinadoX;
                    int saidaY = examinadoY;

                    // Se saiu pela parte superior (y=0), entra pela parte inferior
                    if (saidaY == 0)
                    {
                        examinadoY = 4;
                        examinadoX = saidaX; // mantém mesma coluna
                    }
                    // Se saiu pela parte inferior (y=4), entra pela parte superior
                    else if (saidaY == 4)
                    {
                        examinadoY = 0;
                        examinadoX = saidaX; // mantém mesma coluna
                    }
                    // Se saiu pela direita (x=4), entra pela esquerda
                    else if (saidaX == 4)
                    {
                        examinadoX = 0;
                        examinadoY = saidaY; // mantém mesma linha
                    }
                    // Se saiu pela esquerda (x=0), entra pela direita
                    else if (saidaX == 0)
                    {
                        examinadoX = 4;
                        examinadoY = saidaY; // mantém mesma linha
                    }
                }
            }
            else
            {
                // 0 = caminho livre, fica como está
            }

            sleep_ms(10);
        }
    }

    // Calcula tempo total em millisegundos
    uint64_t fim_teste = to_ms_since_boot(get_absolute_time());
    metricas.tempo_total = fim_teste - inicio_teste;
    
    strcpy(metricas.timestamp, get_timestamp());

    // Concluiu todas as cenas - Exibe métricas
    printf("\nTeste OBSTÁCULOS concluído!\n");
    printf("\n%s\n", metricas.timestamp);
    printf("\n=== Metricas do Teste ===\n");
    printf("Total de colisoes: %d\n", metricas.colisoes);
    printf("Total de movimentos: %d\n", metricas.total_movimentos);
    printf("Tempo total: %llu ms\n", metricas.tempo_total);
    printf("=====================\n");

    teste_concluido();
}