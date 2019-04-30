#ifndef FORGET_BLOOM_H
#define FORGET_BLOOM_H

/*
 * FBF, Forgetful Bloom Filter
 */

#include "data.h"
#include "basic.h"
#include "bloom.h"
#include "definition.h"

class Forget_Bloom : public Basic{
public:
    Forget_Bloom(uint _LENGTH, uint _HASH_NUM, uint _BLOOM_NUM, uint _CYCLE);
    ~Forget_Bloom();

    void Init(const Data &data, ulong time); //Insert data into FBF
    bool Query(const Data &data, ulong time); //whether data is in the set

private:
    const uint BLOOM_NUM; //how many bloom filter in the FBF
    const uint CYCLE;
    uint Last_Time;
    uint future;  //which bloom filter is the "future"
    Bloom **bloom;
    void Update(ulong time); //clear the oldest bloom filter and new a bloom filter
};

#endif // FORGET_BLOOM_H
