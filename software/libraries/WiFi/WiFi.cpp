#include "WiFi.h"
#include "libwifi/libwifi.h"
#include "libwifi/libethernet.h"

#include <string.h>

WiFiClass::WiFiClass()
{
    /* initialize wlan sub-system */
    wlan_system_init();
}

char* WiFiClass::firmwareVersion()
{
    static char version[] = "88W8686 0xB";

    return version;
}

int WiFiClass::begin(char* ssid)
{
	uint8_t status = WL_IDLE_STATUS;
    
    if (wlan_begin(ssid, RT_NULL) == 0)
    {
        status = WL_CONNECTED;
    }

    return status;
}

int WiFiClass::begin(char* ssid, uint8_t key_idx, const char *key)
{
	uint8_t status = WL_IDLE_STATUS;
    
    /* Wlan sub-system will automatically use encryption mode. */
    if (wlan_begin(ssid, key) == 0)
    {
        status = WL_CONNECTED;
    }

    return status;
}

int WiFiClass::begin(char* ssid, const char *passphrase)
{
	uint8_t status = WL_IDLE_STATUS;
    
    if (wlan_begin(ssid, passphrase) == 0)
    {
        status = WL_CONNECTED;
    }

    return status;
}

int WiFiClass::disconnect()
{
    return 0;
}

uint8_t* WiFiClass::macAddress(uint8_t* mac)
{
	memcpy(mac, Ethernet_macAddress(), WL_MAC_ADDR_LENGTH);
    return mac;
}

IPAddress WiFiClass::localIP()
{
	IPAddress ret;

    ret = Ethernet_localIP();
	return ret;
}

IPAddress WiFiClass::subnetMask()
{
	IPAddress ret;
	ret = Ethernet_subnetMask();
	return ret;
}

IPAddress WiFiClass::gatewayIP()
{
	IPAddress ret;
	ret = Ethernet_gatewayIP();
	return ret;
}

char* WiFiClass::SSID()
{
    return wlan_SSID();
}

uint8_t* WiFiClass::BSSID(uint8_t* bssid)
{
	memcpy(bssid, wlan_BSSID(), WL_MAC_ADDR_LENGTH);
    return bssid;
}

int32_t WiFiClass::RSSI()
{
    return wlan_RSSI();
}

uint8_t WiFiClass::encryptionType()
{
    return wlan_encryptionType();
}

int8_t WiFiClass::scanNetworks()
{
    /* do not scan network at present */
	return 0;
}

char* WiFiClass::SSID(uint8_t networkItem)
{
	return wlan_SSID();
}

int32_t WiFiClass::RSSI(uint8_t networkItem)
{
	return wlan_RSSI();
}

uint8_t WiFiClass::encryptionType(uint8_t networkItem)
{
    return wlan_encryptionType();
}

uint8_t WiFiClass::status()
{
    return wlan_status();
}

int WiFiClass::hostByName(const char* aHostname, IPAddress& aResult)
{
    uint32_t ip_addr;
    ip_addr = DNSClient_getHostByName(aHostname);
    aResult = ip_addr;

    if (ip_addr == 0)
        return -1;

	return 1;
}

WiFiClass WiFi;
