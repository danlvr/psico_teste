#include "modules/led_matrix.h"
#include "common.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "ws2818b.pio.h"

// Definição de pixel GRB
struct __attribute__((packed)) pixel_t {
    uint8_t G, R, B;
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t;

npLED_t leds[LED_MATRIX_COUNT];

PIO led_matrix_pio;
uint pio_state_machine;

static int dma_chan;
static uint32_t dma_buffer[LED_MATRIX_COUNT * 3];  // Buffer intermediário para DMA

volatile uint8_t brightness = LED_MATRIX_BRILHO;

// Mapeamento de brightness (0 a 8) para brightness_level
const uint16_t brightness_mapping[] = {
    0,   // 0: Brilho 0% (desligado)
    128, // 1: Brilho 10%
    64,  // 2: Brilho 25%
    32,  // 3: Brilho 40%
    16,  // 4: Brilho 50%
    8,   // 5: Brilho 60%
    4,   // 6: Brilho 75%
    2,   // 7: Brilho 90%
    256  // 8: Brilho 100% (máximo)
};

// Inicializa a matriz de LEDs utilizando a máquina PIO e DMA.
void led_matrix_init()
{
    // Cria programa PIO.
    uint offset = pio_add_program(pio0, &ws2818b_program);
    led_matrix_pio = pio0;

    // Toma posse de uma máquina PIO.
    pio_state_machine = pio_claim_unused_sm(led_matrix_pio, false);
    if (pio_state_machine < 0)
    {
        led_matrix_pio = pio1;
        pio_state_machine = pio_claim_unused_sm(led_matrix_pio, true); 
    }

    // Inicia programa na máquina PIO obtida.
    ws2818b_program_init(led_matrix_pio, pio_state_machine, offset, LED_MATRIX_PIN, 800000.f);

    // Limpa buffer de pixels.
    for (uint i = 0; i < LED_MATRIX_COUNT; ++i)
    {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }

     // Configura DMA
     dma_chan = dma_claim_unused_channel(true);
     dma_channel_config c = dma_channel_get_default_config(dma_chan);
     
     // Configuração DMA
     channel_config_set_transfer_data_size(&c, DMA_SIZE_32); 
     channel_config_set_read_increment(&c, true);
     channel_config_set_write_increment(&c, false);
     channel_config_set_dreq(&c, pio_get_dreq(led_matrix_pio, pio_state_machine, true));
     
     dma_channel_configure(
         dma_chan,
         &c,
         &led_matrix_pio->txf[pio_state_machine], // destino
         leds,                                     // fonte
         LED_MATRIX_COUNT * sizeof(pixel_t),       // tamanho 
         false                                     // não iniciar imediatamente
     );
}

// Define a cor de um LED
void led_matrix_set_LED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b)
{
    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
}

// Limpa todos os LEDs
void led_matrix_clear()
{
    for (uint i = 0; i < LED_MATRIX_COUNT; ++i)
    led_matrix_set_LED(i, 0, 0, 0);
}

// Escreve os dados na matriz de LEDs
void led_matrix_write()
{
    // Converte dados do formato de pixel para o formato do DMA
    for(int i = 0; i < LED_MATRIX_COUNT; i++) {
        dma_buffer[i*3] = leds[i].G;
        dma_buffer[i*3 + 1] = leds[i].R;
        dma_buffer[i*3 + 2] = leds[i].B;
    }
    
    // Inicia transferência com buffer intermediário
    dma_channel_set_read_addr(dma_chan, dma_buffer, false);
    dma_channel_set_trans_count(dma_chan, LED_MATRIX_COUNT * 3, true);
    
    dma_channel_wait_for_finish_blocking(dma_chan);
    sleep_us(10);
}

// Função para converter a posição do matriz para uma posição do vetor
int led_matrix_get_index(int x, int y)
{
    // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
    // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
    if (y % 2 == 0)
    {
        return 24 - (y * 5 + x); // Linha par (esquerda para direita).
    }
    else
    {
        return 24 - (y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
    }
}

// Função para aplicar o brilho em um pixel
void led_matrix_apply_brightness(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t brightness)
{
    // Verifica se o brightness está dentro do intervalo válido (0-8)
    if (brightness > 8)
        brightness = 8;

    // Obtém o brightness_level da tabela
    uint16_t brightness_level = brightness_mapping[brightness];

    // Converte brightness_level para 8 bits (tratando 256 como 255)
    if (brightness_level > 255)
        brightness_level = 255;

    // Aplica o brilho
    *r = (uint8_t)((*r * brightness_level) / 255);
    *g = (uint8_t)((*g * brightness_level) / 255);
    *b = (uint8_t)((*b * brightness_level) / 255);
}

// Função para aumentar o brilho
void led_matrix_increase_brightness()
{
    if (brightness < 8) {
        brightness++;
        sleep_ms(100);
        // Atualiza todos os LEDs com o novo brilho
        for (uint i = 0; i < LED_MATRIX_COUNT; ++i) {
            uint8_t r = leds[i].R;
            uint8_t g = leds[i].G;
            uint8_t b = leds[i].B;
            led_matrix_apply_brightness(&r, &g, &b, brightness);
            leds[i].R = r;
            leds[i].G = g;
            leds[i].B = b;
        }
        led_matrix_write();
    }
}

// Função para diminuir o brilho
void led_matrix_decrease_brightness()
{
    if (brightness > 1) {
        brightness--;
        sleep_ms(100);
        // Atualiza todos os LEDs com o novo brilho
        for (uint i = 0; i < LED_MATRIX_COUNT; ++i) {
            uint8_t r = leds[i].R;
            uint8_t g = leds[i].G;
            uint8_t b = leds[i].B;
            led_matrix_apply_brightness(&r, &g, &b, brightness);
            leds[i].R = r;
            leds[i].G = g;
            leds[i].B = b;
        }
        led_matrix_write(); 
    }
}

// Função para definir o pixel na posição (x, y) com a cor (r, g, b)
void led_matrix_set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    int index = led_matrix_get_index(x, y);
    led_matrix_apply_brightness(&r, &g, &b, brightness);
    led_matrix_set_LED(index, r, g, b);
}