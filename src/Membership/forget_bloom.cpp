#include "forget_bloom.h"

Forget_Bloom::Forget_Bloom(uint _LENGTH, uint _HASH_NUM, uint _BLOOM_NUM, uint _CYCLE):
    Basic(_LENGTH, _HASH_NUM), BLOOM_NUM(_BLOOM_NUM), CYCLE(_CYCLE){
    future = 0;
    Last_Time = 0;
    bloom = new Bloom*[BLOOM_NUM];
    for(uint i = 0;i < BLOOM_NUM;++i){
        bloom[i] = new Bloom(LENGTH, HASH_NUM);
    }
}

Forget_Bloom::~Forget_Bloom(){
    for(uint i = 0;i < BLOOM_NUM;++i){
        delete bloom[i];
    }
    delete []bloom;
}

void Forget_Bloom::Init(const Data &data, ulong time){
    if(!Query(data, time)){
        bloom[future]->Init(data);
        bloom[(future + 1) % BLOOM_NUM]->Init(data);
    }
}

bool Forget_Bloom::Query(const Data &data, ulong time){
    Update(time * (BLOOM_NUM - 1) / CYCLE);
    if(bloom[future]->Query(data))
        return true;
    for(int i = (future + 1) % BLOOM_NUM;(i + 1) % BLOOM_NUM != future;i = (i + 1) % BLOOM_NUM){
        if(bloom[i]->Query(data) && bloom[(i + 1) % BLOOM_NUM]->Query(data))
            return true;
    }
    if(bloom[(future + BLOOM_NUM - 1) % BLOOM_NUM]->Query(data))
        return true;
    return false;
}

void Forget_Bloom::Update(ulong time){
    for(;Last_Time < time;++Last_Time){
        future = (future + BLOOM_NUM - 1) % BLOOM_NUM;
        delete bloom[future];
        bloom[future] = new Bloom(LENGTH, HASH_NUM);
    }
}
