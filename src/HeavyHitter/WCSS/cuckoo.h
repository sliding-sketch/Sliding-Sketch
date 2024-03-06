#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<algorithm>
#include<stdlib.h>
#include "hash_class.h"
using namespace std;

struct kv
{
	string ID;
	int val;
	kv(string ID_="", int val_ = -1)
	{
		ID = ID_;
		val = val_;
	}
};

bool operator< (const kv &k1, const kv &k2)
{
	return k1.ID<k2.ID;	
}
bool operator> (const kv &k1, const kv &k2)
{
	return k1.ID>k2.ID;	
}
bool operator== (const kv &k1, const kv &k2)
{
	return (k1.ID==k2.ID)&&(k1.val==k2.val);	
}
class cuckoo
{
	public:
	kv** array;
	int hash_num;
	int size;
	int seg_size;
	int max_iter;
	cuckoo(int num, int hash_num_ = 4, int max_iter_ = 400)
	{
		size = num*1.25;
		hash_num = hash_num_;
		seg_size = size/hash_num;
		max_iter = max_iter_;
		array = new kv*[hash_num];
		for(int i=0;i<hash_num;i++)
		{ 
			array[i] = new kv[seg_size];
			for(int j=0;j<seg_size;j++)
				array[i][j] = kv(); 
		} 
	}
	~cuckoo()
	{
		for(int i=0;i<hash_num;i++)
			delete []array[i];
		delete []array;
	}
	void clear()
	{
		for(int i=0;i<hash_num;i++)
		{
			for(int j=0;j<seg_size;j++)
				array[i][j] = kv(); 
		}
	}
	void insert(string key, int val, map<kv, int> &moved);
	void insert(string key, int val);
	void reshape(map<kv, int> &moved);
	void reshape();
	int find_pos(string key);
	int find_val(string key);
	void delete_kv(int pos);
	void decrease(string key, int d); // decrease the value of key by d, end erase it if the value is 0
	void increase(string key, int d); // increase the value of key by d
	pair<kv, int> kick_iter(kv n_kv, int former_pos);
	void print_kv(int pos);
};
	
	int cuckoo::find_pos(string key)
	{
		for(int i=0;i<hash_num;i++)
		{
			int pos = Hash((const unsigned char*)key.c_str(), i+2, key.length())%seg_size;
			if(array[i][pos].ID==key)
				return (i*seg_size + pos);
		}
		return -1;
	}
	
	int cuckoo::find_val(string key)
	{
		for(int i=0;i<hash_num;i++)
		{
			int pos = Hash((const unsigned char*)key.c_str(), i+2, key.length())%seg_size;
			if(array[i][pos].ID==key)
				return array[i][pos].val;
		}
		return -1;	
	}
	void cuckoo::delete_kv(int pos)
	{
		int index = pos/seg_size;
		int address = pos%seg_size;
		array[index][address] = kv();
		return;
	} 
	
	void cuckoo::decrease(string key, int d)
	{
		for(int i=0;i<hash_num;i++)
		{
			int pos = Hash((const unsigned char*)key.c_str(), i+2, key.length())%seg_size;
			if(array[i][pos].ID==key)
			{
				array[i][pos].val -=d;
				if(array[i][pos].val<=0)
					array[i][pos] = kv();
				return;
			}
		}
	}
		void cuckoo::increase(string key, int d)
	{
		for(int i=0;i<hash_num;i++)
		{
			int pos = Hash((const unsigned char*)key.c_str(), i+2, key.length())%seg_size;
			if(array[i][pos].ID==key)
			{
				array[i][pos].val +=d;
				return;
			}
		}
	}
	
	void cuckoo::print_kv(int pos)
	{
		int index = pos/seg_size;
		int address = pos%seg_size;
		cout<<array[index][address].ID<<' '<<array[index][address].val<<endl;
		return;
	} 
	void cuckoo::reshape(map<kv, int> &moved)
	{
		vector<kv> inserted_kv;
		for(int i=0;i<hash_num;i++)
		{
			for(int j=0;j<seg_size;j++)
			inserted_kv.push_back(array[i][j]);
			delete []array[i];
		}
		size = size*1.25;
		seg_size = size/hash_num;
		for(int i=0;i<hash_num;i++)
			array[i] = new kv[seg_size];
		for(int i = 0;i<inserted_kv.size();i++)
			insert(inserted_kv[i].ID, inserted_kv[i].val, moved);
	}
	void cuckoo::reshape()
	{
		vector<kv> inserted_kv;
		for(int i=0;i<hash_num;i++)
		{
			for(int j=0;j<seg_size;j++)
			inserted_kv.push_back(array[i][j]);
			delete []array[i];
		}
		size = size*1.25;
		seg_size = size/hash_num;
		for(int i=0;i<hash_num;i++)
			array[i] = new kv[seg_size];
		for(int i = 0;i<inserted_kv.size();i++)
			insert(inserted_kv[i].ID, inserted_kv[i].val);
	}
	
	 pair<kv, int> cuckoo::kick_iter(kv n_kv, int former_address) // try to insert a kv, if not success, randomly select one position to kick, the kick position can not be the same as the former position
	// the kicked out kv and the position is returned. 
	{
		int pos[hash_num];
		int i = 0;
		for(int i=0;i<hash_num;i++)
		{
			pos[i] = Hash((const unsigned char*)n_kv.ID.c_str(), i+2, n_kv.ID.length())%seg_size;
			if(array[i][pos[i]].ID=="")
			{
				array[i][pos[i]] = n_kv;
				return make_pair(kv(), i*seg_size + pos[i]);
			}
		}
		int kick_pos = 0;
		if(former_address>=0)
		{	
		int former_pos = former_address/seg_size;
		kick_pos= (former_pos+1)%hash_num;
		}
		int address= pos[kick_pos];
		kv kicked_kv = array[kick_pos][address];
		array[kick_pos][address] = n_kv;
		return make_pair(kicked_kv, kick_pos*seg_size+address);	
	}
	
	void cuckoo::insert(string key, int val, map<kv, int> &moved)
	{
		kv n_kv(key, val);
		int pos[hash_num];
		for(int i=0;i<hash_num;i++)
		{
			pos[i] = Hash((const unsigned char*)n_kv.ID.c_str(), i+2, n_kv.ID.length())%seg_size;
			if(array[i][pos[i]].ID=="")
			{
				array[i][pos[i]] = n_kv; 
				moved[n_kv] = i*seg_size + pos[i];
				return;
			}
		}
		int address = pos[0];
		kv kicked_kv = array[0][address];
		array[0][address] = n_kv;
		moved[n_kv] = address;
		int counter = 0;
		int former_address = address;
		while(counter<max_iter)
		{
		pair<kv, int> p = kick_iter(kicked_kv, former_address);
		moved[kicked_kv] = p.second;
		if(p.first.ID=="")
			return;
		else
			{
				
				kicked_kv = p.first;
				former_address = p.second; 
			}
			counter++;
		}
		cout<<"reshape triggered!!"<<endl;
		reshape(moved);
		insert(kicked_kv.ID, kicked_kv.val, moved);
		return;
	}
	void cuckoo::insert(string key, int val)
	{
		kv n_kv(key, val);
		int pos[hash_num];
		for(int i=0;i<hash_num;i++)
		{
			pos[i] = Hash((const unsigned char*)n_kv.ID.c_str(), i+2, n_kv.ID.length())%seg_size;
			if(array[i][pos[i]].ID=="")
			{
				array[i][pos[i]] = n_kv; 
				return;
			}
		}
		int address = pos[0];
		kv kicked_kv = array[0][address];
		array[0][address] = n_kv;
		int counter = 0;
		int former_address = address;
		while(counter<max_iter)
		{
		pair<kv, int> p = kick_iter(kicked_kv, former_address);
		if(p.first.ID=="")
			return;
		else
			{
				
				kicked_kv = p.first;
				former_address = p.second; 
			}
			counter++;
		}
		cout<<"reshape triggered!!"<<endl;
		reshape();
		insert(kicked_kv.ID, kicked_kv.val);
		return;
	}
