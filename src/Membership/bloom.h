#ifndef BLOOM_H
#define BLOOM_H

/*
 * Bloom Filter
 */

#include "data.h"
#include "basic.h"
#include "bitset.h"

class Bloom : public Basic{
public:
    Bloom(uint _LENGTH, uint _HASH_NUM);
    ~Bloom();

    void Init(const Data& data); //Insert data into bloom filter
    bool Query(const Data& data); //whether data is in the set

private:
    BitSet* bitset;
};

#endif // BLOOM_H
