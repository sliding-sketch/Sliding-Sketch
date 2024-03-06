#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <time.h>
#include <unordered_map>
#include <unordered_set>
#include "../sketch.cpp"
#include "../data.cpp"
#include "../hash_class.cpp"

#define NtpTest 1 

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
	int update_k = 3;
	int row_length =  15000;
	cout<<"memory of ECM approximate to 5MB, but cannot be precisely controlled"<<endl;

    // Data *dat = new Data[cycle + 1];
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
   unsigned long long int num = 0;
    int Test_num = 0;
    double CM_dif = 0;
    double CM_ae = 0;
    double CM_re = 0;
    
    ECM ecm(cycle, update_k, hash_number, row_length);

    cout <<"ECM,Arrivals,ARE"<<endl;
    int clock_ = clock();
    while(num < input_num)
    {
        packet = indat[num];
        if(num > input_num){
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
        ecm.update(packet.str, DATA_LEN, packet.timestamp);
#ifndef tpTest
        if(mp.find(packet) == mp.end())
            mp[packet] = 1;
        else
            mp[packet] += 1;
	//if(num % 10 == 0 ) cerr << num;
        //cerr << packet.timestamp << endl;
        if( packet.timestamp - indat[0].timestamp >= 2*cycle){
		
            // if(num%(cycle/10) ==0){
            if((int)packet.timestamp - latestT + 1>= cycle/5){ 
                latestT = packet.timestamp;
                st.clear();
                for(auto i = mp.begin(); i!= mp.end();i++){
			if ( i->second == 0) continue;
                    Test_num ++; 
                    CM_dif = fabs((double)ecm.query(i->first.str,DATA_LEN,packet.timestamp) - i->second );
                    CM_ae += CM_dif; 
                    CM_re += CM_dif/i->second;

                    // cerr << CM_dif << " "  << mp[dat[i]] <<endl;
                }
                
                cerr << "ECM  mem" << "," << num << "," << ecm.q_memory(num)/1024.0/1024.0 << "MB"<<endl;
                cerr << "ECM  are" << "," << num<< "," << CM_re / Test_num << endl;
                cerr << "ECM  aae" << "," << num<< "," << CM_ae / Test_num << endl;
            }
        }

#endif
        num++;

        
    }
#ifndef tpTest 
		cout<<"total "<<endl;
		cout << "ECM  mem" << "," << num << "," << ecm.q_memory(num)/1024.0/1024.0 <<"MB" <<endl;
                cout << "ECM are" << "," << num << "," << CM_re / Test_num << endl;
                cout << "ECM aae" << "," << num << "," << CM_ae / Test_num << endl;
#else
    double tp = (double)num /1000000 / ((double)(clock()-clock_)/CLOCKS_PER_SEC);
	cout << "ECM  mem" << "," << num << "," << ecm.q_memory(num)/1024.0/1024.0 << "MB" <<endl;
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
