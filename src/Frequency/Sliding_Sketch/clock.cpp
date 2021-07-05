#include "clock.h"

struct Place{
    unsigned int serial;
    unsigned int pos;
};
bool operator < (Place a, Place b){
    return a.serial < b.serial;
}

int Recent_Sketch::Mid(int *num){
    if(hash_number & 1){
        return max(num[hash_number >> 1], 0);
    }
    else{
        return max(0, (num[hash_number >> 1] + num[(hash_number >> 1) - 1]) >> 1);
    }
}

Recent_Counter::Recent_Counter(int c, int l, int _row_length, int _hash_numberber, int _field_num):
    Recent_Sketch(c,l,_row_length,_hash_numberber,_field_num){
    counter = new Unit [l];
    field_num = _field_num;
    for(int i = 0; i < l; i++){
        counter[i].count = new int[_field_num];
        counter[i].field_num = _field_num;
        memset(counter[i].count, 0, _field_num * sizeof(int));
    }
}

Recent_Counter::~Recent_Counter(){
    delete [] counter;
}

void Recent_Counter::CM_Init(const unsigned char* str, int length, unsigned long long int num){
    unsigned int position;
    Clock_Go(num * step);
    for(int i = 0;i < hash_number;++i){
        position = Hash(str, i, length) % row_length + i * row_length;
        counter[position].count[(cycle_num + (position < clock_pos)) % field_num] += 1;
    }
}

void Recent_Counter::CU_Init(const unsigned char* str, int length, unsigned long long int num){
    Clock_Go(num * step);
    int k = clock_pos / row_length;
    unsigned int position = Hash(str, k ,length) % row_length + k * row_length;
    if(position < clock_pos){
        k = (k + 1) % hash_number;
        position = Hash(str, k ,length) % row_length + k * row_length;
    }

    unsigned int height = counter[position].count[(cycle_num + (position < clock_pos)) % field_num];
    counter[position].count[(cycle_num + (position < clock_pos)) % field_num] += 1;

    for(int i = (k + 1) % hash_number;i != k;i = (i + 1) % hash_number){
        position = Hash(str, i ,length) % row_length + i * row_length;
        if(counter[position].count[(cycle_num + (position < clock_pos)) % field_num] <= height){
            height = counter[position].count[(cycle_num + (position < clock_pos)) % field_num];
            counter[position].count[(cycle_num + (position < clock_pos)) % field_num] += 1;
        }
    }
}

unsigned int Recent_Counter::Query(const unsigned char* str, int length){
    unsigned int min_num = 0x7fffffff;

    for(int i = 0;i < hash_number;++i)
        min_num = min(counter[Hash(str, i, length) % row_length + i * row_length].Total(), min_num);

    return min_num;
}

static int Count_Hash[2] = {-1, 1};
void Recent_Counter::CO_Init(const unsigned char *str, int length, unsigned long long num){
    unsigned int position,countHash;
    Clock_Go(num * step);
    for(int i = 0;i < hash_number;++i){
        position = Hash(str, i, length) % row_length + i * row_length;
	countHash = Hash(str,i+1, length) & 1;
        counter[position].count[(cycle_num + (position < clock_pos)) % field_num] +=
                Count_Hash[countHash];
    }
}


int Recent_Counter::CO_Query(const unsigned char *str, int length){
    int* n = new int[hash_number];
    memset(n, 0, hash_number * sizeof(int));

    for(int i = 0;i < hash_number;++i)
    {
        unsigned int position = Hash(str, i, length) % row_length + i * row_length;
	unsigned int countHash =  Hash(str,i+1, length) & 1;
        /*
        if(clock_pos >= position){
            n[i] = (counter[position].Total() * Count_Hash[countHash ]) * row_length * hash_number / (row_length * hash_number + clock_pos - position);
        }else{
            n[i] = (counter[position].Total() * Count_Hash[countHash ]) * row_length * hash_number / (row_length * hash_number * 2 - (position - clock_pos));

        }*/
        n[i] = counter[position].Total() * Count_Hash[countHash ] ;
        
    }

    std::sort(n, n + hash_number);

    return Mid(n);
}

void Recent_Counter::Clock_Go(unsigned long long int num){
    for(;last_time < num;++last_time){
        counter[clock_pos].count[(cycle_num + 1) % field_num] = 0;
        clock_pos = (clock_pos + 1) % len;
        if(clock_pos == 0){
            cycle_num = (cycle_num + 1) % field_num;
        }
    }
}
