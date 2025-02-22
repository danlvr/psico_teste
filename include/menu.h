#ifndef MENU_H
#define MENU_H

/**
 * @brief Renderiza o menu 
 */
void render_menu(const char **items, int num_items, int selected);

/**
 * @brief Executa as opções do menu
 */
void handle_menu_selection(int option);

/**
 * @brief Update nos LEDs no menu de Configurações
 */
void refresh_led_pattern(void);

/**
 * @brief Menu de configurações
 */
void handle_config_menu(void);

/**
 * @brief Chama o menu principal
 */
void display_menu(void);

#endif // MENU_H