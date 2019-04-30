#ifndef BLOOM_H
#define BLOOM_H

#include <iostream>
#include <algorithm>
#include <string.h>
#include "hash_class.h"

using namespace std;


#define POSITION(x) (x >> 2)
#define OFFSET(x) (x & 0x3)

class Bloom{
public:
    unsigned int clock_pos;
    unsigned int len;
    unsigned int step;
    unsigned int last_time;
    unsigned char* filter;
    int row_length;
    int hash_number;

    Bloom(unsigned int c, unsigned int l, int _row_length, int _hash_number):len(l),step(l/c),row_length(_row_length),hash_number(_hash_number){
        clock_pos = 0;
        last_time = 0;
        filter = new unsigned char [l >> 2];
        memset(filter, 0, l >> 2);
    }
    ~Bloom(){delete [] filter;}

    void Clock_Go(unsigned int num);
    void Init(const unsigned char* str, int length, unsigned int num);
    unsigned int Query(const unsigned char* str, int length);
    unsigned int Real_Query(const unsigned char* str, int length);
};

#endif // BLOOM_H
