#include "menu.h"
#include "common.h"
#include "testes_psi.h"

#define MENU_DELAY 25

typedef enum {
    MENU_MAIN,
    MENU_CONFIG
} MenuState;

bool should_render_menu_text = true;

static int last_selected = -1;

static const char *main_menu_items[] = {
    "Cores",
    "Obstaculos",
    "Previsao",
    "Logica",
    "Configuracoes"};

static const char *config_menu_items[] = {
    "+ Brilho Matrix LED",
    "- Brilho Matrix LED",
    "+ Brilho LED RGB",
    "- Brilho LED RGB",
    "Voltar"};

#define MAIN_MENU_ITEMS (sizeof(main_menu_items) / sizeof(char *))
#define CONFIG_MENU_ITEMS (sizeof(config_menu_items) / sizeof(char *))

// Função para renderizar o menu
void render_menu(const char **items, int num_items, int selected)
{       
    // Renderiza todos os itens do menu apenas na primeira vez
    if (should_render_menu_text)
    {
        display_clear();
        for (int i = 0; i < num_items; i++)
        {
            display_text_at_location(items[i], 1, 15, 2 + i * 12);
        }
        should_render_menu_text = false;
    }

    // Atualiza apenas a posição da seta se a seleção mudou
    if (selected != last_selected)
    {
        // Limpa a coluna da seta antiga (pixels de 2 a 10 na horizontal)
        if (last_selected >= 0)
        {
            for (int x = 2; x < 10; x++)
            {
                for (int y = 2 + last_selected * 12; y < (2 + last_selected * 12) + 8; y++)
                {
                    display_clear_pixel(x, y);
                }
            }
        }
        
        // Desenha a nova seta usando pixels
        int base_y = 2 + selected * 12;
        // Desenha ">" usando pixels
        for (int y = base_y; y < base_y + 8; y++)
        {
            display_draw_pixel(4, y);
        }
        display_draw_pixel(5, base_y + 1);
        display_draw_pixel(6, base_y + 2);
        display_draw_pixel(7, base_y + 3);
        display_draw_pixel(6, base_y + 4);
        display_draw_pixel(5, base_y + 5);    
        
        last_selected = selected;
        display_show();
    }
}

// Função para lidar com a seleção do menu
void handle_menu_selection(int option)
{
    display_clear();
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%s", main_menu_items[option]);
    display_text_at_location("Iniciando", 2, 2, 2);
    display_text_at_location("Teste", 2, 2, 20);
    display_text_at_location(buffer, 2, 2, 38);
    sleep_ms(2000);
    last_selected = -1;

    if (option == 0)
    {
        // Cores
        iniciar_teste_psicotecnico(0);
    }
    else if (option == 1)
    {
        // Obstáculos
        iniciar_teste_psicotecnico(1);
    }
    else if (option == 2)
    {
        // Atencipacao
        iniciar_teste_psicotecnico(2);
    }
    else if (option == 3)
    {
        // Logica Memoria
        iniciar_teste_psicotecnico(3);
    }

}

// Função para atualizar o padrão de LEDs durante o menu de configurações
void refresh_led_pattern()
{
    led_matrix_clear();
    sleep_ms(50);
    for (int x = 1; x <= 3; x++)
    {
        for (int y = 1; y <= 3; y++)
        {
            led_matrix_set_pixel(x, y, 255, 255, 255);
        }
    }
    led_matrix_write();
    rgb_led_set_color(255, 255, 255);
};

// Função para lidar com o menu de configurações
void handle_config_menu(void)
{
    int selected = 0;
    uint16_t vrx_value, vry_value;
    last_selected = -1;

    refresh_led_pattern();

    while (1)
    {
        render_menu(config_menu_items, CONFIG_MENU_ITEMS, selected);
        sleep_ms(MENU_DELAY);

        joystick_read_axis(&vrx_value, &vry_value);

        if (joystick_is_up_down(vry_value) == 1 && selected > 0)
        {
            selected--;
            buzzers_play_tone_both(1000, 100);

        }
        else if (joystick_is_up_down(vry_value) == -1 && selected < CONFIG_MENU_ITEMS - 1)
        {
            selected++;
            buzzers_play_tone_both(1000, 100);
        }

        if (botao_b_pressionado())
        {
            
            should_render_menu_text = true;

            if (selected == 0)
            {
                display_clear();
                display_text_at_location("Aumentando", 1, 2, 2);
                display_text_at_location("Brilho Matriz", 1, 2, 14);
                led_matrix_increase_brightness();
                refresh_led_pattern();
                sleep_ms(1000);
                last_selected = -1;
            }
            else if (selected == 1)
            {
                display_clear();
                display_text_at_location("Diminuindo", 1, 2, 2);
                display_text_at_location("Brilho Matriz", 1, 2, 14);
                led_matrix_decrease_brightness();
                refresh_led_pattern();
                sleep_ms(1000);
                last_selected = -1;
            }
            else if (selected == 2)
            {
                display_clear();
                display_text_at_location("Aumentando", 1, 2, 2);
                display_text_at_location("Brilho LED RGB", 1, 2, 14);
                rgb_led_increase_brightness();
                refresh_led_pattern();
                sleep_ms(1000);
                last_selected = -1;
            }
            else if (selected == 3)
            {
                display_clear();
                display_text_at_location("Diminuindo", 1, 2, 2);
                display_text_at_location("Brilho LED RGB", 1, 2, 14);
                rgb_led_decrease_brightness();
                refresh_led_pattern();
                sleep_ms(1000);
                last_selected = -1;
            }
            else if (selected == 4)
            {
                led_matrix_clear();
                led_matrix_write();
                rgb_led_set_color(0, 0, 0);
                last_selected = -1;
                return; // Volta ao menu principal
            }
        }
    }
}

// Função para exibir o menu
void display_menu(void)
{
    int selected = 0;
    uint16_t vrx_value, vry_value;
    MenuState current_menu = MENU_MAIN;
    last_selected = -1;

    while (1)
    {
        render_menu(main_menu_items, MAIN_MENU_ITEMS, selected);
        sleep_ms(MENU_DELAY);

        joystick_read_axis(&vrx_value, &vry_value);

        if (joystick_is_up_down(vry_value) == 1 && selected > 0)
        {
            selected--;
            buzzers_play_tone_both(1000, 100);
        }
        else if (joystick_is_up_down(vry_value) == -1 && selected < MAIN_MENU_ITEMS - 1)
        {
            selected++;
            buzzers_play_tone_both(1000, 100);
        }

        if (botao_b_pressionado())
        {
            should_render_menu_text = true;

            if (selected == 4)
            { // Configurações
                handle_config_menu();                
            }
            else
            {
                handle_menu_selection(selected);
            }
        }
    }
}