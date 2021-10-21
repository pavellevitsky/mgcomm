/*! Qualcomm scanner service
 * */

#include "CommandProcessor.hh"
#include "TcpServer.hh"
#include "Parser.hh"

// Compilation for unit testing support
#ifdef UNITTEST
#include "DCIBase.hh"
#else
#include "DCI.hh"
#endif

#include <iostream>
#include <functional>
#include <unistd.h>
#include <csignal>

// SIGINT handler (Ctrl-C or so)
bool exit_flag = false;
void catch_sigint (int sig)
{
    (void) sig;

    std::cout << "\nSIGINT handled\n";
    exit_flag = true;
}

int main(int argc, char **argv)
{
    std::cout << "Qualcomm Scanner\n";

    // Setup a Ctrl-C handler
    signal (SIGINT, catch_sigint);

    // Setup DCI interface

#ifdef UNITTEST
    DCIBase dci;    // Software simulation on a PC
#else
    DCI dci;
#endif

    dci.setup_interface();

    // Create a TCP server for reports

    // Create a TCP-server for incoming commands communication
    TcpServer comm_srv;

    // Create a command processor and connect it to communication server
    CommandProcessor processor;

    // User communication commands callback
    receive_callback_t cb_cmd = std::bind(
            &CommandProcessor::process_command, std::ref(processor),
            std::placeholders::_1);

    comm_srv.set_callback(cb_cmd);
    
    // Create a modem Parser and connect it to DCI
    Parser parser;

    using std::placeholders::_1;
    using std::placeholders::_2;

    log_callback_t cb_log = std::bind(&Parser::process_log, std::ref(parser), _1, _2);

    // Connect Logs handler of DCI interface
    dci.add_log_handler(cb_log);

    /* TODO implement
    // Create and connect Events handler of DCI interface
    event_callback_t cb_event = std::bind(
            &Parser::process_event, std::ref(parser), _1, _2);
    dci.add_event_handler(cb_event);
    */

    // Connect to AT Command interface

    while(!exit_flag)
    {
        sleep(3);
        std::cout << "\n\t\t\tTick";
    }

    std::cout << "\nQScan is done\n";

    return 0;
}
