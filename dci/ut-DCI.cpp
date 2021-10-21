/*!
 * @file unittest-DCI.cpp
 *
 * @brief DCI interface class implementation hardware unit test
 *
 * @author	Roman Raisin (roman.raisin@gmail.com)
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
		printf(" 0x%.02hX", x);
}

class DCITest: public DCI
{
	virtual void _handle_log(unsigned char *ptr, int len) override
	{
		uint16_t log_code = *(uint16_t*)(ptr + 2);
		printf("\nReceived log 0x%.04hX", log_code);
		dump(ptr, len);

		DCI::_handle_log(ptr, len);
	};
};

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	std::cout << "\nDCI unit test (Qualcomm interface)\n";

	// Setup a Ctrl-C handler
	signal (SIGINT, catch_sigint);

	{
		DCITest dci;
		dci.setup_interface();

		auto print_line = []{ std::cout <<"___________________________________"; };

		while(!exit_flag)
		{
			print_line();
			std::unordered_set<uint16_t> log_codes { 0x5A71, 0x5A6C };
			std::cout << "\nSetting logs to catch:";
			print_container_uint16(log_codes);
			dci.add_log_codes(log_codes);

			sleep(10);

			print_line();
			log_codes = {0x512F, 0x5B2F, 0x5A64, 0x5A70};
			std::cout << "\nSetting logs to catch:";
			print_container_uint16(log_codes);
			dci.set_log_codes(log_codes);

			sleep(10);

			print_line();
			dci.reset_log_codes();
			sleep(10);
		}
	}

	std::cout << "\nDCI unit test is done\n";

	return 0;
}
