/*!
 * @file ParserBase.cpp
 *
 * @brief Software unit test for ParserBase
 *
 * @author Roman Raisin (roman.raisin@gmail.com)
 */

#include <map>
#include <iostream>
#include <cstring>
#include <assert.h>
#include "ParserBase.hh"

//! Raw logs processing
int ParserBase::process_log(uint8_t *ptr, int len)
{
    std::cout << __func__ << " | len:" << len << " data:" << ptr;

    int selector = ptr[0] - '0';    // Making int from char

    // Search for descriptor of log packet
    auto couple = log_descriptors.find(selector);

    if(couple == log_descriptors.end())
        std::cout << " | Unknown packet id:" << selector;
    else
    {
        LogDescriptor desc = couple->second;

        std::cout << " | Processing packet id:" << selector << " | name:" << desc.name << " | code:";
        printf("0x%.04hi", desc.code);

        Decoder decoder = desc.decoder;
        (this->*decoder)(ptr, len);    // call the decoder
    }

    return 0;
}

int main(int argc, char **argv)
{
	std::cout << "\n***** [UT] ParserBase *****\n";

    ParserBase parser;

    const char *packets[] = 
    {
        "1 data 1",
        "2 data 2",
        "3 data 3",
        "4 data 4",
    };

    for(auto pkt: packets)
    {
        parser.process_log((uint8_t*)pkt, strlen(pkt));
    }

    assert(true);

	std::cout << "\n***** SUCCESS *****\n\n";

    return 0;
}
