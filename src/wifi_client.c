#include <stdio.h>
#include <string.h>
#include "pico/cyw43_arch.h"
#include "lwip/ip_addr.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "wifi_config.h"

void send_score_to_server(int score) {
    struct udp_pcb *pcb;
    struct pbuf *p;
    ip_addr_t dest_ip;
    char msg[64];
    int len;
    err_t err;
    int link_status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);

    if (link_status != CYW43_LINK_UP) {
        printf("Wi-Fi sem link (status=%d), score nao enviado\n", link_status);
        return;
    }

    if (!ipaddr_aton(SERVER_IP, &dest_ip)) {
        printf("IP do servidor invalido\n");
        return;
    }

    pcb = udp_new();
    if (!pcb) {
        printf("Falha ao criar UDP PCB\n");
        return;
    }

    len = snprintf(msg, sizeof(msg), "score=%d", score);
    if (len <= 0 || len >= (int)sizeof(msg)) {
        printf("Erro ao montar payload\n");
        udp_remove(pcb);
        return;
    }

    p = pbuf_alloc(PBUF_TRANSPORT, (u16_t)len, PBUF_RAM);
    if (!p) {
        printf("Falha ao alocar pbuf\n");
        udp_remove(pcb);
        return;
    }

    memcpy(p->payload, msg, (size_t)len);
    err = udp_sendto(pcb, p, &dest_ip, SERVER_PORT);

    pbuf_free(p);
    udp_remove(pcb);

    if (err == ERR_OK) {
        printf("Score enviado via UDP: %s\n", msg);
    } else {
        printf("Falha ao enviar score (err=%d)\n", (int)err);
    }
}
