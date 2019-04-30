#include "summary.h"

Summary::Summary(int num, int max_num, int _cycle, int _para_lambda){
    min_num = 0x7fffffff;
    MAX_SIZE = num;
    MAX_NUM = max_num;
    cycle = _cycle;
    para_lambda = _para_lambda;
    bucket_map = new Bucket[MAX_NUM];
    memory = sizeof(Bucket) * MAX_NUM;
    memset(bucket_map, 0 ,sizeof(Bucket) * MAX_NUM);
    for(int i = 0;i < MAX_NUM;++i)
        bucket_map[i].value = i;
}

Summary::~Summary(){
    for(int i = 0;i < MAX_NUM;++i){
        Delete_Bucket(&bucket_map[i]);
    }
    delete []bucket_map;
}

void Summary::Delete_Bucket(Bucket *bucket){
    if(bucket->child != NULL)
        Delete_Counter(bucket->child);
    bucket->child = NULL;
}

void Summary::Delete_Counter(Counter *counter){
    counter_map.erase(counter->ID);
    memory -= (sizeof(Data) + sizeof(Counter*)) * 15;
    memory -= sizeof(Counter);
    memory -= counter->que.size() * sizeof(int) * 6;
    if(counter->next != NULL){
        Delete_Counter(counter->next);
    }
    delete counter;
}

void Summary::Init(Data data, int t){
    if(counter_map.find(data) != counter_map.end()){
        Counter* temp = counter_map[data];
        int value = temp->father->value;
        temp->l += 1;
        if(temp->l >= para_lambda){
            temp->l = 0;
            temp->que.push(t);
            memory += sizeof(int) * 6;
        }
        value += 1;
        while(temp->que.size() > 0){
            if(t - temp->que.front() >= cycle){
                temp->que.pop();
                memory -= sizeof(int) * 6;
                value -= para_lambda;
            }
            else break;
        }
        Remove(temp, temp->father);
        if(value > 0){
            Add(temp, &bucket_map[value]);
            if(value < min_num)
                min_num = value;
        }
        else{
            memory -= (sizeof(Data) + sizeof(Counter*)) * 15;
            memory -= sizeof(Counter);
            memory -= temp->que.size() * sizeof(int) * 6;

            counter_map.erase(temp->ID);
            delete temp;
        }
    }
    else{
        if(memory >= MAX_SIZE){
            for(int i = min_num;i < MAX_SIZE;++i){
                if(bucket_map[i].child != NULL){
                    Delete_Bucket(&bucket_map[i]);
                    //cout << i << endl;
                    min_num = i;
                    break;
                }
            }
        }

        Counter* counter = new Counter(data);
        counter_map[data] = counter;
        Add(counter, &bucket_map[1]);
        min_num = 1;
        memory += sizeof(Counter);
        memory += (sizeof(Data) + sizeof(Counter*)) * 15;
    }
}

int Summary::Query(Data data, int t){
    if(counter_map.find(data) == counter_map.end())
        return -1;
    else{
        Counter* temp = counter_map[data];
        int value = temp->father->value;
        while(temp->que.size() > 0){
            if(t - temp->que.front() >= cycle){
                temp->que.pop();
                memory -= sizeof(int) * 6;
                value -= para_lambda;
            }
            else break;
        }
        Remove(temp, temp->father);
        if(value > 0){
            Add(temp, &bucket_map[value]);
            if(value < min_num)
                min_num = value;
            return value;
        }
        else{
            memory -= (sizeof(Data) + sizeof(Counter*)) * 15;
            memory -= sizeof(Counter);
            memory -= temp->que.size() * sizeof(int) * 6;
            counter_map.erase(temp->ID);
            delete temp;
            return -1;
        }
    }
}

void Summary::Connect(Counter *ca, Counter *cb){
    if(ca != NULL)
        ca->next = cb;
    if(cb != NULL)
        cb->prev = ca;
}

void Summary::Remove(Counter* counter, Bucket* bucket){
    Connect(counter->prev, counter->next);
    if(bucket->child == counter){
        bucket->child = counter->next;
    }
}

void Summary::Add(Counter* counter, Bucket* bucket){
    Connect(counter, bucket->child);
    bucket->child = counter;
    counter->father = bucket;
    counter->prev = NULL;
}

