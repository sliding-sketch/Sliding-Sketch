#ifndef _C_CSS_H
#define _C_CSS_H

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include "BOBHash32.h"
#include "params.h"
#include "cuckoo.h"
#define rep(i,a,n) for(int i=a;i<=n;i++)
using namespace std;

class cuckoo_CSS
{
public:
	int K;
	int load,last;
	cuckoo* ID_Index;
	int* R;
	int* Counter_Array;
	int* Value_Index;
	cuckoo_CSS() {}
	cuckoo_CSS(int K) :K(K)
	{
		R = new int[K];
		Counter_Array = new int[K];
		Value_Index = new int[K];
		ID_Index = new cuckoo(K);
	    clear();
    }
	void clear()
	{
		for (int i = 0; i<K; i++)
		{
			R[i] = -1;
			Value_Index[i] = 0;
			Counter_Array[i] = 0;
		}
		load = 0;
		last = K-1;
		ID_Index->clear(); 
	}

	int myfind(string x)
	{
		int id = ID_Index->find_val(x);
		if (id >= 0)
			return Counter_Array[id];
		else
			return Counter_Array[0];
	}
	void Change(int ID_pos, int current_counter)
	{
		int index = ID_pos / (ID_Index->seg_size);
		int offset = ID_pos % (ID_Index->seg_size);
		ID_Index->array[index][offset].val = current_counter;
	}
	void Insert(string x)
	{
		int p = ID_Index->find_val(x);
		if (p != -1)
		{
			int tmp = R[p];
			int Q = Counter_Array[p] + 1;
			int z = Value_Index[p];
			if (z != p)
			{
				Change(R[z], p);  // change the ID_index pointing to z to p;
				R[p] = R[z]; 
				for(int i=z-1;i>=0;i--)
				{
					if(Value_Index[i]!=z)		// change all the valu_index pointing to z to z-1;
						break;
					Value_Index[i] = z-1;
				}
			}
			//
			else if (z && Counter_Array[z - 1] == Counter_Array[z]) 
				{
				for(int i=z-1;i>=0;i--)
				{
					if(Value_Index[i]!=z)		// change all the value_index pointing to z to z-1;
						break;
					Value_Index[i] = z-1;
				}	
				}					
			if(Counter_Array[z+1]==Q)
				Value_Index[z] = Value_Index[z+1];
			else
				Value_Index[z] = z;	
			R[z] = tmp;
			Counter_Array[z] = Q;
			Change(R[z], z);
		}
		else
		{
			if(load<K)
			{
				int Q = 1;
				p = last;
				load++;
				last--;
				Counter_Array[p] = Q;
				if(p<K-1&&Counter_Array[p+1]==Q)
					Value_Index[p] = Value_Index[p+1];
				else
					Value_Index[p] = p; 
				
				map<kv, int> mp;
				ID_Index->insert(x, p, mp);
				map<kv, int>::iterator it;
				for(it=mp.begin();it!=mp.end();it++)
					R[(it->first).val] = it->second;
				mp.clear();	
			}
			else{
			ID_Index->delete_kv(R[0]); 
			int Q = Counter_Array[0] + 1;
			int z = Value_Index[0];
			if (z>0)
			{
				Change(R[z], 0);
				R[0] = R[z];
				for(int i=z-1;i>=0;i--)
				{
					if(Value_Index[i]!=z)		// change all the valu_index pointing to z to z-1;
						break;
					Value_Index[i] = z-1;
				}
			}
			if(Counter_Array[z+1]==Q)
				Value_Index[z] = Value_Index[z+1];
			else
				Value_Index[z] = z;	
			Counter_Array[z] = Q;
			map<kv, int> mp;
			ID_Index->insert(x, z, mp);
			map<kv, int>::iterator it;
			for(it=mp.begin();it!=mp.end();it++)
				R[(it->first).val] = it->second;
			mp.clear();
			}
		}
	}
};
#endif
