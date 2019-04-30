#include "bitset.h"

BitSet::BitSet(uint _LENGTH){
    uint size = (_LENGTH >> 3) + 1;
    bitset = new uchar[size];
    memset(bitset, 0, size * sizeof(uchar));
}

BitSet::~BitSet(){
    delete [] bitset;
}

void BitSet::Set(uint index){
    uint position = (index >> 3);
    uint offset = (index & 0x7);
    bitset[position] |= (1 << offset);
}

void BitSet::Clear(uint index){
    uint position = (index >> 3);
    uint offset = (index & 0x7);
    uchar mask = 0xff;
    mask ^= (1 << offset);
    bitset[position] &= mask;
}

bool BitSet::Get(uint index){
    uint position = (index >> 3);
    uint offset = (index & 0x7);
    if((bitset[position] & (1 << offset)) != 0)
        return true;
    return false;
}


