/**
 * DHCP class for Arduino
 */
#ifndef __DHCP_H__
#define __DHCP_H__

#include <IPAddress.h>

class DhcpClass {
public:
    IPAddress getLocalIp();
    IPAddress getSubnetMask();
    IPAddress getGatewayIp();
    IPAddress getDhcpServerIp();
    IPAddress getDnsServerIp();

    int beginWithDHCP(uint8_t *, unsigned long timeout = 60000, unsigned long responseTimeout = 4000);
    void printByte(char * buf, uint8_t n);
};

#endif
