#ifndef _command_processor_hh
#define _command_processor_hh

#include <string>
#include <sstream>
#include <map>

class CommandProcessor
{
    public:

        void process_command(const std::string &cmd);

    private:

        //! Incoming command as a stream
        std::stringstream cmd_stream_;

        //! Type definition for command handler 
        using CommandHandler = int (CommandProcessor::*)();

        //! Command handlers
        std::map<std::string, CommandHandler> cmd_handlers_ =
        {
            {std::string("set_log_codes"), &CommandProcessor::cmd_set_log_codes},
        };

        int cmd_set_log_codes();
};

#endif
