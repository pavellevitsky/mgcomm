/*!
 * @file ut-ModemAt.cpp
 *
 * @brief Modem AT Commands I/O unit test (example)
 *
 * @author Roman Raisin (roman.raisin@gmail.com)
 *
 * How to use: just run.
 * Optionally place a few commands to ./at-commands-list.txt
 * Optional argument 'd' enables dumps
 */

#include <iostream>
#include <fstream>
#include "ModemAt.hh"

int main(int argc, char ** argv)
{
    bool debug = false;

    if(argc > 1 && argv[1][0] == '-' && argv[1][1] == 'd')
    {
        debug = true;
    }

    ModemAt modem("/dev/smd11", debug);

    auto response = modem.at_cmd("");
    std::cout << "Response: " << response << "\n";
    std::cout << "--------------------------------------------\n";

    // AT command test sequence from file
    std::ifstream at_infile("/data/local/tmp/at-commands-list.txt");

    if(at_infile)
    {
        std::cout << "Read AT Commands from at-commands-list.txt\n";
        std::string line;

        while (std::getline(at_infile, line))
        {
            if(line.length() > 0)
            {
                std::cout << "\n--------------------------------------------\n";
                std::cout << "AT" << line << "\n";
                response = modem.at_cmd(line);
                std::cout << response;
            }
        }
        at_infile.close();
    }
    else
    {
        std::string commands[] = {"+CGMI", "+CGSN", "+CFUN?", "$MGPHYCFG=1"};
        std::cout << "Read AT Commands from hard-coded list\n";

        for(auto cmd: commands)
        {
            std::cout << "\n--------------------------------------------\n";
            std::cout << std::string("AT") + cmd;
            response = modem.at_cmd(cmd);
            std::cout << response;
        }
    }

    return 0;
}
