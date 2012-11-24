#include "Arduino.h"

#include "Ethernet.h"
#include "EthernetClient.h"
#include "socket/libsocket.h"

EthernetClient::EthernetClient() : _sock(-1) {
}

EthernetClient::EthernetClient(uint8_t sock) : _sock(sock) {
}

int EthernetClient::connect(const char* host, uint16_t port) {
    int result;
    if (_sock == -1)
    {
        /* open a TCP socket */
        _sock = socket_create(SOCK_STREAM);
        if (_sock < 0) return false;
    }

    result = socket_connect(_sock, host, port);
    if (result != 0)
    {
        /* close socket */
        socket_close(_sock);
        _sock = -1;
    }
    return result == 0? true:false;
}

int EthernetClient::connect(IPAddress ip, uint16_t port) {
    uint32_t ip_addr;

    if (_sock == -1)
    {
        /* open a TCP socket */
        _sock = socket_create(SOCK_STREAM);
        if (_sock < 0) return false;
    }

    ip_addr = ip;
    if (socket_connect_addr(_sock, ip_addr, port) == 0)
        return true;

    return false;
}

size_t EthernetClient::write(uint8_t b) {
  return socket_write(_sock, &b, 1);
}

size_t EthernetClient::write(const uint8_t *buf, size_t size) {
  return socket_write(_sock, buf, size);
}

int EthernetClient::available() {
  return 0;
}

int EthernetClient::read() {
  uint8_t b;
  socket_read(_sock, &b, 1);
}

int EthernetClient::read(uint8_t *buf, size_t size) {
  return socket_read(_sock, buf, size);
}

int EthernetClient::peek() {
  uint8_t b;
}

void EthernetClient::flush() {
}

void EthernetClient::stop() {
  if (_sock >= 0)
  {
    socket_close(_sock);
  }
  _sock = -1;
}

uint8_t EthernetClient::connected() {
  if (_sock == -1) return false;
  return true;
}

uint8_t EthernetClient::status() {
}

// the next function allows us to use the client returned by
// EthernetServer::available() as the condition in an if-statement.
EthernetClient::operator bool() {
  return _sock != -1;
}
