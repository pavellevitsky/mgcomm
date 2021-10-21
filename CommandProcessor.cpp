#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "CommandProcessor.hh"

template <typename V>
void print_container(V v)
{
    std::cout << "\n printing container: ";
    for (auto const& x: v)
        std::cout << x << ' ';
    std::cout << '\n';
}

int CommandProcessor::cmd_set_log_codes()
{
    std::cout << '\n' << __PRETTY_FUNCTION__;

    std::vector<uint16_t> log_codes;

    std::string str_code;

    while(cmd_stream_)
    {
        cmd_stream_ >> str_code;

        std::transform(str_code.begin(), str_code.end(), str_code.begin(), ::toupper);

        std::cout << " " << str_code;
    }

    return 0;    // Success
}

void CommandProcessor::process_command(const std::string &cmd)
{
    std::cout << '\n' << __PRETTY_FUNCTION__ << ": new command: " << cmd;

    cmd_stream_.str(cmd);    // Replace old command
    cmd_stream_.clear();

    std::string command;
    cmd_stream_ >> command;

    std::cout << "\n\tcmd_stream_: " << cmd_stream_.str();

    // Find command handler
    auto couple = cmd_handlers_.find(command);

    if(couple == cmd_handlers_.end()) {
        std::cout << "\n\tInvalid command: " << command;
        return;
    }

    // Handle command
    std::cout << "\n\tProcessing command: " << command;

    auto phandler = couple->second;

    (this->*phandler)();

    //std::vector<int> vec(
            //(std::istream_iterator<int>(cmd_stream_)), std::istream_iterator<int>());
    //print_container(vec);
}

