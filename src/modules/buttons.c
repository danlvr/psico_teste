#include "modules/buttons.h"
#include "common.h"

static volatile uint64_t last_press_time_a = 0;
static volatile uint64_t last_press_time_b = 0;
static volatile uint64_t last_press_time_joy = 0;
static volatile bool btn_a_state = false;
static volatile bool btn_b_state = false;
static volatile bool joy_btn_state = false;

// Handler de interrupção para todos os botões
void gpio_callback(uint gpio, uint32_t events)
{
    uint64_t current_time = to_ms_since_boot(get_absolute_time());

    // Verifica qual botão gerou a interrupção e aplica debounce
    if (gpio == BTN_A_PIN && (current_time - last_press_time_a) > BTN_DEBOUNCE_TIME_MS)
    {
        btn_a_state = true;
        last_press_time_a = current_time;
    }
    else if (gpio == BTN_B_PIN && (current_time - last_press_time_b) > BTN_DEBOUNCE_TIME_MS)
    {
        btn_b_state = true;
        last_press_time_b = current_time;
    }
    else if (gpio == JOYSTICK_BTN_PIN && (current_time - last_press_time_joy) > BTN_DEBOUNCE_TIME_MS)
    {
        joy_btn_state = true;
        last_press_time_joy = current_time;
    }
}

// Configuração dos botões como entrada e com pull-up
// no pino do botão para evitar flutuações
void buttons_init()
{
    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);

    gpio_init(BTN_B_PIN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);
    gpio_pull_up(BTN_B_PIN);

    gpio_init(JOYSTICK_BTN_PIN);
    gpio_set_dir(JOYSTICK_BTN_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BTN_PIN);

    gpio_set_irq_enabled_with_callback(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BTN_B_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(JOYSTICK_BTN_PIN, GPIO_IRQ_EDGE_FALL, true);
}

// Funções para checar se os botões foram pressionados

bool botao_a_pressionado(void)
{
    bool estado = btn_a_state;
    btn_a_state = false; // Reseta o estado
    return estado;
}

bool botao_b_pressionado(void)
{
    bool estado = btn_b_state;
    btn_b_state = false; // Reseta o estado
    return estado;
}

bool botao_joystick_pressionado(void)
{
    bool estado = joy_btn_state;
    joy_btn_state = false; // Reseta o estado
    return estado;
}