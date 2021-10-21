/*!
 * @file ParserBase.cpp
 *
 * @brief Software unit test for ParserBase
 *
 * @author Roman Raisin (roman.raisin@gmail.com)
 */

#include <iostream>
#include <cstring>
#include <assert.h>
#include "ParserBase.hh"

//! Raw logs processing
int ParserBase::process_log(uint8_t *ptr, int len)
{
    std::cout << "\n\nParserBase::process_log(): " << ptr;

    int selector = ptr[0] - '0';    // Making int from char

    // Search for descriptor of log packet
    auto couple = log_descriptors.find(selector);
    if(couple == log_descriptors.end())
        std::cout << "\n- Unknown packet with id: " << selector; 
    else
    {
        LogDescriptor desc = couple->second;

        std::cout << "\n\tProcessing packet: <" << desc.name
            << "> with id: ";
        printf("0x%.04hi", desc.code);

        Decoder decoder = desc.decoder;
        (this->*decoder)(ptr, len);    // call the decoder
    }

    return 0;
}

int main(int argc, char **argv)
{
    std::cout << "\nParserBase unit test\n";

    ParserBase parser;

    const char *packets[] = 
    {
        "1 some data 1",
        "9 bad data 9",
        "2 some data 2",
    };

    for(auto pkt: packets)
    {
        parser.process_log(
                (uint8_t*)pkt, 
                strlen(pkt));
    }

    assert(1 == 1);

    std::cout << "\nSUCCESS";
    return 0;
}
