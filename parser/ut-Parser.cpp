/*!
 * @file unittest-Parser.cpp
 *
 * @brief Unit test for Parser class.
 *
 * @author Roman Raisin (roman.raisin@gmail.com)
 */

#include <iostream>
#include <cstring>
#include <assert.h>
#include "Parser.hh"

int main(int argc, char **argv)
{
	std::cout << "\nParser unit test\n";

	Parser parser;
	parser.bDump = true;

	const char *packets[] = 
	{
		"ab\x2F\x51some data 1",
		"cdd daYYbata 9",
		"cd\x2F\x5B datdca 9",
		"cdd daYYbata 3",
		"ef\x64\x50ZZsome data 2",
		"ghAD dZZ Bata 2",
		"gh\x64\x5AZZso me dat",
	};

	for(auto pkt: packets)
	{
		parser.process_log((uint8_t*)pkt, strlen(pkt));
	}

	std::cout << "\nSUCCESS";
	return 0;
}
