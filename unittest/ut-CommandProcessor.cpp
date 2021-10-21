#include <iostream>
#include "CommandProcessor.hh"

int main()
{
    std::cout << "[UT] CommandProcessor class";

    CommandProcessor processor;

    processor.process_command((const char*)"hello-world 4 5 6 0x66");
    processor.process_command((const char*)"set_log_codes 1 2 3 0xab");
    processor.process_command((const char*)"stop_logging 7 8 9 0x77");

    return 0;
}

