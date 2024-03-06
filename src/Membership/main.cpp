#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <string.h>
#include <time.h>
#include <queue>
#include <math.h>
#include <unordered_map>
#include "sliding_bloom.h"
#include "forget_bloom.h"
#include "data.h"

using namespace std;

struct Node{
    Data data;
    uint time;
};

void Error_Rate(int argc, char** argv);
void Hash_Num(int argc, char** argv);
void Insertion_Speed(int argc, char** argv);

int main(int argc, char** argv){
    //Insertion_Speed(argc, argv);
    Error_Rate(argc, argv);
    //Hash_Num(argc, argv);
}

/*
 * char* input_file = argv[0];
 * uint CYCLE = atoi(argv[1]);
 * uint memory_begin = atoi(argv[2]);
 * uint memory_interval = atoi(argv[3]);
 * uint arrival_begin = atoi(argv[4]);
 * uint arrival_interval = atoi(argv[5]);
 */

void Hash_Num(int argc, char **argv){
   // char* input_file = "../../data/formatted00.dat";
    uint CYCLE = 100000;
    uint memory_begin = 100000;
    uint memory_interval = 50000;
    uint arrival_begin = 10000000;
    uint arrival_interval = 723331;

    Node packet;
    FILE *input;
    Sliding_Bloom* slide[6];

    for(uint i = 1;i < 6;++i){
        double Slide_Err[6] = {0};
        queue<Node> que;
        unordered_map<Data, int, My_Hash> mp;

        uint memory = memory_begin + i * memory_interval;
        input = fopen("../../data/formatted00.dat","rb");
        cout << memory << endl;

        for(int hash = 1;hash < 6;++hash){
            slide[hash] = new Sliding_Bloom(ceil(memory/(double)hash), 2*hash, CYCLE);
        }

        uint num = 0;
        uint slide_err[6] = {0};
        double all = 0;
        while(fread(packet.data.str, DATA_LEN, 1, input) > 0){
            packet.time = num;

            while(!que.empty() && num - que.front().time >= CYCLE){
                mp[que.front().data] -= 1;
                que.pop();
            }
            que.push(packet);

            if(mp.find(packet.data) == mp.end())
                mp[packet.data] = 1;
            else
                mp[packet.data] += 1;

            for(int hash = 1;hash < 6;++hash){
                slide[hash]->Init(packet.data, num);
            }

            if(num > arrival_begin && num % arrival_interval == 0){
                unordered_map<Data, int, My_Hash>::iterator it;
                for(it = mp.begin();it != mp.end();++it){
                    if(it->second <= 0){
                        all += 1;
                        for(int hash = 1;hash < 6;++hash){
                            if(slide[hash]->Query(it->first, num)){
                                slide_err[hash] += 1;
                            }
                        }
                    }
                }
            }
            num++;
        }

        for(int hash = 1;hash < 6;++hash){
            Slide_Err[hash] = slide_err[hash]/all;
            cout << hash*2 << ": " << Slide_Err[hash] << endl;
        }

        for(int hash = 1;hash < 6;++hash){
            delete slide[hash];
        }
        fclose(input);
    }
}

/*
 * char* input_file = argv[0];
 * uint CYCLE = atoi(argv[1]);
 * uint SL_Hash = atoi(argv[2]);
 * uint FBF_Hash = atoi(argv[3]);
 * uint FBF_Num = atoi(argv[4]);
 * uint memory_begin = atoi(argv[5]);
 * uint memory_interval = atoi(argv[6]);
 * uint arrival_begin = atoi(argv[7]);
 * uint arrival_interval = atoi(argv[8]);
 */

void Error_Rate(int argc, char **argv){
   // char* input_file = "../../data/formatted00.dat";
    uint CYCLE = 100000;
    uint SL_Hash = 10;
    uint FBF_Hash = 8;
    uint FBF_Num = 10;
    uint memory_begin = 100000;
    uint memory_interval = 50000;
    uint arrival_begin = 10000000;
    uint arrival_interval = 723331;

    Node packet;
    FILE *input;
    Sliding_Bloom* slide;
    Forget_Bloom* forget;

    for(uint i = 1;i <= 5;++i){
        queue<Node> que;
        unordered_map<Data, int, My_Hash> mp;

        uint memory = memory_begin + i * memory_interval;
        cout << memory << endl;
        input = fopen("../../data/formatted00.dat","rb");

        slide = new Sliding_Bloom(ceil(memory / (double)SL_Hash), SL_Hash, CYCLE);
        forget = new Forget_Bloom(ceil(memory*2 / (double)FBF_Num), FBF_Hash, FBF_Num, CYCLE);

        uint num = 0;
        uint slide_err1 = 0, forget_err1 = 0;
        uint slide_err2 = 0, forget_err2 = 0;
        double all1 = 0, all2 = 0;
        while(fread(packet.data.str, DATA_LEN, 1, input) > 0){
            packet.time = num;

            while(!que.empty() && num - que.front().time >= CYCLE){
                mp[que.front().data] -= 1;
                que.pop();
            }
            que.push(packet);

            if(mp.find(packet.data) == mp.end())
                mp[packet.data] = 1;
            else
                mp[packet.data] += 1;

            slide->Init(packet.data, num);
            forget->Init(packet.data, num);

            if(num > arrival_begin && num % arrival_interval == 0){
                unordered_map<Data, int, My_Hash>::iterator it;
                for(it = mp.begin();it != mp.end();++it){
                    if(it->second > 0){
                        all1 += 1;
                        if(!slide->Query(it->first, num)){
                            slide_err1 += 1;
                        }
                        if(!forget->Query(it->first, num)){
                            forget_err1 += 1;
                        }
                    }
                    else{
                        all2 += 1;
                        if(slide->Query(it->first, num)){
                            slide_err2 += 1;
                        }
                        if(forget->Query(it->first, num)){
                            forget_err2 += 1;
                        }
                    }
                }
            }
            num++;
        }

        cout << "slide positive: " << slide_err1/all1 << endl;
        cout << "slide negative: " << slide_err2/all2 << endl;
        cout << "forget positive: " << forget_err1/all1 << endl;
        cout << "forget negative: " << forget_err2/all2 << endl;
        delete slide;
        delete forget;
        fclose(input);
    }
}

/*
 * char* input_file = argv[0];
 * uint CYCLE = atoi(argv[1]);
 * uint SL_Hash = atoi(argv[2]);
 * uint FBF_Hash = atoi(argv[3]);
 * uint FBF_Num = atoi(argv[4]);
 * uint memory_begin = atoi(argv[5]);
 * uint memory_interval = atoi(argv[6]);
 */

void Insertion_Speed(int argc, char **argv){
   // char* input_file = "../../data/formatted00.dat";
    uint CYCLE = 100000;
    uint SL_Hash = 10;
    uint FBF_Hash = 8;
    uint FBF_Num = 10;
    uint memory_begin = 100000;
    uint memory_interval = 50000;

    double duration;
    uint num = 0;
    uint start, final;
    Node packet;
    FILE *input;
    Sliding_Bloom* slide;
    Forget_Bloom* forget;

    for(uint i = 1;i < 5;++i){
        uint memory = memory_begin + i * memory_interval;
        cout << memory << endl;

        for(int j = 1;j <= 5;++j){
            slide = new Sliding_Bloom(ceil(memory / (double)SL_Hash), SL_Hash, CYCLE);
            num = 0;
            input = fopen("../../data/formatted00.dat","rb");

            start = clock();
            while(fread(packet.data.str, DATA_LEN, 1, input) > 0){
                slide->Init(packet.data, num++);
            }
            final = clock();

            fclose(input);
            duration = ((double)(final-start))/CLOCKS_PER_SEC;
            cout << (num + 0.0)/ duration / 1000000.0 << endl;
            delete slide;
        }

        for(int j = 1;j <= 5;++j){
            forget = new Forget_Bloom(ceil(memory*2 / (double)FBF_Num), FBF_Hash, FBF_Num, CYCLE);
            num = 0;
            input = fopen("../../data/formatted00.dat","rb");

            start = clock();
            while(fread(packet.data.str, DATA_LEN, 1, input) > 0){
                forget->Init(packet.data, num++);
            }
            final = clock();

            fclose(input);
            duration = ((double)(final-start))/CLOCKS_PER_SEC;
            cout << (num + 0.0)/ duration / 1000000.0 << endl;
            delete forget;
        }

    }
}


