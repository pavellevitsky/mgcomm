#ifndef _Parser_h
#define _Parser_h

#include <map>
#include <cstdint>
#include "ParserBase.hh"
#include "log_codes_gsm.h"

#define DSDS 0x0A00        // The DSDS addition to normal Log code

#define LOG_GSM_DSDS_POWER_SCAN_C (LOG_GSM_POWER_SCAN_C + DSDS)

class Parser: public ParserBase
{
    public:

        Parser();

        /// Proceses Log stream from modem
         int process_log(uint8_t *raw, int len);

         bool bDump;    //! Whether to dump incoming packets, defaults to false

    private:
    
        using Decoder = int (Parser::*)();

        /// Qualcomm modem log description
        typedef struct
        {
            uint16_t code;     /// Log code
            const char* name;  /// Name of log
            Decoder decoder;   /// Reference to decoding method
        } LogDescriptor;

        //! LOG handlers definition (code -> function address)
        std::map<uint16_t, LogDescriptor> log_descriptors =
        {
            {LOG_GSM_RR_SIGNALING_MESSAGE_C,
                {LOG_GSM_RR_SIGNALING_MESSAGE_C, "LOG GSM SIGNALLING MESSAGE",
                    &Parser::dec_gsm_signalling}},

            {LOG_GSM_RR_SIGNALING_MESSAGE_C + DSDS,
                {LOG_GSM_RR_SIGNALING_MESSAGE_C + DSDS, "LOG GSM SIGNALLING MESSAGE DSDS",
                    &Parser::dec_gsm_signalling}},

            {LOG_GSM_POWER_SCAN_C,
                {LOG_GSM_POWER_SCAN_C, "LOG GSM POWER SCAN",
                    &Parser::dec_gsm_powerscan}},

            {LOG_GSM_POWER_SCAN_C + DSDS,
                {LOG_GSM_POWER_SCAN_C + DSDS, "LOG GSM POWER SCAN DSDS",
                    &Parser::dec_gsm_powerscan}},
        };

        /// Handlers declaration

        int dec_gsm_signalling();
        int dec_gsm_powerscan();
};

#endif
