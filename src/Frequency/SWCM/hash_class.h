#ifndef HASH_CLASS_H
#define HASH_CLASS_H

#include "definition.h"
#include <iostream>

unsigned int Hash(const unsigned char* str, int num, int length);
unsigned int BOB1(const unsigned char * str, unsigned int len);
unsigned int BOB2(const unsigned char * str, unsigned int len);
unsigned int BOB3(const unsigned char * str, unsigned int len);
unsigned int BOB4(const unsigned char * str, unsigned int len);
unsigned int FNV32(const unsigned char *str, unsigned int len);
unsigned int PJWHash(const unsigned char *str, unsigned int len);
unsigned int BKDR(const unsigned char* str, int len);
unsigned int OAAT(const unsigned char *str, int len);
unsigned int RSHash(const unsigned char * str, int len);
unsigned int JSHash(const unsigned char * str, int len);
unsigned int DJBHash (const unsigned char * str, int len);
unsigned int APHash(const unsigned char *str, int len);
unsigned int SDBM(const unsigned char *str, int len);

#endif // HASH_CLASS_H
