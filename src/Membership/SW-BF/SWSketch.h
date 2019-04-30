#include"BloomFilter.h"
#include<fstream>
#include<cmath>
#include<iostream>

class SlidingSketchLevel
{
private:
	int memory;
	int W;
	int cnt = 0;
	double epsilon;
	int *block_num;
	int *block_size;
	int *bf_num;
	int *bf_size;
	int level;
	int **start_time;
	BloomFilter **bf;
public:
	SlidingSketchLevel(int _W, int _memory, int _block_num, int _level = 1)
	{
		//ofstream out("C:\\Users\\12936\\Desktop\\res.txt", ios::app);
		cnt = 0;
		W = _W * 1000;
		level = _level;
		block_num = new int[level];
		bf_num = new int[level];
		block_size = new int[level];
		bf_size = new int[level];
		memory = _memory * 8000;
		epsilon = 4.0 / (2 << (memory / W));
		start_time = new int*[level];
		cout << "W=" << W << ", memory=" << memory << ", epsilon=" << epsilon << endl;
		//out << "W=" << W << ", memory=" << memory << ", epsilon=" << epsilon << endl;
		bf = new BloomFilter*[level];
		int temp = _block_num;
		for (int i = 0;i < level;i++)
		{
			block_num[i] = temp;
			bf_num[i] = temp+1;
			block_size[i] = W / temp;
			bf[i] = new BloomFilter[temp+1];
			start_time[i] = new int[temp+1];
			bf_size[i] = memory / ((temp + 1)*level);
			for (int j = 0;j < temp+1;j++)
				bf[i][j].init(bf_size[i]);
			temp = temp / 2;
			cout << "Level=" << i << ", block_num=" << block_num[i] << ", block_size=" << block_size[i] << ", bf_size=" << bf_size[i] << endl;
			//out << "Level=" << i << ", block_num=" << block_num[i] << ", block_size=" << block_size[i] << ", bf_size=" << bf_size[i] << endl;
		}
		//out.close();
	}
	void insert(char * str)
	{
		for (int i = 0;i < level;i++)
		{
			int number = (cnt / block_size[i]) % bf_num[i];
			if (cnt%block_size[i] == 0)
			{
				start_time[i][number] = cnt;
				bf[i][number].clear();
			}
			bf[i][number].insert(str);
		}
		cnt++;
	}
	bool query(char * str)
	{ 
		bool res = true, temp = false;
		for (int i = 0;i < level;i++)
		{
			temp = false;
			for (int j = 0;j < bf_num[i];j++)
				if (bf[i][j].query(str))
					temp=true;
			res &= temp;
		}
		return res;
	}
	void check()
	{
		for (int i = 0;i < level;i++)
			for (int j = 0; j < block_num[i];j++)
				if (start_time[i][j] < cnt - 100000)
					bf[i][j].clear();
		/*
		for (int i = 0;i < block_num;i++)
		{
			cout << bf[i].count() << endl;
		}
		*/
	}
};
