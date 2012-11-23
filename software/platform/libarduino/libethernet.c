#include <rtthread.h>

#ifdef RT_USING_LWIP
#include "libethernet.h"
#include <inttypes.h>
#include <lwip/ip_addr.h>
#include <lwip/netif.h>

#include <lwip/dns.h>
#include <lwip/netdb.h>
#include <lwip/dhcp.h>

#define ADDR_ANY	0

int Ethernet_begin(uint32_t local_ip, uint32_t dns_server, uint32_t gateway, uint32_t subnet)
{
	struct ip_addr *ip;
	struct ip_addr addr;

	if (netif_default != RT_NULL)
	{
		/* set IP address */
		struct netif * netif = netif_default;
		
		ip = (struct ip_addr *)&addr;
		ip->addr = local_ip;

		/* set ip address */
		netif_set_ipaddr(netif, ip);
		
		/* set gateway address */
		if (gateway == ADDR_ANY)
		{
			gateway = (local_ip & ~0xff) | 0x01;
		}
		ip->addr = gateway;
		netif_set_gw(netif, ip);
		
		/* set netmask address */
		if (subnet == ADDR_ANY)
		{
			subnet = 0xffffff00;
		}
		ip->addr = subnet;
		netif_set_netmask(netif, ip);
	}
	
	if (dns_server != ADDR_ANY)
	{
		ip = (struct ip_addr *)&addr;
		ip->addr = dns_server;

		/* always set DNS server 1 */
		dns_setserver(1, ip);
	}
	
	return 0;
}
RTM_EXPORT(Ethernet_begin);

int Ethernet_beginWithDHCP(uint8_t *mac, unsigned long timeout, unsigned long responseTimeout)
{
	int ret = -1;

	if (netif_default != RT_NULL)
	{
		ret = dhcp_start(netif_default);
		if (ret == ERR_OK) ret = 1;
	}

	return ret;
}
RTM_EXPORT(Ethernet_beginWithDHCP);

uint32_t Ethernet_localIP(void)
{
	uint32_t ipaddr = 0;
	
	if (netif_default != RT_NULL)
	{
		ipaddr = netif_default->ip_addr.addr;
	}

	return ipaddr;
}
RTM_EXPORT(Ethernet_localIP);

uint32_t Ethernet_subnetMask(void)
{
	uint32_t ipaddr = 0;
	
	if (netif_default != RT_NULL)
	{
		ipaddr = netif_default->netmask.addr;
	}

	return ipaddr;
}
RTM_EXPORT(Ethernet_subnetMask);

uint32_t Ethernet_gatewayIP(void)
{
	uint32_t ipaddr = 0;
	
	if (netif_default != RT_NULL)
	{
		ipaddr = netif_default->gw.addr;
	}

	return ipaddr;
}
RTM_EXPORT(Ethernet_gatewayIP);

uint32_t Ethernet_DHCPServer(void)
{
	/* always return gateway IP */
	return Ethernet_gatewayIP();
}
RTM_EXPORT(Ethernet_DHCPServer);

uint32_t Ethernet_DNSServer(void)
{
	uint32_t ret;
	struct ip_addr addr;

	addr = dns_getserver(1);
	ret = addr.addr;
	return ret;
}
RTM_EXPORT(Ethernet_DNSServer);

const char* Ethernet_macAddress(void)
{
	if (netif_default != RT_NULL)
	{
		return (char*)netif_default->hwaddr;
	}

	return RT_NULL;
}
RTM_EXPORT(Ethernet_macAddress);

void Ethernet_setLocalIP(uint32_t localIP)
{
	struct ip_addr *ip;
	struct ip_addr addr;

	if (netif_default != RT_NULL)
	{
		/* set IP address */
		struct netif * netif = netif_default;
		
		ip = (struct ip_addr *)&addr;
		ip->addr = localIP;

		/* set ip address */
		netif_set_ipaddr(netif, ip);
	}
}
RTM_EXPORT(Ethernet_setLocalIP);

void Ethernet_setSubnetMask(uint32_t subnetMask)
{
	struct ip_addr *ip;
	struct ip_addr addr;

	if (netif_default != RT_NULL)
	{
		/* set IP address */
		struct netif * netif = netif_default;
		
		ip = (struct ip_addr *)&addr;
		ip->addr = subnetMask;

		/* set netmask address */
		netif_set_netmask(netif, ip);
	}
}
RTM_EXPORT(Ethernet_setSubnetMask);

void Ethernet_setGatewayIP(uint32_t gatewayIP)
{
	struct ip_addr *ip;
	struct ip_addr addr;

	if (netif_default != RT_NULL)
	{
		/* set IP address */
		struct netif * netif = netif_default;
		
		ip = (struct ip_addr *)&addr;
		ip->addr = gatewayIP;

		/* set gateway address */
		netif_set_gw(netif, ip);
	}
}
RTM_EXPORT(Ethernet_setGatewayIP);

void Ethernet_setDNSServer(uint32_t dns_server)
{
	struct ip_addr *ip;
	struct ip_addr addr;

	if (netif_default != RT_NULL)
	{
		/* set IP address */
		ip = (struct ip_addr *)&addr;
		ip->addr = dns_server;

		/* always set DNS server 1 */
		dns_setserver(1, ip);
	}
}
RTM_EXPORT(Ethernet_setDNSServer);

uint32_t DNSClient_getHostByName(const char* hostname)
{
	uint32_t ret;
    struct hostent *host;

    host = lwip_gethostbyname(hostname);

	ret = ((struct in_addr *)host->h_addr)->s_addr;

    return ret;
}
RTM_EXPORT(DNSClient_getHostByName);

#endif

