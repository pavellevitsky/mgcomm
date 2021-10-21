#include <iostream>
#include <functional>
#include <unistd.h>
#include "TcpServer.hh"
#include "CommandProcessor.hh"

int main()
{
    std::cout << "Unit test for CommandProcessor class";

    CommandProcessor processor;

    receive_callback_t callback = std::bind(
            &CommandProcessor::process_command, std::ref(processor),
            std::placeholders::_1);

    TcpServer server;

    server.set_callback(callback);

    for(int cnt=0;;sleep(2), ++cnt)
        std::cout << "\n" << cnt << '\n';

    return 0;
}

