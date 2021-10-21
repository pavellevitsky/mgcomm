#ifndef _ParserBase_h
#define _ParserBase_h

namespace
{
    static const uint16_t 
        LOG_1 = 0x0001,
        LOG_2 = 0x0002,
        LOG_3 = 0x0003;
}

class ParserBase
{
    public:

        ParserBase(){};

        // Disable copy constructor
        ParserBase(ParserBase&) = delete;

        // Proceses Log stream from modem
        int process_log(uint8_t *raw, int len);

    protected:

        unsigned char *raw_;    //! Current packet raw data
        int len_;                //! Current packet raw length
        uint16_t log_code_;

    private:

        using Decoder = int (ParserBase::*)(uint8_t *ptr, int len);

        typedef struct
        {
            uint16_t    code;
            const char *name;
            Decoder     decoder;
        } LogDescriptor;

        int decoder_1(uint8_t *packet, int len)
        {
            std::cout << " | decode_1()\n";
            return 0;
        }

        int decoder_2(uint8_t *packet, int len) 
        {
            std::cout << " | decode_2()\n";
            return 0;
        }

        int decoder_3(uint8_t *packet, int len) 
        {
            std::cout << " | decode_3()\n";
            return 0;
        }

        /// Defines decoder functions for logs by codes
        std::map<uint16_t, LogDescriptor> log_descriptors =
        {
            {LOG_1, {LOG_1, "LOG_1", &ParserBase::decoder_1}},
            {LOG_2, {LOG_2, "LOG_2", &ParserBase::decoder_2}},
            {LOG_3, {LOG_3, "LOG_3", &ParserBase::decoder_3}},
        };
};

#endif
