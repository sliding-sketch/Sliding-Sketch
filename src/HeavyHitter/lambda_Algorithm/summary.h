#ifndef SUMMARY_H
#define SUMMARY_H

#include "data.h"
#include "hash_class.h"
#include "definition.h"
#include <queue>
#include <iostream>
#include <unordered_map>
#include <memory.h>



using namespace std;

class Counter;

class Bucket{
public:
    int value;
    Counter* child;
    Bucket(){
        child = NULL;
    }
};

class Counter{
public:
    int l;
    Bucket* father;
    Counter* prev;
    Counter* next;
    Data ID;
    queue<int> que;
    Counter(Data data):ID(data){
        l = 0;
        father = NULL;
        prev = next = NULL;
    }
};

class Summary{
public:
    int MAX_SIZE;
    int MAX_NUM;
    int min_num;
    int memory;
    int cycle;
    int para_lambda;
    unordered_map<Data, Counter*, My_Hash> counter_map;
    Bucket* bucket_map;

    Summary(int num, int max_num, int _cycle, int _para_lambda);
    ~Summary();

    void Delete_Bucket(Bucket* bucket);
    void Delete_Counter(Counter* counter);

    void Remove(Counter* counter, Bucket* bucket);
    void Add(Counter* counter, Bucket* bucket);
    void Connect(Counter* ca, Counter* cb);

    int Query(Data data, int t);
    void Init(Data data, int t);//update an item
};


#endif // SUMMARY_H
