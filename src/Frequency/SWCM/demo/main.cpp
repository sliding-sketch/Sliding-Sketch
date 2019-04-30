#include <fstream>
#include <iomanip>
#include <map>
#include <math.h>
#include "../splitter.h"
#include "../data.h"

map<Data, int> mp;

//argv[1]:cycle
//argv[2]:hash_number
//argv[3]:row_length
//argv[4]:file
//argv[5]:input_num_max
//argv[6]:out_file
//argv[7]:out_model
//argv[8]:label_name
int main(int argc, char* argv[])
{
    double tau = 0.05;
    double mu = 1.5;
    int cycle = 50000;
    int hash_number = 3;
    int row_length = 2000;
    int input_num_max = 500000;
    int out_model = 2;

    Bucket cm(cycle, tau, mu, hash_number, row_length);
    //Bucket cu(cycle, tau, mu, hash_number, row_length);
    //Bucket co(cycle, tau, mu, hash_number, row_length);
    int input_num = 0;

    Data *dat = new Data[cycle];
    memset(dat, 0, cycle * sizeof(Data));

    FILE* file = fopen("../../../../data/formatted00.dat", "rb");
    Data packet;
 
    double are_cm = 0;
    double aae_cm = 0;
    double are_cu = 0;
    double aae_cu = 0;
    double are_co = 0;
    double aae_co = 0;
    cout <<"SWCM,Arrivals,ARE"<<endl;
    while(fread(packet.str, DATA_LEN, 1, file) > 0){
        if(input_num >= input_num_max){
            break;
        }
        if(input_num >= cycle){
            mp[dat[input_num % cycle]] -= 1;
        }
        dat[input_num % cycle] = packet;
        if(mp.find(packet) == mp.end()){
            mp[packet] = 1;
        }
        else{
            mp[packet] += 1;
        }
        cm.update(packet.str, DATA_LEN, input_num);
        //cu.cu_update(packet.str, DATA_LEN, input_num);
        //co.count_update(packet.str, DATA_LEN, input_num);

        double dif_cm = fabs(cm.query(packet.str, DATA_LEN) - mp[packet]);
        //double dif_cu = fabs(cu.query(packet.str, DATA_LEN) - mp[packet]);
        //double dif_co = fabs(co.count_query(packet.str, DATA_LEN) - mp[packet]);

        are_cm = are_cm + (dif_cm/mp[packet]);
        aae_cm = aae_cm + dif_cm;

        //are_cu = are_cu + (dif_cu/mp[packet]);
        //aae_cu = aae_cu + dif_cu;

        //are_co = are_co + (dif_co/mp[packet]);
        //aae_co = aae_co + dif_co;

        
        switch(out_model){
        case 1:
            if(input_num % cycle == 0 && input_num > 0){
                //fout << argv[8]<<","  << (double)input_num <<"," << (double)cm.q_memory() <<  endl;
                cout << "SWCM"<<","  << (double)input_num <<"," << (double)cm.q_memory() <<  endl;
            }
            break;
        case 2:
            if(input_num % cycle == 0 && input_num > 0){
                //fout << argv[8]<<","  << (double)input_num <<"," << are_cm/input_num <<  endl;
                cout << "SWCM"<<","  << (double)input_num <<"," << are_cm/input_num <<  endl;
            }
            break;
        case 3:
            if(input_num % cycle == 0 && input_num > 0){
                //fout << argv[8]<<","  << (double)input_num <<"," << aae_cm/input_num <<  endl;
                cout << "SWCM"<<","  << (double)input_num <<"," << aae_cm/input_num <<  endl;
            }
            break;
        
        }

        input_num ++;                       
    }

}