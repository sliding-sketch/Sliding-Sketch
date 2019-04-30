#ifndef SPLITTER_H
#define SPLITTER_H
#define my_min(x, y) (x < y? x:y)
#define my_max(x, y) (x > y? x:y)

#include <iostream>
#include <string.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include "hash_class.h"


using namespace std;

struct Splitter{
    int init;
    int last;
    int counter;
    void print();
};

double err(Splitter s1, Splitter s2);

class Bucket{
public:
    int cycle;
    double tau;
    double mu;
    int hash_number;
    int row_length;

    vector<Splitter>* q;
    //vector<Splitter>* q_d;
    int* v;
    //int* v_d;

    Bucket(int _cycle, double _tau, double _mu, int _hash_number, int _row_length);

    void update(const unsigned char* str, int length, int time_stamp);//update an item
    //void cu_update(const unsigned char* str, int length, int time_stamp);
    //void count_update(const unsigned char* str, int length, int time_stamp);
    
    int q_memory();//return memory(bytes)
    //int q_memory_count();

    int query(const unsigned char* str, int length);//query an item
    int count_query(const unsigned char* str, int length);

    void phaseout(int i1, int i2, int time_stamp);//algorithm speedup
    void print(int i1, int i2);

};

#endif // SPLITTER_H