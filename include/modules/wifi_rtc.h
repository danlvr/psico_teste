#ifndef WIFI_RTC_H
#define WIFI_RTC_H

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

/**
 * @brief Converte o tempo do RTC para uma string formatada
 */
char* get_timestamp(void);

/**
 * @brief Seta o tempo do RTC a partir do NTP
 */
static void set_rtc_time_from_ntp(struct tm *time_info);

/**
 * @brief Handler para falha na requisição NTP
 */
static int64_t handler_ntp_falho(alarm_id_t id, void *user_data);

/**
 * @brief Conversão do DNS para IP
 */
static void dns_ntp_encontrado(const char *hostname, const ip_addr_t *ipaddr, void *arg);

/**
 * @brief Handler quando o NTP é recebido
 */
static void ntp_recebido(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

/**
 * @brief Requisição NTP
 */
void get_time_ntp(void);

/**
 * @brief Converte o endereço IP para uma string
 */
char *return_ip_address();

/**
 * @brief Initializa o módulo Wi-Fi e o atualiza o horário do RTC
 */
void wifi_init(uintptr_t);

#endif // WIFI_RTC_H