#!/bin/sh

g++ -std=c++11 -I../include unittest-TcpServer.cpp TcpServer.cpp -o unittest-TcpServer && ./unittest-TcpServer
rm -f ./unittest-TcpServer

