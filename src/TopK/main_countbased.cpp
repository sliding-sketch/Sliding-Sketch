#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <map>
#include <math.h>
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include "Hash.h"
#include "param.h"

#include "heavykeeper.h"
#include "wcss.h"
#include "summary.h"

using namespace std;
unordered_map <DATA_TYPE ,int> mp;
//map <DATA_TYPE, int> map_hk;
// #define TIMETEST
#define algo 2

//argv[1]:MEM  KB
//argv[2]:K  heavy_hitter
//argv[3]:m, number of flows
//argv[4]:cycle
//argv[5]:read_length
//argv[6]:input_file_name
//argv[7]:field_num
//argv[8]:out_model
//argv[9]:str_name
//argv[10]:output_file

void BenchOurs(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    BenchOurs(argc,argv);

    return 0;
}

void BenchOurs(int argc, char* argv[]){
    FILE* file = fopen(argv[2], "rb");

    int MEM,K;

    MEM = atoi(argv[1]);
	cout <<"file:" << argv[2] << " memory:" << MEM << endl;
    K = 200;
    int cycle = 1000000;
    DATA_TYPE* cyc_dat = new DATA_TYPE[cycle];
    int top_k = 3000;
    int m = 8000000;

    int hk_M;
    int field_num = 4;
    int single_size = sizeof(DATA_TYPE) + (4 * (field_num));

    hk_M = MEM * 1024 / (single_size * HK_d);
#if algo==1
    heavykeeper *hk;
    hk = new heavykeeper(hk_M,cycle);
#endif
#if algo==2
    wcss *hk;
    hk = new wcss(cycle, MEM * 1024, 4);
#endif
#if algo==3
    Summary *hk;
    hk = new Summary(MEM * 1024, sizeof(DATA_TYPE), cycle, K / 10);
#endif
    // double average_cr = 0;
    // double average_rr = 0;
    // double average_are = 0;
    // int average_heavy = 0;
    int out_num = 0;
    double are = 0;
    double average_recall = 0;
    double average_prec = 0;
    int average_heavy = 0;
    double average_are = 0;

    // Inserting
    int clock_ = clock();
    for (int i=0; i<=m; i++)
    {
        DATA_TYPE s;
        fread(&s, sizeof(DATA_TYPE), 1, file);
        hk->Init(s, i);
#ifndef TIMETEST
        if(i >= cycle){
            mp[cyc_dat[(i%cycle)]] --;
        }
        cyc_dat[(i%cycle)] = s;
        mp[s]++;
        


        if((i%(cycle/5)== 0)&&(i>=2*cycle)){
            are = 0;
            out_num ++;

            double recall = 0;
            double recall_ = 0;
            double real = 0;
            double real_ = 0;

            //cout << "precision rate:" << real_/real << endl;

            unordered_map<DATA_TYPE, int>::iterator it;
            vector< pair<int,DATA_TYPE> > std,my;
            std.clear();
            my.clear();
            for(it = mp.begin();it != mp.end();++it){
                if(it->second == 0) continue;
                
                int stdnum = it->second;
                int mynum = hk->Query(it->first, i);
                std.push_back(make_pair(it->second,it->first));

                my.push_back(make_pair(mynum,it->first));
            }
            sort(std.begin(),std.end());
            sort(my.begin(),my.end());
            unordered_set<DATA_TYPE> stdst;
            stdst.clear();
            for(int i=0; i< top_k && i < std.size();++i){
                stdst.insert((std.rbegin()+i)->second);
            }
            int tp = 0;
            for(int i=0; i< top_k && i < std.size();++i){
                auto it = my.rbegin() + i;
                if(stdst.find(it->second) != stdst.end()) tp ++;
                double dif = fabs((double) mp[it->second] - it->first);
                are += dif / mp[it->second];
            }

            /*
            cout << summary.memory << endl;
            cout << heavy << " " << yes << " " << no << endl;
            */
            cerr << "lambda_Algorithm,Arrivals:"<<i << ",Recall Rate:"<<(double)tp/top_k<< endl;
            cerr <<"lambda_Algorithm,Arrivals:"<<i << ",Precision Rate:"<<(double)tp/top_k<< endl; // recall rate = Precision rate
			cerr <<are / top_k <<endl;
			
            average_prec += (double)tp / top_k;
			average_recall +=(double)tp / top_k;
			average_are += are/top_k;

        }
#endif
    }
#ifndef TIMETEST
    // cout << "total average heavy hitter:" << average_heavy / out_num << endl;
    cout << "average precision rate:" << average_prec / out_num << endl;
    cout <<"average recall rate:" << average_recall / out_num << endl;
    cout <<"average are:" << average_are / out_num << endl;
#else
    double total_time = (double)(clock() - clock_)/CLOCKS_PER_SEC;
    cout << m / total_time / 1000000.0 << endl;
#endif
}