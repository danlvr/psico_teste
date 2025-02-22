#include "modules/wifi_rtc.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "common.h"
#include "hardware/rtc.h"
#include "lwip/dns.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"

// Estrutura para armazenar o estado do NTP
typedef struct NTP_T_
{
    ip_addr_t ntp_server_address;
    bool dns_request_sent;
    struct udp_pcb *ntp_pcb;
    absolute_time_t ntp_test_time;
    alarm_id_t ntp_resend_alarm;
} NTP_T;

// Converte para string o horário atual do RTC
char* get_timestamp() {
    static char timestamp[20]; // Buffer para armazenar "DD/MM/YYYY HH:MM:SS"
    datetime_t t;
    
    rtc_get_datetime(&t);
    
    snprintf(timestamp, sizeof(timestamp), 
             "%02d/%02d/%04d %02d:%02d:%02d",
             t.day, t.month, t.year,
             t.hour, t.min, t.sec);
             
    return timestamp;
}

// Função para setar o horário do RTC com o tempo ajustado
static void set_rtc_time_from_ntp(struct tm *time_info)
{
    datetime_t t = {
        .year = time_info->tm_year + 1900,
        .month = time_info->tm_mon + 1,
        .day = time_info->tm_mday,
        .dotw = time_info->tm_wday,
        .hour = time_info->tm_hour,
        .min = time_info->tm_min,
        .sec = time_info->tm_sec};

    rtc_init();
    sleep_ms(1000);
    rtc_set_datetime(&t);
    sleep_ms(1000);

    // Usa a função get_timestamp para exibir o tempo atual
    printf("Horário RTC da Pico W (GMT-3): %s\n", get_timestamp());
}

// Chamado quando a resposta NTP é recebida
static void resultado_ntp(NTP_T *state, int status, time_t *result)
{
    if (status == 0 && result)
    {
        struct tm *utc = gmtime(result);
        printf("Resposta NTP (GMT): %02d/%02d/%04d %02d:%02d:%02d\n", utc->tm_mday, utc->tm_mon + 1, utc->tm_year + 1900,
               utc->tm_hour, utc->tm_min, utc->tm_sec);

        // Converte o tempo UTC para o horário local (GMT-3)
        utc->tm_hour -= 3;
        if (utc->tm_hour < 0)
        {
            utc->tm_hour += 24;
            utc->tm_mday--;
        }

        // Configura o horário do RTC
        set_rtc_time_from_ntp(utc);
    }

    if (state->ntp_resend_alarm > 0)
    {
        cancel_alarm(state->ntp_resend_alarm);
        state->ntp_resend_alarm = 0;
    }
    state->ntp_test_time = make_timeout_time_ms(NTP_TEST_TIME);
    state->dns_request_sent = false;
}

static int64_t handler_ntp_falho(alarm_id_t id, void *user_data);

// Faz uma requisição NTP
static void request_ntp(NTP_T *state)
{
    cyw43_arch_lwip_begin();
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, NTP_MSG_LEN, PBUF_RAM);
    uint8_t *req = (uint8_t *)p->payload;
    memset(req, 0, NTP_MSG_LEN);
    req[0] = 0x1b;
    udp_sendto(state->ntp_pcb, p, &state->ntp_server_address, NTP_PORT);
    pbuf_free(p);
    cyw43_arch_lwip_end();
}

// Handler para o alarme de falha na requisição NTP
static int64_t handler_ntp_falho(alarm_id_t id, void *user_data)
{
    NTP_T *state = (NTP_T *)user_data;
    printf("Requisição NTP falhou\n");
    resultado_ntp(state, -1, NULL);
    return 0;
}

// Callback para a requisição DNS do NTP
static void dns_ntp_encontrado(const char *hostname, const ip_addr_t *ipaddr, void *arg)
{
    NTP_T *state = (NTP_T *)arg;
    if (ipaddr)
    {
        state->ntp_server_address = *ipaddr;
        printf("Endereço NTP %s\n", ipaddr_ntoa(ipaddr));
        request_ntp(state);
    }
    else
    {
        printf("Requisição DNS do NTP falhou\n");
        resultado_ntp(state, -1, NULL);
    }
}

// Dados do servidor NTP recebidos
static void ntp_recebido(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    NTP_T *state = (NTP_T *)arg;
    uint8_t mode = pbuf_get_at(p, 0) & 0x7;
    uint8_t stratum = pbuf_get_at(p, 1);

    // Checa se a resposta é válida
    if (ip_addr_cmp(addr, &state->ntp_server_address) && port == NTP_PORT && p->tot_len == NTP_MSG_LEN &&
        mode == 0x4 && stratum != 0)
    {
        uint8_t seconds_buf[4] = {0};
        pbuf_copy_partial(p, seconds_buf, sizeof(seconds_buf), 40);
        uint32_t seconds_since_1900 = seconds_buf[0] << 24 | seconds_buf[1] << 16 | seconds_buf[2] << 8 | seconds_buf[3];
        uint32_t seconds_since_1970 = seconds_since_1900 - NTP_DELTA;
        time_t epoch = seconds_since_1970;
        resultado_ntp(state, 0, &epoch);
    }
    else
    {
        printf("Respota NTP inválida\n");
        resultado_ntp(state, -1, NULL);
    }
    pbuf_free(p);
}

// Inicializa o estado NTP
static NTP_T *ntp_init(void)
{
    NTP_T *state = (NTP_T *)calloc(1, sizeof(NTP_T));
    if (!state)
    {
        printf("Falha em alocar estado\n");
        return NULL;
    }
    state->ntp_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
    if (!state->ntp_pcb)
    {
        printf("Falha ao criar pcb\n");
        free(state);
        return NULL;
    }
    udp_recv(state->ntp_pcb, ntp_recebido, state);
    return state;
}

// Obtém o tempo NTP
void get_time_ntp(void)
{
    NTP_T *state = ntp_init();
    if (!state)
        return;

    if (absolute_time_diff_us(get_absolute_time(), state->ntp_test_time) < 0 && !state->dns_request_sent)
    {
        // Seta o alarme para reenviar a requisição NTP
        state->ntp_resend_alarm = add_alarm_in_ms(NTP_RESEND_TIME, handler_ntp_falho, state, true);

        cyw43_arch_lwip_begin();
        int err = dns_gethostbyname(NTP_SERVER, &state->ntp_server_address, dns_ntp_encontrado, state);
        cyw43_arch_lwip_end();

        state->dns_request_sent = true;
        if (err == ERR_OK)
        {
            request_ntp(state); // Resultado cacheado
        }
        else if (err != ERR_INPROGRESS)
        { // ERR_INPROGRESS significa que a requisição DNS está em andamento
            printf("Requisição DNS falhou\n");
            resultado_ntp(state, -1, NULL);
        }
    }

    free(state);
}

char *return_ip_address()
{
    static char ip_str[16]; // Buffer estático para armazenar o endereço IP
    uint8_t *ip_address = (uint8_t *)&(cyw43_state.netif[0].ip_addr.addr);
    snprintf(ip_str, sizeof(ip_str), "%d.%d.%d.%d",
             ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    return ip_str;
}

void wifi_init(uintptr_t)
{
    // Inicializa o chip Wi-Fi
    if (cyw43_arch_init())
    {
        printf("Inicialização do Wi-Fi falhou\n");
        return;
    }

    // Ativa o modo estação
    cyw43_arch_enable_sta_mode();

    printf("Conectando ao Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("Falha ao conectar.\n");
        return;
    }
    else
    {
        printf("Conectado.\n");
        printf("Endereço IP %s\n", return_ip_address());
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    }
    get_time_ntp();
}