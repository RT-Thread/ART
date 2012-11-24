#! /usr/bin/env python
#coding=utf-8

import sys
import time 
import struct
import os

from socket import socket
from socket import AF_INET
from socket import SOCK_STREAM

PORT = 2000

def daemon_client(filename, host):
    addr = (host,PORT)
    TCPSock = socket(AF_INET, SOCK_STREAM)
    
    try:
        TCPSock.connect(addr)
    except:
        print 'connect ART daemon server: %s failed.' % host
        sys.exit(1)
    
    total_length = 0
    f = file(filename, 'rb')
    if f:
        f.seek(0, os.SEEK_END)
        length = f.tell()
        f.seek(0, os.SEEK_SET)
        
        request = struct.pack('II64s', 0, length, os.path.basename(filename))
        TCPSock.send(request)
        
        print 'Push file to device',
        while True:
            data = f.read(1024)
            if not data:
                break
            TCPSock.send(data)
            print '.',
        print 'Done.'

    TCPSock.close()
    
if __name__ == '__main__':
    if len(sys.argv) != 3:
        print 'Usage:%s file host' % sys.argv[0]
        sys.exit(1)

    fn = sys.argv[1]
    host = sys.argv[2]
    
    daemon_client(fn, host)
    sys.exit(0)
