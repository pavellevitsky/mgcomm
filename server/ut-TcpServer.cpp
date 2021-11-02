#include <iostream>
#include <unistd.h>
#include "TcpServer.hh"

static void parser_function (const std::string &s);

int main(int argc, char **argv)
{
    std::cout << "TcpServer Test\n";

    TcpServer server;

//  receive_callback_t callback = [](const std::string &s) { std::cout << "\nCallback got string: " << s; };
    receive_callback_t callback = parser_function;

    sleep(5);

    std::cout << "\nSending Hello";
    server.writeline("Hello\n");

    sleep(5);

    std::cout << "\nSetting callback...";
    server.set_callback(callback);

    int cnt = 0;
    int n_rcv;

    for(;;) {
        server.writeline(std::to_string(cnt++));
        sleep(5);
    }

    return 0;
}

void parser_function (const std::string &s)
{
    std::cout << "\nCallback got string: " << s;
}
