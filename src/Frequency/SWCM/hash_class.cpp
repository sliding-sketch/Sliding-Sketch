#include "hash_class.h"

unsigned int Hash(const unsigned char* str, int num, int length){
    switch(num)
    {
    case 0: return BOB1(str, length);
    case 1: return BOB4(str, length);
    case 2: return BOB2(str, length);
    case 3: return DJBHash(str, length);
    case 4: return SDBM(str, length);
    case 5: return OAAT(str, length);
    case 6: return BOB3(str, length);
    case 7: return FNV32(str, length);
    case 8: return PJWHash(str, length);
    case 9: return APHash(str, length);
    case 10: return JSHash(str, length);
    case 11: return RSHash(str, length);
    case 12: return BKDR(str, length);
    default: printf("Hash_Num Error\n"); return 0;
    }
}

unsigned int BKDR(const unsigned char* str, int len){
    unsigned int hash = 0;
    for(int i = 0;i < len;++i){
        hash += 131 * hash + str[i];
    }
    return hash;
}

unsigned int OAAT(const unsigned char *str, int len)
{
    unsigned int hash, i;
    for (hash=0, i=0; i<len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

unsigned int RSHash(const unsigned char * str, int len)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;

    for(unsigned int i = 0; i < len; i++){
        hash = hash * a + (*str++);
        a *= b;
    }
    return hash;
}

unsigned int JSHash(const unsigned char * str, int len)
{
    unsigned int hash = 1315423911;
    for(unsigned int i = 0; i < len; i++)
    {
        hash ^= ((hash << 5) + str[i] + (hash >> 2));
    }
    return hash;
}

unsigned int DJBHash (const unsigned char * str, int len)
{
    unsigned int hash = 5381;
    for(unsigned int i = 0; i < len; i++){
        hash += (hash << 5) + (*str++);
    }
    return hash;
}

unsigned int APHash(const unsigned char *str, int len)
{
    unsigned int hash = 0;

    for (unsigned int i = 0; i < len; i++)
    {
        if ((i & 1) == 0)
        {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        }
        else
        {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }

    return hash;
}

unsigned int SDBM(const unsigned char *str, int len)
{
    unsigned int hash = 0;

    for (unsigned int i = 0; i < len; i++) {
        hash = str[i] + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

unsigned int FNV32(const unsigned char *str, unsigned int len)
{
    //unsigned char *bp = (unsigned char *)str;	/* start of buffer */
    //unsigned char *be = str + len;		/* beyond end of buffer */
    unsigned int hval = 0;
    for(int i = 0; i < len; i++) {
        hval *= 0x01000193;

        //hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);

        /* xor the bottom with the current octet */
        hval ^= (unsigned int)*str++;
    }

    /* return our new hash value */
    return hval;
}

// P. J. Weinberger Hash Function
unsigned int PJWHash (const unsigned char *str, unsigned int len)
{
    unsigned int BitsInUnignedInt = (unsigned int) (sizeof (unsigned
                                int) * 8);
    unsigned int ThreeQuarters = (unsigned int) ((BitsInUnignedInt * 3) / 4);
    unsigned int OneEighth = (unsigned int) (BitsInUnignedInt / 8);
    unsigned int HighBits = (unsigned int) (0xFFFFFFFF) <<
    (BitsInUnignedInt - OneEighth);
    unsigned int hash = 0;
    unsigned int test = 0;
    for(int i = 0; i < len; i++)
    {
        hash = (hash << OneEighth) + (*str++);
        if ((test = hash & HighBits) != 0)
        {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }
    return hash ;
}

#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

unsigned int BOB4(const unsigned char * str, unsigned int len)
{
    //register ub4 a,b,c,len;
    unsigned int a,b,c;
    unsigned int initval = 127;
    /* Set up the internal state */
    //len = length;
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = initval;         /* the previous hash value */

    /*---------------------------------------- handle most of the key */
    while (len >= 12)
    {
        a += (str[0] +((unsigned int)str[1]<<8) +((unsigned int)str[2]<<16) +((unsigned int)str[3]<<24));
        b += (str[4] +((unsigned int)str[5]<<8) +((unsigned int)str[6]<<16) +((unsigned int)str[7]<<24));
        c += (str[8] +((unsigned int)str[9]<<8) +((unsigned int)str[10]<<16)+((unsigned int)str[11]<<24));
        mix(a,b,c);
        str += 12; len -= 12;
    }

    /*------------------------------------- handle the last 11 bytes */
    c += len;
    switch(len)              /* all the case statements fall through */
    {
        case 11: c+=((unsigned int)str[10]<<24);
        case 10: c+=((unsigned int)str[9]<<16);
        case 9 : c+=((unsigned int)str[8]<<8);
        /* the first byte of c is reserved for the length */
        case 8 : b+=((unsigned int)str[7]<<24);
        case 7 : b+=((unsigned int)str[6]<<16);
        case 6 : b+=((unsigned int)str[5]<<8);
        case 5 : b+=str[4];
        case 4 : a+=((unsigned int)str[3]<<24);
        case 3 : a+=((unsigned int)str[2]<<16);
        case 2 : a+=((unsigned int)str[1]<<8);
        case 1 : a+=str[0];
        /* case 0: nothing left to add */
    }
    mix(a,b,c);
    /*-------------------------------------------- report the result */
    return c;
}

unsigned int BOB1(const unsigned char * str, unsigned int len)
{
    //register ub4 a,b,c,len;
    unsigned int a,b,c;
    unsigned int initval = 2;
    /* Set up the internal state */
    //len = length;
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = initval;         /* the previous hash value */

    /*---------------------------------------- handle most of the key */
    while (len >= 12)
    {
        a += (str[0] +((unsigned int)str[1]<<8) +((unsigned int)str[2]<<16) +((unsigned int)str[3]<<24));
        b += (str[4] +((unsigned int)str[5]<<8) +((unsigned int)str[6]<<16) +((unsigned int)str[7]<<24));
        c += (str[8] +((unsigned int)str[9]<<8) +((unsigned int)str[10]<<16)+((unsigned int)str[11]<<24));
        mix(a,b,c);
        str += 12; len -= 12;
    }

    /*------------------------------------- handle the last 11 bytes */
    c += len;
    switch(len)              /* all the case statements fall through */
    {
        case 11: c+=((unsigned int)str[10]<<24);
        case 10: c+=((unsigned int)str[9]<<16);
        case 9 : c+=((unsigned int)str[8]<<8);
        /* the first byte of c is reserved for the length */
        case 8 : b+=((unsigned int)str[7]<<24);
        case 7 : b+=((unsigned int)str[6]<<16);
        case 6 : b+=((unsigned int)str[5]<<8);
        case 5 : b+=str[4];
        case 4 : a+=((unsigned int)str[3]<<24);
        case 3 : a+=((unsigned int)str[2]<<16);
        case 2 : a+=((unsigned int)str[1]<<8);
        case 1 : a+=str[0];
        /* case 0: nothing left to add */
    }
    mix(a,b,c);
    /*-------------------------------------------- report the result */
    return c;
}
unsigned int BOB2(const unsigned char * str, unsigned int len)
{
    //register ub4 a,b,c,len;
    unsigned int a,b,c;
    unsigned int initval = 31;
    /* Set up the internal state */
    //len = length;
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = initval;         /* the previous hash value */

    /*---------------------------------------- handle most of the key */
    while (len >= 12)
    {
        a += (str[0] +((unsigned int)str[1]<<8) +((unsigned int)str[2]<<16) +((unsigned int)str[3]<<24));
        b += (str[4] +((unsigned int)str[5]<<8) +((unsigned int)str[6]<<16) +((unsigned int)str[7]<<24));
        c += (str[8] +((unsigned int)str[9]<<8) +((unsigned int)str[10]<<16)+((unsigned int)str[11]<<24));
        mix(a,b,c);
        str += 12; len -= 12;
    }

    /*------------------------------------- handle the last 11 bytes */
    c += len;
    switch(len)              /* all the case statements fall through */
    {
        case 11: c+=((unsigned int)str[10]<<24);
        case 10: c+=((unsigned int)str[9]<<16);
        case 9 : c+=((unsigned int)str[8]<<8);
        /* the first byte of c is reserved for the length */
        case 8 : b+=((unsigned int)str[7]<<24);
        case 7 : b+=((unsigned int)str[6]<<16);
        case 6 : b+=((unsigned int)str[5]<<8);
        case 5 : b+=str[4];
        case 4 : a+=((unsigned int)str[3]<<24);
        case 3 : a+=((unsigned int)str[2]<<16);
        case 2 : a+=((unsigned int)str[1]<<8);
        case 1 : a+=str[0];
        /* case 0: nothing left to add */
    }
    mix(a,b,c);
    /*-------------------------------------------- report the result */
    return c;
}
unsigned int BOB3(const unsigned char * str, unsigned int len)
{
    //register ub4 a,b,c,len;
    unsigned int a,b,c;
    unsigned int initval = 73;
    /* Set up the internal state */
    //len = length;
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = initval;         /* the previous hash value */

    /*---------------------------------------- handle most of the key */
    while (len >= 12)
    {
        a += (str[0] +((unsigned int)str[1]<<8) +((unsigned int)str[2]<<16) +((unsigned int)str[3]<<24));
        b += (str[4] +((unsigned int)str[5]<<8) +((unsigned int)str[6]<<16) +((unsigned int)str[7]<<24));
        c += (str[8] +((unsigned int)str[9]<<8) +((unsigned int)str[10]<<16)+((unsigned int)str[11]<<24));
        mix(a,b,c);
        str += 12; len -= 12;
    }

    /*------------------------------------- handle the last 11 bytes */
    c += len;
    switch(len)              /* all the case statements fall through */
    {
        case 11: c+=((unsigned int)str[10]<<24);
        case 10: c+=((unsigned int)str[9]<<16);
        case 9 : c+=((unsigned int)str[8]<<8);
        /* the first byte of c is reserved for the length */
        case 8 : b+=((unsigned int)str[7]<<24);
        case 7 : b+=((unsigned int)str[6]<<16);
        case 6 : b+=((unsigned int)str[5]<<8);
        case 5 : b+=str[4];
        case 4 : a+=((unsigned int)str[3]<<24);
        case 3 : a+=((unsigned int)str[2]<<16);
        case 2 : a+=((unsigned int)str[1]<<8);
        case 1 : a+=str[0];
        /* case 0: nothing left to add */
    }
    mix(a,b,c);
    /*-------------------------------------------- report the result */
    return c;
}

