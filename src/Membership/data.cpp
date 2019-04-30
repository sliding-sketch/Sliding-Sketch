#include "data.h"

bool Data::operator < (const Data& an) const{
    for(int i = 0;i < DATA_LEN;++i){
        if(str[i] < an.str[i])
            return true;
        else if(str[i] > an.str[i])
            return false;
    }
    return false;
}

bool Data::operator == (const Data& an) const{
    for(int i = 0;i < DATA_LEN;++i){
        if(str[i] != an.str[i])
            return false;
    }
    return true;
}

Data& Data::operator = (const Data& an){
    for(int i = 0;i < DATA_LEN;++i){
        str[i] = an.str[i];
    }
    return *this;
}

uint Data::Hash(uint num) const{
    return Hash::BOBHash32(str, DATA_LEN, num);
}

uint My_Hash::operator()(const Data& data) const{
    return data.Hash(0);
}
