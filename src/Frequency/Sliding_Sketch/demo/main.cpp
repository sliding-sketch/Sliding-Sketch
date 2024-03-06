#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <time.h>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include "../clock.cpp"
#include "../data.cpp"
#include "../hash_class.cpp"

//#define tpTest 1   // when set this tag, the program will run speed test, where it will only process items with a certain algorithm but will not stop and compute error at checkpoints

using namespace std;
unordered_map<Data, int, My_Hash> mp;
unordered_set<Data, My_Hash> st;


void Read_File(int argc,char*argv[]){
    
    int hash_number = 5;
    double mymemory  = 5; // size of memory usage, use unit MB
    cout <<"memory "<<mymemory<<" MB"<<endl;;
    int field_num = 3; // number of field in each bucket
    int row_length = (mymemory * 1024 * 1024) / hash_number / (4 * field_num);



    unsigned long long int num = 0;
    int Test_num = 0;
    double CM_dif = 0, CU_dif = 0, CO_dif = 0;
    double CM_ae = 0,  CU_ae = 0,  CO_ae = 0;
    double CM_re = 0,  CU_re = 0,  CO_re = 0;
    
	FILE* file = fopen("../../../../data/formatted00.dat","rb");
    Data packet;
    vector<Data> indat;
    unsigned int latestT = 0;
    while(fread(packet.str, DATA_LEN, 1, file) > 0)
    {
    	packet.timestamp = indat.size(); // as the dataset provided in the demo has no timestamp, we use count-based sliding window. In other words, each item will get a timestamp equal to its index in the datasteam, and the length of the
		// sliding window is equal to the number of items in it. 
    	indat.push_back(packet);
	}
	unsigned int input_num = indat.size();
    queue<Data> dat;

    int cycle = 200000; // the length of the sliding window, here we use count based sliding window. 
    cout<<"window size "<<cycle<<endl;
    
    Recent_Counter CM_Counter(cycle, hash_number * row_length, row_length, hash_number, field_num);
    Recent_Counter CU_Counter(cycle, hash_number * row_length, row_length, hash_number, field_num);
    Recent_Counter CO_Counter(4*cycle/5, hash_number * row_length, row_length, hash_number, field_num);

    cout <<"Sliding Sketch,Arrivals,ARE"<<endl;
    int clock_ = clock();
    while(num < input_num)
    {
        packet = indat[num];
#ifndef tpTest
        while(dat.size() && packet.timestamp - dat.front().timestamp >= cycle){
            mp[dat.front()] -= 1;   // expiration of the eldest item in the sliding window
            dat.pop();
        }

        dat.push(packet);
#endif

#if !defined(tpTest)||tpTest==1
        CM_Counter.CM_Init(packet.str, DATA_LEN, num);
#endif
#if !defined(tpTest)||tpTest==2
        CU_Counter.CU_Init(packet.str, DATA_LEN, num);
#endif
#if !defined(tpTest)||tpTest==3
        CO_Counter.CO_Init(packet.str, DATA_LEN, num);
#endif

#ifndef tpTest
        if(mp.find(packet) == mp.end())
            mp[packet] = 1;
        else
            mp[packet] += 1;

        if( packet.timestamp - indat[0].timestamp >= 2*cycle){
            if((int)packet.timestamp - latestT + 1>= cycle/5){ 
                latestT = packet.timestamp;
                st.clear();
                for(auto i = mp.begin(); i!= mp.end();i++){
					if ( i->second == 0) continue;
                    Test_num ++; 
                    CM_dif = fabs((double)CM_Counter.Query(i->first.str,DATA_LEN) - i->second );
                    CU_dif = fabs((double)CU_Counter.Query(i->first.str,DATA_LEN) - i->second);
                    CO_dif = fabs((double)CO_Counter.CO_Query(i->first.str,DATA_LEN) - i->second);
                    CM_ae += CM_dif; CU_ae += CU_dif; CO_ae += CO_dif;
                    CM_re += CM_dif/i->second; CU_re += CU_dif/i->second; CO_re += CO_dif/i->second;

                }
                
                // double tp = (double)num /1000000 / ((double)(clock()-clock_)/CLOCKS_PER_SEC);
                cerr << "Sl-CM are" << "," << num << "," << CM_re / Test_num << endl;
                cerr << "Sl-CU are" << "," << num << "," << CU_re / Test_num << endl;
                cerr << "Sl-Count are" << "," << num << "," << CO_re / Test_num << endl;
                cerr << "Sl-CM aae" << "," << num << "," << CM_ae / Test_num << endl;
                cerr << "Sl-CU aae" << "," << num << "," << CU_ae / Test_num << endl;
                cerr << "Sl-Count aae" << "," << num << "," << CO_ae / Test_num << endl;
                cerr <<endl;
            }
        }

#endif
        num++;

        
    }
#ifndef tpTest   
				cout<<"Total"<<endl; 
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
