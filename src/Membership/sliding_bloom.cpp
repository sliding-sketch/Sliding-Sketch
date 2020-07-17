#include "sliding_bloom.h"

Sliding_Bloom::Sliding_Bloom(uint _STAGE_LEN, uint _HASH_NUM, uint _CYCLE):
    Basic(_STAGE_LEN * _HASH_NUM, _HASH_NUM), STAGE_LEN(_STAGE_LEN), CYCLE(_CYCLE){
    Clock_Pos = 0;
    Last_Time = 0;
    future = new BitSet(LENGTH);
    now = new BitSet(LENGTH);
}

Sliding_Bloom::~Sliding_Bloom(){
    delete future;
    delete now;
}

void Sliding_Bloom::Init(const Data &data, ulong time){
    Clock_Go(time * (LENGTH / CYCLE));
    for(uint i = 0;i < HASH_NUM;++i){
        uint position = data.Hash(i) % STAGE_LEN + i * STAGE_LEN;
        future->Set(position);
    }
}

bool Sliding_Bloom::Query(const Data &data, ulong time){
    Clock_Go(time * (LENGTH / CYCLE));
    for(uint i = 0;i < HASH_NUM;++i){
        uint position = data.Hash(i) % STAGE_LEN + i * STAGE_LEN;
        if(!future->Get(position) && !now->Get(position))
            return false;
    }
    return true;
}

void Sliding_Bloom::Clock_Go(ulong num){
    for(;Last_Time < num;++Last_Time){
        now->Clear(Clock_Pos);
        if(future->Get(Clock_Pos)){
            now->Set(Clock_Pos);
            future->Clear(Clock_Pos);
        }
        Clock_Pos = (Clock_Pos + 1) % LENGTH;
    }
}
