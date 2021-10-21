/*!
 * @file common.hh
 *
 * @brief Common and utility definitions
 *
 * @author	Roman Raisin (roman.raisin@gmail.com)
 */

#ifndef _common_hh
#define _common_hh

#include <cstdio>

inline void dump(void *data, unsigned len)
{
	unsigned u = 0;
	uint8_t *p = (uint8_t*) data;

	while(u < len) 
	{
		if(u % 16 == 0) printf("\n");
		if(u % 8 == 0) printf(" ");
		if(u % 4 == 0) printf(" ");

		printf(" %02x", p[u]);
		u++;
	}

	printf("\n");
}

#endif
