/*
 * EthernetClient Class for Arduino
 */
#ifndef __ETHERNET_CLIENT_H__
#define __ETHERNET_CLIENT_H__

#include "Arduino.h"	
#include "Print.h"
#include "Client.h"
#include "IPAddress.h"

class EthernetClient : public Client
{
public:
    EthernetClient();
    EthernetClient(uint8_t sock);

    uint8_t status();
    virtual int connect(IPAddress ip, uint16_t port);
    virtual int connect(const char *host, uint16_t port);

    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual int available();
    virtual int read();
    virtual int read(uint8_t *buf, size_t size);
    virtual int peek();
    virtual void flush();
    virtual void stop();
    virtual uint8_t connected();
    virtual operator bool();

    friend class EthernetServer;

    using Print::write;

private:
    int _sock;
};

#endif
