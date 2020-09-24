#include <iostream>
#include <fstream>
#include <memory.h>
#include <math.h>
#include <iomanip>
#include <time.h>
#include <unordered_map>
#include <stdlib.h>
#include "../summary.h"
#include "../data.h"
#include "../definition.h"

//#define TOP_SIZE 170000
//#define NUM 100000


using namespace std;
unordered_map<Data, int, My_Hash> mp;

//argv[1]:cycle
//argv[2]:input_file
//argv[3]:hitter_k
//argv[4]:input_max
//argv[5]:para_lambda
//argv[6]:output_file
//argv[7]:out_model
//argv[8]:label_name
//argv[9]:memory_para1
//argv[10]:memory_para2

void Read_File(int argc, char* argv[]){

    int cycle = 1000000;
    int hitter_k = 1000;
    int input_max = 10000000;
    int para_lambda = 100;
    Data* dat = new Data[cycle + 1];
    memset(dat, 0, (cycle + 1) * sizeof(Data));

    int memory_para = 200; // KB memory

    
    Summary summary(memory_para*1024, DATA_LEN, cycle, para_lambda);


    unsigned long long int num = 1;
    double SU_dif = 0;

    FILE* file = fopen("../../../../data/formatted00.dat","rb");
    Data packet;
    int cycle_num = 0;
    double average_recall = 0;
    double average_prec = 0;
    int average_heavy = 0;
    double are = 0;

    while(fread(packet.str, DATA_LEN, 1, file) > 0)
    {
        if(num >= input_max){
            break;
        }
        unsigned int pos = num % cycle;
        if(num >= cycle){
            mp[dat[pos]] -= 1;
        }

        dat[pos] = packet;

        summary.Init(packet, num);

        if(mp.find(packet) == mp.end())
            mp[packet] = 1;
        else
            mp[packet] += 1;

        double real = mp[packet];

        if(num % cycle == 0){
            are = 0;
            int heavy = 0;
            int yes = 0, no = 0;
            unordered_map<Data, int, My_Hash>::iterator it;
            for(it = mp.begin();it != mp.end();++it){
                bool init = false;
                if(it->second > hitter_k){
                    are = are + (fabs(it->second-summary.Query(it->first, num)) + 0.0)/it->second;
                    heavy += 1;
                    init = true;
                }
                if(summary.Query(it->first, num) > hitter_k){
                    if(init) yes += 1;
                    else no += 1;
                }
            }
            are = are/heavy;
            /*
            cout << summary.memory << endl;
            cout << heavy << " " << yes << " " << no << endl;
            */
            cout << "lambda_Algorithm,Arrivals:"<<num << ",Recall Rate:"<<(yes + 0.0)/heavy<< endl;
            if((yes + no )== 0 ){
                    cout <<"lambda_Algorithm,Arrivals:"<<num << ",Precision Rate:"<<0<< endl;
            }else{
                cout <<"lambda_Algorithm,Arrivals:"<<num << ",Precision Rate:"<<(yes + 0.0)/heavy<< endl;
            } 


            cycle_num ++;
        }

        num++;
    }

}

int main(int argc, char* argv[]){
    Read_File(argc, argv);
}
