#include <string.h>
#include <stdlib.h>

#include "Dhcp.h"
#include "Arduino.h"
#include "socket/libethernet.h"

IPAddress DhcpClass::getLocalIp()
{
    return IPAddress(Ethernet_localIP());
}

IPAddress DhcpClass::getSubnetMask()
{
    return IPAddress(Ethernet_subnetMask());
}

IPAddress DhcpClass::getGatewayIp()
{
    return IPAddress(Ethernet_gatewayIP());
}

IPAddress DhcpClass::getDhcpServerIp()
{
    return IPAddress(Ethernet_DHCPServer());
}

IPAddress DhcpClass::getDnsServerIp()
{
    return IPAddress(Ethernet_DNSServer());
}

void DhcpClass::printByte(char * buf, uint8_t n ) {
  char *str = &buf[1];
  buf[0]='0';
  do {
    unsigned long m = n;
    n /= 16;
    char c = m - 16 * n;
    *str-- = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);
}

int DhcpClass::beginWithDHCP(uint8_t *mac, unsigned long timeout, unsigned long responseTimeout)
{
    return Ethernet_beginWithDHCP(mac, timeout, responseTimeout);
}
