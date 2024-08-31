//Networking Includes
#include "pico/cyw43_arch.h"
#include "lwip/altcp.h"
#include "lwip/dns.h"
#include "setupWifi.h"
#include "request.h"

// Custom Software Includes
#include "networkCommunication.h"
#include "appDefinitions.h"


void dns_found(const char *name, const ip_addr_t *ip, void *arg)
{
    ip_addr_t *ipResult = (ip_addr_t *)arg;
    if (ip)
    {
        ip4_addr_copy(*ipResult, *ip);
    }
    else
    {
        ip4_addr_set_loopback(ipResult);
    }
    return;
}

err_t getIP(char *URL, ip_addr_t *ipResult)
{
    cyw43_arch_lwip_begin();
    err_t err = dns_gethostbyname(URL, ipResult, dns_found, ipResult);
    cyw43_arch_lwip_end();
    return err;
}


int log_data_to_server(ip_addr_t ip, float data, char *output_buffer, int station)
{
    char buf[50];
    char hostname[] = "monitoring-backend.ccorso.ca";
    memset(buf, 0, sizeof(buf));
    memset(output_buffer, 0, 2048);
    snprintf(buf, sizeof(buf), "/record/%u/?data=%.2f", station, data);
    
    #ifdef DEBUG_MODE
        printf("/record/%u/?data=%.2f", station, data);
    #endif // DEBUG_MODE
    
    // struct connectionState *cs1 = doRequest(&ip, hostname, 2710, "POST", buf, NULL, myBuff1);
    struct connectionState *cs1 = doRequest(&ip, "monitoring-backend.ccorso.ca", 2710, "POST", buf, NULL, output_buffer);
    
    while (pollRequest(&cs1))
    {
        sleep_ms(100);
        //printf("."); // print a "." to indicate that the system is not stuck
    }
    return 0;
}

ip_addr_t set_ip()
{
    ip_addr_t ip;
    ip4_addr_set_zero(&ip);
    getIP("monitoring-backend.ccorso.ca", &ip);
    //getIP("jellyfin.ccorso.ca", &ip); // Use jellyfin as that will get the home IP address instead of the cloudflare one
    while (!ip_addr_get_ip4_u32(&ip))
    {
        sleep_ms(100);
    };
    if (ip4_addr_isloopback(&ip))
    {
        printf("address not found");
        return ip;
    }
    return ip;
}


void transmit_data(int station, float data, ip_addr_t ip)
{
    // Setup a buffer
    char myBuff1[BUF_SIZE];
    
    // Log the data to the server
    log_data_to_server(ip, data, myBuff1, station);

    // Print the results
    #ifdef DEBUG_MODE
        printf("Data: %f, station: %d. \nUpload results: %s", data, station, myBuff1);
    #endif // DEBUG_MODE
}