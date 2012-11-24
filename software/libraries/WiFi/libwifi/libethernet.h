#ifndef __LIB_ETHERNET_H__
#define __LIB_ETHERNET_H__

#include <rtthread.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef RT_USING_LWIP

int Ethernet_begin(uint32_t local_ip, uint32_t dns_server, uint32_t gateway, uint32_t subnet);
int Ethernet_beginWithDHCP(uint8_t *mac, unsigned long timeout, unsigned long responseTimeout);

uint32_t Ethernet_localIP(void);
uint32_t Ethernet_subnetMask(void);
uint32_t Ethernet_gatewayIP(void);

uint32_t Ethernet_DHCPServer(void);
uint32_t Ethernet_DNSServer(void);
const char* Ethernet_macAddress(void);

void Ethernet_setLocalIP(uint32_t localIP);
void Ethernet_setSubnetMask(uint32_t subnetMask);
void Ethernet_setGatewayIP(uint32_t gatewayIP);
void Ethernet_setDNSServer(uint32_t dns_server);
uint32_t DNSClient_getHostByName(const char* hostname);

#endif

#ifdef __cplusplus
}
#endif

#endif

