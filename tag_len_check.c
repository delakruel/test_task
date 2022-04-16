#include "sel_ppse.h"

bool    is_constructed_or_primitiv_tag(unsigned short c)
{
    if (c & 0b00100000)
        return 1;
    return 0;
}

bool    is_long_tag(unsigned short c)
{
    if ((c & 0b00011111) == 0b00011111)
        return 1;
    return 0;
}

bool    is_super_long_tag(unsigned short c)
{
    if (c & 0b10000000)
        return 1;
    return 0;
}

bool    is_len_2(unsigned short c)
{
    if ((c & 0b10000001) == 0b10000001)
        return 1;
    return 0;
}

bool    is_len_3(unsigned short c)
{
    if ((c & 0b10000010) == 0b10000010)
        return 1;
    return 0;
}