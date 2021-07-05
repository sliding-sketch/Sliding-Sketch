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

#include "heavykeeper2.h"
#include "summary.h"

using namespace std;
unordered_map <DATA_TYPE ,int> mp;
//map <DATA_TYPE, int> map_hk;
// #define TIMETEST
#define algo 1

struct Packet{
    DATA_TYPE dat;
    int timestamp;
    bool operator < (const Packet&p)const{
        return timestamp < p.timestamp;
    }
};

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
	// cout <<"file:" << argv[2] << " memory:" << MEM << endl;
    K = 200;
    
    queue<Packet> cyc_dat;// = new Packet[cycle];
    int top_k = 3000;
    int m = 8000000 ;
    Packet *BUF = new Packet[m];
    fread(BUF, sizeof(Packet),m , file);
    sort(BUF,BUF+m);
    int cycle = (double)(BUF[m-1].timestamp - BUF[0].timestamp) / m * 1000000;
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
    hk = new wcss(cycle, MEM * 1024, sizeof(DATA_TYPE) );
#endif
#if algo==3
    Summary *hk;
    hk = new Summary(MEM * 1024, 4, cycle, K / 10);
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
    Packet p;
    // Inserting
    
    int latestT;
    int preT=-1;
    int firstT = BUF[0].timestamp;
    int clock_ = clock();
    for (int i=0; i<m; i++)
    {
        int timestamp = BUF[i].timestamp - firstT;
        DATA_TYPE s = BUF[i].dat;
        // fread(&p, sizeof(Packet), 1, file);
        p = BUF[i];
        // s = ;
        // timestamp = ;
        // if(firstT == -1) firstT = timestamp;
        // timestamp -= firstT;
        // if(preT > timestamp) continue;
        //cerr << timestamp << " " << preT << endl;
        // if(i%1000 == 0) cerr << i << endl;
        hk->Init(s, timestamp);
#ifndef TIMETEST
        for(;!cyc_dat.empty() && timestamp + firstT - cyc_dat.front().timestamp + 1> cycle;cyc_dat.pop()) mp[cyc_dat.front().dat] -- ;
        cyc_dat.push(p);
        mp[s]++;
        
	//if( i % 10000 == 0) cerr << i << endl;
        if(timestamp >= 2*cycle && timestamp - latestT + 1 > (cycle / 10) ){
            cerr <<"size" <<cyc_dat.size() << endl;
            latestT = timestamp;
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
                int mynum = hk->Query(it->first, timestamp);
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
		if(std.size() < top_k) cerr << "ERR" << endl;
            /*
            cout << summary.memory << endl;
            cout << heavy << " " << yes << " " << no << endl;
            */
            cerr << "lambda_Algorithm,Arrivals:"<<i << ",Recall Rate:"<<(double)tp/top_k<< endl;
            cerr <<"lambda_Algorithm,Arrivals:"<<i << ",Precision Rate:"<<(double)tp/top_k<< endl; // recall rate = Precision rate
			cerr <<"lambda_Algorithm,Arrivals:"<<i << ",ARE:"<<are / top_k <<endl;
			
            average_prec += (double)tp / top_k;
			average_recall +=(double)tp / top_k;
			average_are += are/top_k;

        }
#endif
    }
#ifndef TIMETEST
    // cout << "total average heavy hitter:" << average_heavy / out_num << endl;
    cout  << average_prec / out_num << " ";
    // cout <<"average recall rate:" << average_recall / out_num << endl;
    cout << average_are / out_num << endl;
#else
    double total_time = (double)(clock() - clock_)/CLOCKS_PER_SEC;
    cout << m / total_time / 1000000.0 << endl;
#endif
}