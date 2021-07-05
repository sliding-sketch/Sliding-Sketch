//
// Created by z yd on 2020-07-30.
//

#ifndef SLIDING_HEAVYKEEPER_H
#define SLIDING_HEAVYKEEPER_H

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cstring>

#include "Hash.h"
#include "param.h"
#define HK_d 5
#define HK_b 1.08


using namespace std;


class heavykeeper
{
public:

    int M2;
    int cycle;
    double time_now;
    int time_pos;
    const int field_num = 4;

    struct node {int C[4] ;DATA_TYPE FP;} HK[HK_d][MAX_MEM+10];

    heavykeeper(int M2, int cycle):M2(M2),cycle(cycle) {
        for(int hk_i = 0; hk_i < HK_d; hk_i++){
            for(int hk_j = 0; hk_j < MAX_MEM + 10; hk_j ++){
                memset(HK[hk_i][hk_j].C, 0, field_num * sizeof(int));
                memset(&HK[hk_i][hk_j].FP, 0, sizeof(DATA_TYPE));
            }
        }
    }

    //update an item
    void Init(DATA_TYPE x, int time_stamp)
    {
        //用一层
        if(time_stamp > (cycle / (field_num))){
            int time_i = 0;
            for(; ((time_i + 1.0) * cycle / field_num / HK_d / M2) + time_now < time_stamp; time_i ++){
                int hki = time_pos / M2;
                int hkj = time_pos % M2;

                for(int nk = field_num - 1; nk > 0; nk--){
                    HK[hki][hkj].C[nk] = HK[hki][hkj].C[nk - 1];
                }
                HK[hki][hkj].C[0] = 0;
                time_pos ++;
                time_pos = time_pos % (M2 * HK_d);
            }
            time_now = time_now + ((time_i + 0.0) * cycle / field_num / HK_d / M2);

        }else{
            time_now = time_stamp;
        }

        for (int j=0; j<HK_d; j++)
        {
            //calculate position
            unsigned long long H=Hash(x, j);
            int Hsh=H%(M2-(2*HK_d)+2*j+3);
            //If the finger print indicates that it is the same element
            if (HK[j][Hsh].FP==x)
            {
                HK[j][Hsh].C[0]++;
            } else
                //Not the same element
            {
                int prob = 0;
                for(int prob_i = 0; prob_i <field_num; prob_i ++){
                    prob = prob + HK[j][Hsh].C[prob_i];
                }
                //probability decay
                if (!(rand()%int(pow(HK_b,prob))))
                {
                    bool flag = false;
                    for(int decay_i = 0; decay_i < field_num; decay_i ++){
                        if(HK[j][Hsh].C[decay_i] != 0){
                            HK[j][Hsh].C[decay_i] --;
                            flag = true;
                            break;
                        }
                    }
                    if (flag == false)
                    {
                        HK[j][Hsh].FP=x;
                        HK[j][Hsh].C[0]=1;
                    }
                }
            }
        }
    }
    //query an item
    int Query(DATA_TYPE x, int t){

        //int FP=(H>>48);
        int maxv = 0;
        for(int j = 0; j <HK_d; j++){
            unsigned long long H=Hash(x, j);
            int Hsh=H%(M2-(2*HK_d)+2*j+3);
            if(HK[j][Hsh].FP==x){
                int prob = 0;
                for(int prob_i = 0; prob_i <field_num; prob_i ++){
                    prob = prob + HK[j][Hsh].C[prob_i];
                }
                maxv = max(maxv, prob);
            }
        }
        return maxv;

    }

};


#endif //SLIDING_HEAVYKEEPER_H
