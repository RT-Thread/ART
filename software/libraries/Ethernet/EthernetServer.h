/*
 * EthernetServer for Arduino
 */
#ifndef __ETHERNET_SERVER_H__
#define __ETHERNET_SERVER_H__

#include "Server.h"

class EthernetClient;

class EthernetServer : public Server 
{
private:
    int sock;
    uint16_t _port;
    void accept();

public:
  EthernetServer(uint16_t);

  EthernetClient available();
  virtual void begin();
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);

  using Print::write;
};

#endif
