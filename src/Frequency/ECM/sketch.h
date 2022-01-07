#ifndef SKETCH_H
#define SKETCH_H
#include <iostream>
#include <algorithm>
#include <string.h>
#include <vector>
#include <utility>
#include <stdlib.h>
#include "hash_class.h"

using namespace std;

#define min(x, y) (x < y? x:y)
#define max(x, y) (x > y? x:y)

class Counter{
public:
    int update_k;
    int cycle;
    vector<pair<int, int> > element;
    vector<pair<int, int> > element_decrease;
    Counter(int _update_k, int _cycle);

    void update(int time_stamp);
    void decrease(int time_stamp);

    int query(int time_stamp);
    int query_all(int time_stamp);

    void counter_merge(vector<pair<int, int> >::iterator iter);
    void counter_merge_d(vector<pair<int, int> >::iterator iter);

   	vector<pair<int, int> >::iterator counter_merge_value(vector<pair<int, int> >::iterator iter);
    vector<pair<int, int> >::iterator counter_merge_value_d(vector<pair<int, int> >::iterator iter);

    void counter_delete(int time_stamp);
    void counter_delete_d(int time_stamp);

    int q_memory();
    int q_memory_all();

    void counter_print();
    
};


class ECM{
public:
    Counter** ECM_start;
    ECM(int _cycle, int _update_k,int _hash_number, int _row_length);
    int cycle;
    int update_k;
    int hash_number;
    int row_length;
    void update(const unsigned char* str, int length, int time_stamp);//update item
    int query(const unsigned char* str, int length, int time_stamp);//query item
    int q_memory(int time_stamp);//query memory
};

class ECU{
public:
    Counter** ECU_start;
    ECU(int _cycle, int _update_k,int _hash_number, int _row_length);
    int cycle;
    int update_k;
    int hash_number;
    int row_length;
    void update(const unsigned char* str, int length, int time_stamp);
    int query(const unsigned char* str, int length, int time_stamp);
    int q_memory(int time_stamp);
};

class ECO{
public:
    Counter** ECO_start;
    ECO(int _cycle, int _update_k,int _hash_number, int _row_length);
    int cycle;
    int update_k;
    int hash_number;
    int row_length;
    void update(const unsigned char* str, int length, int time_stamp);
    int query(const unsigned char* str, int length, int time_stamp);
    int q_memory(int time_stamp);
};

#endif // SKETCH_H
