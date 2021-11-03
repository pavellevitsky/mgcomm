/*!
 * @file ModemAt.cpp
 *
 * @brief Modem AT Commands I/O
 *
 * @author Roman Raisin (roman.raisin@gmail.com)
 *
 * This class is an example of AT Commands communication with modem
 */

#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include "ModemAt.hh"
#include "common.hh"

// https://www.tldp.org/HOWTO/text/Serial-Programming-HOWTO
struct sigaction saio;    /* definition of signal action */         

int wait_flag = true;       /* TRUE while no signal received */   

void signal_handler_IO (int /*status*/)
{
    printf("received SIGIO signal\n");
    wait_flag = false;
}

ModemAt::ModemAt(const char *device_name, bool debug)
    :debug_{debug}
{
    fd_ = open(device_name, O_RDWR | O_NOCTTY);

    if(fd_ < 0)
        throw "Can't open modem port";

    // install the signal handler before making the device asynchronous
    saio.sa_handler = signal_handler_IO;
    saio.sa_mask = 0;
    saio.sa_flags = 0;
    saio.sa_restorer = NULL;
    sigaction(SIGIO,&saio,NULL);

    /* allow the process to receive SIGIO */
    fcntl(fd_, F_SETOWN, getpid());

    // Make fd unblocking on read
    int flags = fcntl(fd_, F_GETFL, 0);
    fcntl(fd_, F_SETFL, flags | O_NONBLOCK);

    // Send \r
    write_("");

    // Flush input
    std::cout << "Flashing input\r\n";
    read_();

    // Disable echo
    write_("E0");
    read_();
}

ModemAt::~ModemAt()
{
    if(fd_ > 0)
    {
        // Enable echo
        std::cout << "-----------------------------------\n";
        std::cout << "ModemAt cleanup\r\n";
        write_("E1;V1");
        read_();
    }
}

std::string ModemAt::at_cmd(std::string command, int timeout)
{
    write_(command);
    return read_(timeout);
}

std::string ModemAt::read_(unsigned timeout)
{
    const auto bufsize = 64;
    char buf[bufsize];
    std::string result;

    // Read until second \r
    unsigned tenth = timeout * 10;    // in 1/10 s

    do
    {
        int cnt = read(fd_, buf, bufsize-1);    // 1 byte for termination

        if(cnt > 0)
        {
            buf[cnt] = 0;    // Add string terminator
            result += buf;

            if(debug_) {
                printf("READ %d bytes\r\n", cnt);
                dump(buf, cnt);
            }

            // Check for termination. Note: bad performance!
            size_t pos = result.find("\r\nOK\r\n");

            if(pos != std::string::npos)
            {
                if(debug_)
                    std::cout << "FOUND OK @ " << pos;
                break;
            }

            pos =  result.find("\r\nERROR\r\n");

            if(pos != std::string::npos)
            {
                if(debug_)
                    std::cout << "FOUND ERROR @ " << pos;
                break;
            }
        }
        else    // Nothing read - wait
            usleep(100000);    // 0.1 s
    } while(tenth--);

    if(debug_)
    {
        std::cout << "\r\nread_() finished, length:" << result.length() << "\n";
    }

    return result;
}

void ModemAt::write_(const std::string command)
{
    std::string full_cmd = "AT" + command + "\r\n";

    if(debug_)
        std::cout << "WRITE " << full_cmd;

    size_t written = write(fd_, full_cmd.c_str(), full_cmd.size());

    if(written != full_cmd.size())
        throw "Command write failed";
}
