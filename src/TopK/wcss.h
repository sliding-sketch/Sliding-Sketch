//
// Created by z yd on 2020-07-30.
//

#ifndef SLIDING_WCSS_H
#define SLIDING_WCSS_H

#include <iostream>
#include <queue>
#include "cuckoo_css.h"

using namespace std;

class wcss
{
    int wcssM;
    int W;//size of window
    cuckoo* B;
    int k;
    bool not_first_frame;
    cuckoo_CSS* ccs;
    queue<queue<DATA_TYPE> >b;

public:
    wcss(){}
    wcss(int _W, int memory, int data_len)
    {
        not_first_frame = false;
        wcssM = 0;
        W = _W;

        int memory_per_counter = 2*2 + 4 + 3*(data_len+2) + 2* data_len;
        k = memory/memory_per_counter;
        cout<<k<<endl;
        ccs = new cuckoo_CSS(k);
        B = new cuckoo(2*k);
        B->clear();
        ccs->clear();
        queue<DATA_TYPE> temp;
        b.push(temp);
    }
    //update an item
    void Init(DATA_TYPE str, int t)
    {
        wcssM = wcssM + 1;
        if (wcssM == W + W / k)
            not_first_frame = true;//从第二个frame才开始pop
        if (wcssM%W == 0)
            ccs->clear();
        if ((wcssM % W) % (W / k) == 0)
        {
            queue<DATA_TYPE> temp;
            if(not_first_frame)
                b.pop();
            b.push(temp);
        }
        if (not_first_frame&&!b.front().empty())
        {
            DATA_TYPE oldid = b.front().front();
            b.front().pop();
            B->decrease(oldid, 1);
        }
        DATA_TYPE temp = str;
        ccs->Insert(temp);
        if (ccs->myfind(temp) % (W / k) == 0)
        {
            //cout << y.myfind(temp) << ' ';
            b.back().push(temp);
            if(B->find_pos(temp)<0)
                B->insert(temp, 1);
            else
                B->increase(temp, 1);
        }
        //cout << y.myfind(temp)  << ' ';
    }
    //query an item
    int Query(DATA_TYPE str, int t)
    {
        DATA_TYPE temp = str;
        int overflow = B->find_val(temp);
        if(overflow<0)	// if not in the hash table
            overflow=0;
        return (W / k)*(overflow + 2) + ccs->myfind(temp) % (W / k);
    }
    //return estimated memory
    ~wcss()
    {
        delete ccs;
        delete B;
    }
};

#endif //SLIDING_WCSS_H
