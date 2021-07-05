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
    int first_timestamp;
    Sketch(int num, int datalen, int _cycle, int _para_lambda,int _cycle_i,int _first_timestamp);
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

Summary::Summary(int num, int dl, int _cycle, int _para_lambda){
    min_num = 0x7fffffff;
    MAX_SIZE = num;
    datalen = dl;
    counter_size = 4 + 8*5 + dl;	// 1 counter, 2 pointer for double linked list, 1 pointer to bucket, 2 pointer for queue, head and tail, and 1 ID
    cycle = _cycle;
    para_lambda = _para_lambda;
    S[0] = new Sketch(num/2, dl, _cycle,_para_lambda,0,0);
    S[1] = new Sketch(num/2, dl, _cycle,_para_lambda,0,0);
}

Sketch::Sketch(int num, int dl, int _cycle, int _para_lambda,int _cycle_i,int _first_timestamp){
    first_timestamp = _first_timestamp;
    datalen = dl;
    cycle_i = _cycle_i;
    min_num = 0x7fffffff;
    MAX_SIZE = num;
    counter_size = (4+8*5+dl);
    int hash_kv_size = datalen + 8*2; //1 key, 1 value which is a counter pointer, and 1 pointer for linked list of hash table;
    MAX_SIZE -= (num/(counter_size+hash_kv_size))*1.25*hash_kv_size; // the memory of hash table, allocated according to the max number of counter; In the worst case, each counter has value smaller than lamda, the queue is empty, and most counters have the same value, the bucket number is
    // small compared to counter number. The load factor is set to 80%. The memory for hash table is preserved.
    cycle = _cycle;
    para_lambda = _para_lambda;
    // bucket_map = new Bucket[MAX_NUM];
    memory = 0;
    bucket_head = NULL;
    // memset(bucket_map, 0 ,sizeof(Bucket) * MAX_NUM);
    // for(int i = 0;i < MAX_NUM;++i)
    // bucket_map[i].value = i;    // use a list, not a sequence table
}

Sketch::~Sketch(){
    // for(int i = 0;i < MAX_NUM;++i){
    //     Delete_Bucket(&bucket_map[i]);
    // }
    Delete_List(bucket_head);
    // for(Bucket*cur = bucket_head;cur != NULL;cur=cur->next)
    // Delete_Bucket(cur);
    // delete []bucket_map;
}

Summary::~Summary(){
    delete S[0];
    delete S[1];
}

void Sketch::Delete_List(Bucket *bucket){
    if(bucket == NULL) return;
    Delete_List(bucket->next);
    Delete_Bucket(bucket);
    // delete bucket;
}

void Sketch::Delete_Bucket(Bucket *bucket){
    if(bucket->child != NULL)
        Delete_Counter(bucket->child);
    bucket->child = NULL;
    memory -= sizeof(Bucket);
    delete bucket;
}

void Sketch::Delete_Counter(Counter *counter){
    counter_map.erase(counter->ID);
    // memory -= (sizeof(DATA_TYPE) + sizeof(Counter*) + 8) * Mt; //the memory for hash table is not dynamicly allocated.
    memory -= counter_size;
    memory -= counter->que.size() * (sizeof(int) + 8); // one int and one counter;
    if(counter->next != NULL){
        Delete_Counter(counter->next);
    }
    delete counter;
}

void Sketch::Init(DATA_TYPE data, int t){
    if(counter_map.find(data) != counter_map.end()){
        Counter* temp = counter_map[data];
        int value = temp->father->value;
        temp->l += 1;
        if(temp->l >= para_lambda){
            temp->l = 0;
            temp->que.push(t);
            memory += (sizeof(int) + 8);
        }
        value += 1;
        Bucket *bucket =  temp->father->next;
        Bucket *fbucket = temp->father;
        Remove(temp, fbucket);
        if(value > 0){
            if(bucket == NULL || bucket->value > value){
                Bucket *nbucket = new Bucket;
                memory += sizeof(Bucket);
                nbucket->value = value;
                nbucket->next = fbucket->next;
                fbucket->next = nbucket;
                Add(temp,nbucket);
            } else if(bucket->value == value){
                Add(temp,bucket);
            }
        }
        else{
            //      memory -= (sizeof(DATA_TYPE) + sizeof(Counter*)+8) * Mt;
            memory -= counter_size;
            memory -= temp->que.size() * (sizeof(int) + 8);

            counter_map.erase(temp->ID);
            delete temp;
        }
    }
    else{
        if(memory >= MAX_SIZE){
            // for(int i = min_num;i < MAX_SIZE;++i){
            //     if(bucket_map[i].child != NULL){
            //         Delete_Bucket(&bucket_map[i]);
            //         //cout << i << endl;
            //         min_num = i;
            //         break;
            //     }
            // }
            Bucket *prev = NULL;
            for(Bucket *bucket = bucket_head,*nxt;bucket!=NULL;bucket=nxt){
                nxt = bucket->next;
                if(bucket->child == NULL){
                    if(prev == NULL) bucket_head = bucket->next;
                    else{
                        prev->next = bucket->next;
                    }
                    Delete_Bucket(bucket);
                    bucket = prev;
                }
                prev = bucket;
            }
            if(bucket_head != NULL){
                Bucket *head = bucket_head->next;
                Delete_Bucket(bucket_head);
                bucket_head = head;
            }
        }

        Counter* counter = new Counter(data);
        counter_map[data] = counter;
        if(bucket_head == NULL || bucket_head->value != 1){
            Bucket *head = new Bucket;
            head->value = 1;
            head->next = bucket_head;
            bucket_head = head;
            memory += sizeof(Bucket);
        }
        Add(counter,bucket_head);
        // Add(counter, &bucket_map[1]);
        min_num = 1;
        memory += counter_size;
        //   memory += (sizeof(DATA_TYPE) + sizeof(Counter*)+8) * Mt;
    }
}

void Summary::Init(DATA_TYPE data,int t){
    // cerr << t << endl;
    while(t - S[1]->first_timestamp + 1 > cycle){ // 每隔一个周期 删除旧的sketch
        swap(S[0],S[1]);
        delete S[1];
        S[1] = new Sketch(MAX_SIZE/2, datalen ,cycle,para_lambda,t/cycle,t);
    }
    S[1]->Init(data,t);
}

int Sketch::Query(DATA_TYPE data, int t){
    if(counter_map.find(data) == counter_map.end())
        return -1;
    else{
        Counter* temp = counter_map[data];
        while(temp->que.size() > 0){
            if(t - temp->que.front() >= cycle){
                temp->que.pop();
            }
            else break;
        }
        int nk = temp->que.size();
        return (nk-1) * para_lambda + temp->l;
    }
}

int Summary::Query(DATA_TYPE data, int t){
    return S[0]->Query(data,t) + S[1]->Query(data,t);
}

void Sketch::Connect(Counter *ca, Counter *cb){
    if(ca != NULL)
        ca->next = cb;
    if(cb != NULL)
        cb->prev = ca;
}

void Sketch::Remove(Counter* counter, Bucket* bucket){
    Connect(counter->prev, counter->next);
    if(bucket->child == counter){
        bucket->child = counter->next;
    }
}

void Sketch::Add(Counter* counter, Bucket* bucket){
    Connect(counter, bucket->child);
    bucket->child = counter;
    counter->father = bucket;
    counter->prev = NULL;
}



#endif //SLIDING_SUMMARY_H
