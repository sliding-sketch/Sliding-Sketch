#include<iostream>
#include<set>
#include<time.h>
#include<string>
#include<fstream>
#include<cstdlib>
#include<stdio.h>
#include<cmath>
#include<stdlib.h>
#include"../SWSketch.h"
using namespace std;

set<string> ss;
char packet[500000][10] = { 0 };
int memory = 200;

void speed()
{
	ss.clear();
	memset(packet, 0, sizeof(packet));
	FILE* file = fopen("../../../../data/formatted00.dat", "rb");
	SlidingSketchLevel my(100, memory, 16, 2);
	int cnt = 0;
	double start = clock(), end = 0;
	while (cnt < 500000)
	{
		//read a packet
		fread(packet[cnt], 8, 1, file);
		/*
		if (ss.count(packet[cnt]) > 0)
		{
			memset(packet[cnt], 0, sizeof(char) * 10);
			continue;
		}
		*/
		//ss.insert(packet[cnt]);
		//cout << packet[cnt] << endl;
		//read a packet
		my.insert(packet[cnt]);
		cnt++;
	}
	end = clock();
	double duration = (end - start) / CLOCKS_PER_SEC;
	cout << (cnt + 0.0) / duration / 1000000 << endl;
	cout << start<<' '<<end<<' '<<CLOCKS_PER_SEC<<' '<<duration << endl;
	cout<<"SW-BF,"<<memory<<',' << (cnt + 0.0) / duration / 1000000;
	fclose(file);
}

void errorrate()
{
	ss.clear();
	memset(packet, 0, sizeof(packet));
	FILE* file = fopen("../../../../data/formatted00.dat", "rb");
	SlidingSketchLevel my(100, memory, 32, 2);
	int cnt = 0;
	double wrong = 0;
	while (cnt < 500000)
	{
		//read a packet
		fread(packet[cnt], 8, 1, file);
		//if (ss.count(packet[cnt]) > 0)
		//{
		//	memset(packet[cnt], 0, sizeof(char) * 10);
		//	continue;
		//}
		ss.insert(packet[cnt]);
		//cout << packet[cnt] << endl;
		//read a packet
		my.insert(packet[cnt]);
		if (cnt>100000 && cnt % 20000 == 0)
		{
			my.check();

			for (int i = cnt - 100000;i <= cnt;i++)
			{
				if (!my.query(packet[i]))
					wrong++;
			}
			cout << wrong << ' ';
			char temp[10] = { 0 };
			for (int i = 0;i < 100000;i++)
			{
				//itoa(i, temp, 10);
				sprintf(temp,"%d",i);
				if (ss.count(temp) == 0)
					if (my.query(temp))
						wrong++;
			}
			cout << wrong << endl;
			if (cnt % 100000 == 0)
			{
				if (cnt > 300000)
				{
					cout << "SW-BF," << memory << ',';
					cout << wrong / 1000000;
					break;
				}
				cout << "-----------------" << endl << wrong << ' ';
				cout << wrong / 1000000 << endl;
				wrong = 0;
			}
		}
		cnt++;
	}
}

int main()
{
	/*
	for (;memory <= 300;memory += 100)
		errorrate();
	*/
	for (;memory <= 500;memory += 100)
		for (int i = 0;i < 5;i++)
			speed();

	return 0;
}
