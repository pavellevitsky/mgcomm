/*!
 * @file ut-Parser.cpp
 *
 * @brief Unit test for Parser class.
 *
 * @author Roman Raisin (roman.raisin@gmail.com)
 */

#include <assert.h>
#include <iostream>
#include <cstring>
#include "Parser.hh"

int main(int argc, char **argv)
{
	std::cout << "\n***** [UT] Parser *****\n";

	Parser parser;
	parser.bDump = true;

	const char *packets[] = 
	{
		"\xFF\x01\x2F\x51\xA0\x01\x02\x03",
		"\xFF\x02\x2F\x5B\x10\x11\x12\x13",
		"\xFF\x03\x64\x50\xA0\x21\x22\x23",
		"\xFF\x04\x2F\x51\xA0\x31\x32\x33",
	};

	for(auto pkt: packets)
	{
		parser.process_log((uint8_t*)pkt, strlen(pkt));
	}

	std::cout << "\n***** SUCCESS *****\n\n";

	return 0;
}
