#ifndef DATA_H
#define DATA_H

#include "definition.h"

class Data{
public:
    unsigned char str[DATA_LEN];
    Data& operator = (Data an);
};

bool operator < (Data bn, Data an);
bool operator == (Data bn, Data an);

#endif // DATA_H