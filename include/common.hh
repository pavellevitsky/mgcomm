/*!
 * @file common.hh
 *
 * @brief Common and utility definitions
 *
 * @author Roman Raisin (roman.raisin@gmail.com)
 */

#ifndef _common_hh
#define _common_hh

inline void dump(void *data, unsigned len)
{
    unsigned u = 0;
    uint8_t *p = (uint8_t*) data;

    while(u < len) 
    {
        printf("%02x ", p[u]);
        u++;
        if(u % 16 == 0)
            printf("\n");
    }

    printf("\n");
}

#endif
