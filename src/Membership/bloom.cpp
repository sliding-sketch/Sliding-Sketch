#include "bloom.h"

Bloom::Bloom(uint _LENGTH, uint _HASH_NUM):
    Basic(_LENGTH, _HASH_NUM){
    bitset = new BitSet(LENGTH);
}

Bloom::~Bloom(){
    delete bitset;
}

void Bloom::Init(const Data &data){
    for(uint i = 0;i < HASH_NUM;++i){
        uint position = data.Hash(i) % LENGTH;
        bitset->Set(position);
    }
}

bool Bloom::Query(const Data &data){
    for(uint i = 0;i < HASH_NUM;++i){
        uint position = data.Hash(i) % LENGTH;
        if(!bitset->Get(position))
            return false;
    }
    return true;
}
