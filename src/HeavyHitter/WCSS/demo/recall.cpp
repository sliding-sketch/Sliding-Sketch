#define _CRT_SECURE_NO_DEPRECATE
#include <fstream>
#include<iostream>
#include <iomanip>
#include <math.h>
#include <map>
#include <math.h>
#include "../wcss.h"
#include "../data.h"
using namespace std;

map<Data, int> mp,mp1,mp2;

//argv[1]:cycle
//argv[2]:hash_number
//argv[3]:memory(KB)
//argv[4]:input_num_max
//argv[5]:k
//argv[6]::thre
//argv[7]:in_file
//argv[8]:out_file
int main(int argc,char ** argv)
{

	int jindu = 0;

	int cycle = 1000000;
	int hash_number = 3;
	//int row_length = atoi(argv[3]);
	double mymemory=200;
	int input_num_max = 10000000;
	int k = 10000;
	int thre = 1000;
	const char * in_file = "../../../../data/formatted00.dat";
	mp.clear();
	int input_num = 0;
	ofstream fout;
	Data *dat = new Data[cycle];
	memset(dat, 0, cycle * sizeof(Data));

	FILE* file = fopen(in_file, "rb");
	Data packet;
	wcss cm(cycle, mymemory*1024, DATA_LEN);
	double real = 0, estiall = 0, estireal = 0;
	double are=0;
	while (fread(packet.str, DATA_LEN, 1, file) > 0) {
		if (input_num >= input_num_max) {
			break;
		}
		if (input_num >= cycle) {
			mp[dat[input_num % cycle]] -= 1;
		}
		dat[input_num % cycle] = packet;
		if (mp.find(packet) == mp.end()) {
			mp[packet] = 1;
		}
		else {
			mp[packet] += 1;
		}

		cm.add((char *)packet.str, DATA_LEN);


		if (input_num % cycle == 0 && input_num > 0) {
			are=0;
			real = 0;
			estiall = 0;
			estireal = 0;
			mp1.clear();
			mp2.clear();
			for (int i = 0;i < cycle;i++)
			{
				if (mp[dat[i]] > thre&&mp1.find(dat[i]) == mp1.end())
				{
					real++;
					are=are + (fabs(mp[dat[i]]-cm.query((char *)dat[i].str, DATA_LEN))+0.0)/cm.query((char *)dat[i].str, DATA_LEN);
					//cout << mp[dat[i]] << ' ';
					mp1[dat[i]] = mp[dat[i]];
					//cout << cm.query((char *)dat[i].str, DATA_LEN) << endl;
					if (cm.query((char *)dat[i].str, DATA_LEN) > thre)
					{
						estireal++;
					}
				}
				if (cm.query((char *)dat[i].str, DATA_LEN) > thre&&mp2.find(dat[i]) == mp2.end())
				{
					estiall++;
					mp2[dat[i]] = cm.query((char *)dat[i].str, DATA_LEN);
				}
			}
			are=are/real;

			cout << "WCSS,Arrivals:"<< input_num <<",Recall Rate:" << estireal/real << endl;
		}

		input_num++;
	}
	delete []dat;
	fclose(file);

	//fout << endl;

	return 0;
}
