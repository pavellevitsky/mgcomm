#include <iostream>
#include <unistd.h>
#include "TcpServer.hh"

int main(int argc, char **argv)
{
    std::cout << "TcpServer Test\n";

    TcpServer server;

    receive_callback_t callback = [](const std::string &s){
        std::cout << "\nCallback got that: " << s; };

    std::cout << "\nSending greeing";
    server.writeline("Hello\n");

    int cnt = 0;
    int n_rcv;
    for(;;) {
        //n_rcv = server.receive();
        //std::cout << "\nreceived bytes: " << n_rcv << "\n";
        //if(n_rcv <= 0)
        //    break;
        server.writeline(std::to_string(cnt++));
        sleep(1);

        if(cnt == 5) {
            std::cout << "\nSetting callback...";
            server.set_callback(callback);
        }
    }

    return 0;
}
