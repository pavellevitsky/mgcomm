/*!
 * @file Parser.cpp
 *
 * @brief Qualcomm information Parser - implementation
 *
 * @author Roman Raisin (roman.raisin@gmail.com)
 *
 * This class is for parsing Logs, Events, Messages and other info from Qualcomm modems
 */

#include "Parser.hh"
#include "common.hh"

Parser::Parser()
: bDump(false)
{
    // Initialize Log table
}

/*!
 * @brief Processes raw log stream from modem
 */
int Parser::process_log(unsigned char *raw, int len)
{
    std::cout << '\n' << __func__ ;

    // Saving packet. Note that in case we want to keep packet it should
    // be copied, not just referenced.
    raw_ = raw;
    len_ = len;
    
    if(bDump)    // Provide hex dump to console
        dump(raw, len);

    log_code_ = *(uint16_t*)(raw+2); 

    // Search for descriptor of log packet
    auto couple = log_descriptors.find(log_code_);
    if(couple == log_descriptors.end())
        printf("\n\t- Unknown Log packet with id: 0x%.04hX len=%d", log_code_, len);
    else
    {
        LogDescriptor desc = couple->second;

        std::cout << "\n\tProcessing packet: <" << desc.name << "> with id: ";
        printf("0x%.04hX", desc.code);

        Decoder decoder = desc.decoder;
        (this->*decoder)();    // call the decoder
    }

    return 0;
}

int Parser::dec_gsm_signalling()
{
    std::cout << "\nGSM LOG SIGNALLING handler";
    return 0;
}

int Parser::dec_gsm_powerscan()
{
    std::cout << "\nGSM LOG POWER SCAN handler";
    return 0;
}
