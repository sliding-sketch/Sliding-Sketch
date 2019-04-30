#ifndef SLIDING_BLOOM_H
#define SLIDING_BLOOM_H

/*
 * Sl-BF, Sliding Bloom Filter
 */

#include "data.h"
#include "basic.h"
#include "bitset.h"
#include "definition.h"

class Sliding_Bloom : public Basic
{
public:
    Sliding_Bloom(uint _STAGE_LEN, uint _HASH_NUM, uint _CYCLE);
    ~Sliding_Bloom();

    void Init(const Data& data, ulong time); //Insert data into Sl-BF
    bool Query(const Data& data, ulong time); //whether data is in the set

private:
    const uint STAGE_LEN;
    const uint CYCLE;
    uint Clock_Pos;
    uint Last_Time;
    BitSet* future;
    BitSet* now;
    void Clock_Go(ulong num); //clock moves around the Sl-BF
};

#endif // SLIDING_BLOOM_H
