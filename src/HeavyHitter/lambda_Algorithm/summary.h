//
// Created by z yd on 2020-07-30.
//

#ifndef SLIDING_SUMMARY_H
#define SLIDING_SUMMARY_H

#include <queue>
#include <iostream>
#include <unordered_map>
#include <memory.h>
#include <list>
#include "data.h"
#include "hash_class.h"
#include "defintion.h" 
using namespace std;

class Counter;

class Bucket{
public:
    int value;
    Counter* child;
    Bucket* next;
    Bucket(){
        child = NULL;
        next = NULL;
    }
};

class Counter{
public:
    int l;
    Bucket* father;
    Counter* prev;
    Counter* next;
    DATA_TYPE ID;
    queue<int> que;
    Counter(DATA_TYPE data):ID(data){
        l = 0;
        father = NULL;
        prev = next = NULL;
    }
};

class Sketch{
public:
    Bucket* bucket_head;
    unordered_map<DATA_TYPE, Counter*> counter_map;
    int cycle_i;
    int MAX_SIZE;
    int datalen;
    int counter_size;
    int min_num;
    int memory;
    int cycle;
    int para_lambda;
    Sketch(int num, int datalen, int _cycle, int _para_lambda,int _cycle_i);
    ~Sketch();

    void Delete_Bucket(Bucket* bucket);
    void Delete_Counter(Counter* counter);
    void Delete_List(Bucket* bucket);

    void Remove(Counter* counter, Bucket* bucket);
    void Add(Counter* counter, Bucket* bucket);
    void Connect(Counter* ca, Counter* cb);

    void Init(DATA_TYPE data,int t);
    int Query(DATA_TYPE data, int t);
};

class Summary{		//datalen is the length of the ID;
public:
    int MAX_SIZE;
    int counter_size;
    int datalen;
    int min_num;
    int memory;
    int cycle;
    int para_lambda;

    Sketch* S[2];
    Summary(int num, int dl, int _cycle, int _para_lambda);
    ~Summary();

    int Query(DATA_TYPE data, int t);
    void Init(DATA_TYPE data, int t);//update an item
};


#endif //SLIDING_SUMMARY_H
