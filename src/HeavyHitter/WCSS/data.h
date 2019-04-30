#ifndef DATA_H
#define DATA_H

#include "definition.h"
#include "hash_class.h"

class Data {
public:
	unsigned char str[DATA_LEN];
	Data& operator = (Data an) {
		for (int i = 0;i < DATA_LEN;++i) {
			str[i] = an.str[i];
		}
		return *this;
	}
};

bool operator < (Data bn, Data an) {
	for (int i = 0;i < DATA_LEN;++i) {
		if (bn.str[i] < an.str[i])
			return true;
		else if (bn.str[i] > an.str[i])
			return false;
	}
	return false;
}

bool operator == (Data bn, Data an) {
	for (int i = 0;i < DATA_LEN;++i) {
		if (bn.str[i] != an.str[i])
			return false;
	}
	return true;
}

class My_Hash {
public:
	size_t operator()(const Data dat) const {
		return RSHash(dat.str, DATA_LEN);
	}
};

#endif // DATA_H
