#ifndef BITSET_H
#define BITSET_H

/*
 * My bitset
 */

#include <string.h>
#include "definition.h"

class BitSet
{
public:
    BitSet(uint _LENGTH);
    ~BitSet();
    void Set(uint index); //set the indexed bit to 1
    void Clear(uint index); //set the indexed bit to 0
    bool Get(uint index); //return the indexed bit

private:
    uchar* bitset;
};

#endif // BITSET_H
