#ifndef _heavykeeper_H
#define _heavykeeper_H

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include "BOBHash32.h"
#include "params.h"
#include "BOBHash64.h"
#define HK_d 8
#define HK_b 1.08
#define rep(i,a,n) for(int i=a;i<=n;i++)
using namespace std;
class heavykeeper
{
    public:
        struct node {int* C ;string FP;} HK[HK_d][MAX_MEM+10];
        BOBHash64 * bobhash;
        int M2;
        int cycle;
        double time_now;
        int time_pos;
        int field_num;
        heavykeeper(int M2, int cycle, int field_num):M2(M2),cycle(cycle),field_num(field_num) {
            //bobhash=new BOBHash64(1005);time_now=0;time_pos=0;
            for(int hk_i = 0; hk_i < HK_d; hk_i++){
                for(int hk_j = 0; hk_j < MAX_MEM + 10; hk_j ++){
                    HK[hk_i][hk_j].C = new int[field_num];
                    memset(HK[hk_i][hk_j].C, 0, field_num * sizeof(int));
                }
            }
        }

        void clear()
        {
            for (int i=0; i<HK_d; i++)
                for (int j=0; j<=M2+5; j++)
                    for(int k = 0; k < field_num;k++)
                        {
                            HK[i][j].C[k]=0;
                            HK[i][j].FP.clear();
                        }
        }
        //hash function
        unsigned long long Hash(int j, string ST)
        {   
            bobhash = new BOBHash64(j + 10);
            unsigned long long ans = (bobhash->run(ST.c_str(),ST.size()));
            delete bobhash;

            return ans;
            
        }

        //update an item
        void Insert(string x, int time_stamp)
        {
            //用一层
            if(time_stamp > (cycle / (field_num))){
                int time_i = 0;
                for(; ((time_i + 1) * cycle / field_num / HK_d / M2) + time_now < time_stamp; time_i ++){
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

            //calculate finger print
            
            //int FP=(H>>48);
            //HK_d hash
            for (int j=0; j<HK_d; j++)
            {
                //calculate position
                unsigned long long H=Hash(j, x); 
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
        int num_query(string x){
            
            //int FP=(H>>48);
            int maxv = 0;
            for(int j = 0; j <HK_d; j++){
                unsigned long long H=Hash(j, x); 
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
#endif
