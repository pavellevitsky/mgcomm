#!/bin/sh

g++ -std=c++11 -pthread -I../include ut-TcpServer.cpp TcpServer.cpp -o ut-TcpServer
./ut-TcpServer
rm -f ./ut-TcpServer

