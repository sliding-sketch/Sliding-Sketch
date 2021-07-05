#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <time.h>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include "../MMap.h"
#include "../clock.cpp"
#include "../data.cpp"
#include "../hash_class.cpp"

#define tpTest 1

using namespace std;
unordered_map<Data, int, My_Hash> mp;
unordered_set<Data, My_Hash> st;

//argv[1]:cycle
//argv[2]:hash_number
//argv[3]:Memory
//argv[4]:file
//argv[5]:input_num_max
//argv[6]:out_file
//argv[7]:out_model
//argv[8]:label_name
//argv[9]:field

void Read_File(int argc,char*argv[]){
    
    int hash_number = 5;
    // cout << argv[0];
    double mymemory  = atof(argv[1]);
    cout << mymemory;
    int input_num_max = 1000000*8;
    int field_num = 3;
    int row_length = (mymemory * 1024 * 1024) / hash_number / (4 * field_num);


    // Data *dat = new Data[cycle + 1];

    unsigned long long int num = 0;
    double CM_dif = 0, CU_dif = 0, CO_dif = 0;
    double CM_ae = 0,  CU_ae = 0,  CO_ae = 0;
    double CM_re = 0,  CU_re = 0,  CO_re = 0;
    int Test_num = 0;
    LoadResult ret = Load(argv[2]);
    // FILE* file = fopen(argv[2],"rb");
    Data packet;
    Data* indat = (Data*)(ret.start);
    queue<Data> dat;

    int cycle = (indat[input_num_max -1].timestamp - indat[0].timestamp ) / 8;
    Recent_Counter CM_Counter(cycle, hash_number * row_length, row_length, hash_number, field_num);
    Recent_Counter CU_Counter(cycle, hash_number * row_length, row_length, hash_number, field_num);
    Recent_Counter CO_Counter(4*cycle/5, hash_number * row_length, row_length, hash_number, field_num);

    cout <<"Sliding Sketch,Arrivals,ARE"<<endl;
    int clock_ = clock();
    // while(fread(packet.str, DATA_LEN, 1, file) > 0)
    int latestT = indat[0].timestamp;
	cerr << cycle << endl << sizeof(Data) << endl << latestT<<endl;
    while(num < input_num_max)
    {
        packet = indat[num];
        if(num > input_num_max){
            break;
        }
#ifndef tpTest
        // unsigned int pos = num % cycle;
        while(dat.size() && packet.timestamp - dat.front().timestamp >= cycle){
            mp[dat.front()] -= 1;
            dat.pop();
        }

        dat.push(packet);
        // dat[pos] = packet;
#endif

#if !defined(tpTest)||tpTest==1
        CM_Counter.CM_Init(packet.str, DATA_LEN, packet.timestamp);
#endif
#if !defined(tpTest)||tpTest==2
        CU_Counter.CU_Init(packet.str, DATA_LEN, packet.timestamp);
#endif
#if !defined(tpTest)||tpTest==3
        CO_Counter.CO_Init(packet.str, DATA_LEN, packet.timestamp);
#endif

#ifndef tpTest
        if(mp.find(packet) == mp.end())
            mp[packet] = 1;
        else
            mp[packet] += 1;
        //cerr << packet.timestamp << endl;
        if( packet.timestamp - indat[0].timestamp >= 2*cycle){
            // if(num%(cycle/5) ==0){
            if((int)packet.timestamp - latestT + 1>= cycle/5){ 
                latestT = packet.timestamp;
                st.clear();
                for(auto i = mp.begin(); i!= mp.end();i++){
			if ( i->second == 0) continue;
                // for(int i=0;i<cycle;++i){
                    // if(st.find(dat[i]) != st.end()) continue;
                    // st.insert(dat[i]);
                    Test_num ++; 
                    CM_dif = fabs((double)CM_Counter.Query(i->first.str,DATA_LEN) - i->second );
                    CU_dif = fabs((double)CU_Counter.Query(i->first.str,DATA_LEN) - i->second);
                    CO_dif = fabs((double)CO_Counter.CO_Query(i->first.str,DATA_LEN) - i->second);
                    CM_ae += CM_dif; CU_ae += CU_dif; CO_ae += CO_dif;
                    CM_re += CM_dif/i->second; CU_re += CU_dif/i->second; CO_re += CO_dif/i->second;

                    // cerr << CM_dif << " "  << mp[dat[i]] <<endl;
                }
                
                // double tp = (double)num /1000000 / ((double)(clock()-clock_)/CLOCKS_PER_SEC);
                cerr << "Sl-CM are" << "," << num << "," << CM_re / Test_num << endl;
                cerr << "Sl-CU are" << "," << num << "," << CU_re / Test_num << endl;
                cerr << "Sl-Count are" << "," << num << "," << CO_re / Test_num << endl;
                cerr << "Sl-CM aae" << "," << num << "," << CM_ae / Test_num << endl;
                cerr << "Sl-CU aae" << "," << num << "," << CU_ae / Test_num << endl;
                cerr << "Sl-Count aae" << "," << num << "," << CO_ae / Test_num << endl;
            }
        }

#endif
        num++;

        
    }
#ifndef tpTest    
                cout << "Sl-CM are" << "," << num << "," << CM_re / Test_num << endl;
                cout << "Sl-CU are" << "," << num << "," << CU_re / Test_num << endl;
                cout << "Sl-Count are" << "," << num << "," << CO_re / Test_num << endl;
                cout << "Sl-CM aae" << "," << num << "," << CM_ae / Test_num << endl;
                cout << "Sl-CU aae" << "," << num << "," << CU_ae / Test_num << endl;
                cout << "Sl-Count aae" << "," << num << "," << CO_ae / Test_num << endl;
#else
    double tp = (double)num /1000000 / ((double)(clock()-clock_)/CLOCKS_PER_SEC);
    cout << "TP" << tpTest << "," << num << "," << tp << endl;
#endif

}

int main(int argc, char* argv[]){
    Read_File(argc, argv);
    // printf("---\n");
    // Read_File(20);
    // printf("---\n");
    // Read_File(25);
    // printf("---\n");
    // Read_File(30);
    // printf("---\n");
    // Read_File(35);
    // printf("---\n");
    // Read_File(40);
}
