/*!
 * @file ut-DCI.cpp
 *
 * @brief DCI interface class implementation hardware unit test
 *
 * @author Roman Raisin (roman.raisin@gmail.com)
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "common.hh"
#include "DCI.hh"

// SIGINT handler (Ctrl-C or so)
bool exit_flag = false;

void catch_sigint (int sig)
{
    (void) sig;

    std::cout << "\nSIGINT handled\n";
    exit_flag = true;
}

template <typename V>
void print_container_uint16(V v)
{
    for (auto x: v)
        printf("0x%.02hX ", x);
    printf("\n");
}

class DCITest: public DCI
{
    virtual void _handle_log(unsigned char *ptr, int len) override
    {
        uint16_t log_code = *(uint16_t*)(ptr + 2);
        printf("[0x%.04hX]\n", log_code);
        dump(ptr, len);

        DCI::_handle_log(ptr, len);
    };
};

int main (int argc, char **argv)
{
    (void)argc;
    (void)argv;
    DCITest dci;
    auto print_line = []{ std::cout <<"------------------------------------------------------\n"; };

    std::cout << "******************************************************\n";
    std::cout << "** DCI (Qualcomm Diag Consumer Interface) UNIT TEST **\n";
    std::cout << "******************************************************\n";

    // Setup a Ctrl-C handler
    signal (SIGINT, catch_sigint);

    dci.setup_interface();

    while(!exit_flag)
    {
        std::unordered_set<uint16_t> log_codes;

        print_line();
        log_codes = { 0x512F, 0x5B2F, 0x5A64, 0x5A70 };
        std::cout << "Set GSM logs -> ";
        print_container_uint16(log_codes);
        dci.set_log_codes(log_codes);
        sleep(10);

        print_line();
        log_codes = { 0x4111, 0x4125, 0x412F, 0x41AC };
        std::cout << "Add WCDMA logs -> ";
        print_container_uint16(log_codes);
        dci.add_log_codes(log_codes);
        sleep(10);

        print_line();
        log_codes = { 0xB031, 0xB119, 0xB197, 0xB0CB };
        std::cout << "Add LTE logs -> ";
        print_container_uint16(log_codes);
        dci.add_log_codes(log_codes);
        sleep(10);

        print_line();
        std::cout << "Cleanup logs list\n";
        dci.reset_log_codes();
        sleep(5);
    }

    std::cout << "DCI UNIT TEST finished\n";

    return 0;
}
