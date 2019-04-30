#ifndef WCSS_H_INCLUDED
#define WCSS_H_INCLUDED

#include<iostream>
#include<queue>
#include<string>
#include"CSS.h"
#include "BOBHash32.h"
using namespace std;

class wcss
{
	BOBHash32 *hash;
	int wcssM;
	int W;//size of frames
	int hash_num;
	int counter_per_array;
	int **B;
	int k;
	bool not_first_frame;
	CSS y;
	queue<queue<string> >b;
public:
	wcss(){}
	wcss(int _M, int _W, int _hash_num, int _counter_per_array, int _k)
	{
		not_first_frame = false;
		wcssM = _M;
		W = _W;
		hash_num = _hash_num;
		counter_per_array = _counter_per_array;
		k = _k;
		y = CSS(k, k);
		y.clear();
		B = new int*[hash_num];
		hash = new BOBHash32[hash_num];
		for (int i = 0;i<hash_num;i++)
		{
			B[i] = new int[counter_per_array];
			memset(B[i], 0, sizeof(int)*counter_per_array);
			hash[i].initialize((7*i)%MAX_PRIME32);
		}
		queue<string> temp;
		b.push(temp);
	}
	//update an item
	void add(char *str, int len)
	{
		wcssM = wcssM + 1;
		if (wcssM == W + W / k)
			not_first_frame = true;//从第二个frame才开始pop
		if (wcssM%W == 0)
			y.clear();
		if ((wcssM % W) % (W / k) == 0)
		{
			queue<string> temp;
			if(not_first_frame)
				b.pop();
			b.push(temp);
		}
		if (not_first_frame&&!b.front().empty())
		{
			string oldid = b.front().front();
			b.front().pop();
			for (int i = 0;i<hash_num;i++)
				if(B[i][hash[i].run(oldid.c_str(), 8) % counter_per_array]>0)
				B[i][hash[i].run(oldid.c_str(), 8) % counter_per_array] -= 1;
		}
		string temp = str;
		y.Insert(temp);
		if (y.myfind(temp) % (W / k) == 0)
		{
			//cout << y.myfind(temp) << ' ';
			b.back().push(temp);
			for (int i = 0;i<hash_num;i++)
			{
				B[i][hash[i].run(str, len) % counter_per_array]+=1;
				//cout << hash[i].run(str, len) % counter_per_array << endl;
			}
		}
		//cout << y.myfind(temp)  << ' ';
	}
	//query an item
	int query(char *str, int len)
	{
		int min_value = 0x7fffffff;
		for (int i = 0;i<hash_num;i++)
		{
			if (B[i][hash[i].run(str, len) % counter_per_array]<min_value)
				min_value = B[i][hash[i].run(str, len) % counter_per_array];
		}
		string temp = str;
		//min_value = max(min_value, 0);
		//cout << y.myfind(temp) % (W / k) << endl;
		return W / k*(min_value + 2) + y.myfind(temp) % (W / k);
	}
	//return estimated memory
	int querymemory()
	{
	    int sum=hash_num*counter_per_array+y.querymemory();
	    return sum;
	}
	~wcss()
	{
		delete[] hash;
		for (int i = 0;i<hash_num;++i)
			delete[] B[i];
		delete[] B;
	}
};

#endif // WCSS_H_INCLUDED
