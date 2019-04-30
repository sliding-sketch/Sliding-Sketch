#include "bloom.h"

void Bloom::Init(const unsigned char* str, int length, unsigned int num){
    for(int i = 0;i < hash_number;++i){
        unsigned int position = Hash(str, i, length) % row_length + i * row_length;
        unsigned int pos = POSITION(position);
        unsigned int offset = OFFSET(position);
        filter[pos] |= (1 << (offset << 1));
    }
    Clock_Go((num + 1) * step);
}

static const unsigned char mask[4] = {0xfc, 0xf3, 0xcf, 0x3f};
unsigned int Bloom::Query(const unsigned char* str, int length){
    for(int i = 0;i < hash_number;++i){
        unsigned int position = Hash(str, i, length) % row_length + i * row_length;
        unsigned int pos = POSITION(position);
        unsigned int offset = OFFSET(position);
        if( (filter[pos] & (~mask[offset])) == 0 )
            return 0;
    }
    return 1;
}

unsigned int Bloom::Real_Query(const unsigned char* str, int length){
    bool* have = new bool[hash_number << 1];
    for(int i = 0; i < (hash_number << 1);i++ ){
        have[i] = 0;
    }
    int k = clock_pos / row_length, i = 0;
    unsigned int position = Hash(str, k ,length) % row_length + k * row_length;
    if(position < clock_pos){
        k = (k + 1) % hash_number;
        position = Hash(str, k ,length) % row_length + k * row_length;
    }

    unsigned int pos = POSITION(position);
    unsigned int offset = OFFSET(position);
    unsigned char temp = (filter[pos] >> (offset << 1));
    have[i + hash_number] = temp & 1;
    have[i] = (temp >> 1) & 1;

    for(int j = (k + 1) % hash_number;j != k;j = (j + 1) % hash_number){
        i += 1;
        position = Hash(str, j, length) % row_length + j * row_length;
        pos = POSITION(position);
        offset = OFFSET(position);
        temp = (filter[pos] >> (offset << 1));
        have[i + hash_number] = temp & 1;
        have[i] = (temp >> 1) & 1;
    }

    int l = 0;
    for(int j = 0;j < (hash_number << 1);++j){
        if(have[j]){
            l += 1;
            if(l >= hash_number)
                return 1;
        }
        else
            l = 0;
    }

    if(l >= hash_number)
        return 1;
    else return 0;
}

void Bloom::Clock_Go(unsigned int num){
    for(;last_time < num;++last_time){
        unsigned int pos = POSITION(clock_pos);
        unsigned int offset = OFFSET(clock_pos);
        unsigned char future = (filter[pos] >> (offset << 1)) & 1;
        filter[pos] &= mask[offset];
        filter[pos] |= (future << ((offset << 1) + 1));
        clock_pos = (clock_pos + 1) % len;
    }
}

