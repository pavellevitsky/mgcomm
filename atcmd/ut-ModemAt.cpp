/*!
 * @file ut-ModemAt.cpp
 * @brief Modem AT Commands I/O unit test (example)
 * @author Roman Raisin (roman.raisin@gmail.com)
 *
 * How to use: just run.
 * Optionally place a few commands to ./at-commands-list.txt
 * Optional argument '-f' enables reading commands from above file
 * Optional argument '-c command' writes it to Unix device
 * Optional argument '-d' enables dumps
 */

#include <iostream>
#include <fstream>
#include "ModemAt.hh"

int main (int argc, char ** argv)
{
    bool is_command = false;
    bool is_file    = false;
    bool is_debug   = false;
    std::string cmd;
    std::string resp;

    if (argc == 1)
    {
        std::cout << "USAGE : [-d | -f] [-c AT$MGPHYCFG=param1,param2,...]\n";
        return 0;
    }
    else if (argc == 2)
    {
        if (argv[1][0] == '-' && argv[1][1] == 'd')
        {
            is_debug = true;
        }
        if (argv[1][0] == '-' && argv[1][1] == 'f')
        {
            is_file = true;
        }
    }
    else if (argc == 3)
    {
        if (argv[1][0] == '-' && argv[1][1] == 'c')
        {
            is_command = true;
            cmd.assign(argv[2]);
        }
    }
    else if (argc == 4)
    {
        if (argv[1][0] == '-' && argv[1][1] == 'd')
        {
            is_debug = true;
        }
        if (argv[2][0] == '-' && argv[2][1] == 'c')
        {
            is_command = true;
            cmd.assign(argv[3]);
        }
    }

    ModemAt modem("/dev/smd11", is_debug);

    std::cout << "--------------------------------------------\n";

    if (is_command)
    {
        std::cout << "AT" << cmd.c_str() << "\n";
        resp = modem.at_cmd(cmd.c_str());
        std::cout << resp;
    }
    else if (is_file)
    {
        // AT command test sequence from file
        std::ifstream at_infile("/data/local/tmp/at-commands-list.txt");

        if (at_infile)
        {
            std::cout << "Read AT Commands from at-commands-list.txt\n";
            std::string line;

            while (std::getline(at_infile, line))
            {
                if(line.length() > 0)
                {
                    std::cout << "\n--------------------------------------------\n";
                    std::cout << "AT" << line << "\r\n";
                    resp = modem.at_cmd(line);
                    std::cout << resp;
                }
            }
            at_infile.close();
        }
    }
    else
    {
        std::string commands[] = {"I", "+CGMI", "+CFUN?", "$MGPHYCFG?"};
        std::cout << "Execute below hard-coded list : ATI, AT+CGMI, AT+CFUN?, AT$MGPHYCFG?\n";

        for(auto cmd: commands)
        {
            std::cout << "\n--------------------------------------------\n";
            std::cout << std::string("AT") + cmd;
            resp = modem.at_cmd(cmd);
            std::cout << resp;
        }
    }

    return 0;
}
