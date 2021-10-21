/*!
 * @file Parser.cpp
 *
 * @brief Qualcomm information Parser - implementation
 *
 * @author Roman Raisin (roman.raisin@gmail.com)
 *
 * This class is for parsing Logs, Events, Messages and other info from Qualcomm modems
 */

#include <iostream>
#include "Parser.hh"
#include "common.hh"

Parser::Parser(): bDump(false)
{
    // Initialize Log table
}

/*!
 * @brief Processes raw log stream from modem
 */
int Parser::process_log(unsigned char *raw, int len)
{
    std::cout << __func__ << " | len:" << len << " | ";

    // In case we want to keep packet it should be copied, not just referenced.
    raw_ = raw;
    len_ = len;
    
    if (bDump)
        dump(raw, len);   // Provide hex dump to console

    log_code_ = *(uint16_t*)(raw+2);

    // Search for descriptor of log packet
    auto couple = log_descriptors.find(log_code_);

    if(couple == log_descriptors.end())
        printf(" | Unknown Log packet id:0x%.04hX len=%d\n", log_code_, len);
    else
    {
        LogDescriptor desc = couple->second;

        printf("[0x%.04hX] ", desc.code);
        std::cout << desc.name;

        Decoder decoder = desc.decoder;
        (this->*decoder)();    // call the decoder
    }

    return 0;
}

int Parser::dec_gsm_signalling()
{
    std::cout << " handler:dec_gsm_signalling\n\n";
    return 0;
}

int Parser::dec_gsm_powerscan()
{
    std::cout << " handler:dec_gsm_powerscan\n\n";
    return 0;
}
