/* 
 * Ethernet Class Implement for Arduino
 */
#include "socket/libethernet.h"

#include "Ethernet.h"
#include "Dhcp.h"

int EthernetClass::begin(uint8_t *mac_address)
{
    /* always ignore MAC address */

    /* startup DHCP */
    return Ethernet_beginWithDHCP(mac_address, 0, 0);
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip)
{
    /* set local address */
    Ethernet_begin(local_ip, 0, 0, 0);
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server)
{
    Ethernet_begin(local_ip, dns_server, 0, 0);
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
    Ethernet_begin(local_ip, dns_server, gateway, 0);
}

void EthernetClass::begin(uint8_t *mac, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
    Ethernet_begin(local_ip, dns_server, gateway, subnet);
}

int EthernetClass::maintain(){
  return 1;
}

IPAddress EthernetClass::localIP()
{
  IPAddress ret = Ethernet_localIP();
  return ret;
}

IPAddress EthernetClass::subnetMask()
{
  IPAddress ret = Ethernet_subnetMask();
  return ret;
}

IPAddress EthernetClass::gatewayIP()
{
  IPAddress ret = Ethernet_gatewayIP();
  return ret;
}

IPAddress EthernetClass::dnsServerIP()
{
  IPAddress ret = Ethernet_DHCPServer();
  return ret;
}

EthernetClass Ethernet;
