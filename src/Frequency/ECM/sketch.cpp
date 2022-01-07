#include "sketch.h"

Counter::Counter(int _update_k, int _cycle){
    update_k = _update_k;
    cycle = _cycle;
}

void Counter::update(int time_stamp){
    element.push_back(make_pair(1, time_stamp));
    counter_delete(time_stamp);
    counter_merge(element.end()-1);
}

void Counter::decrease(int time_stamp){
    element_decrease.push_back(make_pair(1, time_stamp));
    counter_delete_d(time_stamp);
    counter_merge_d(element_decrease.end()-1);
}

void Counter::counter_delete(int time_stamp){
    for(vector<pair<int, int> >::iterator iter = element.begin(); iter != element.end();){
        if( time_stamp - iter->second >= cycle){
            iter = element.erase(iter);
        }
        else{
            break;
        }
    }
}

void Counter::counter_delete_d(int time_stamp){
    for(vector<pair<int, int> >::iterator iter = element_decrease.begin(); iter != element_decrease.end();){
        if( time_stamp - iter->second >= cycle){
            iter = element_decrease.erase(iter);
        }
        else{
            break;
        }
    }
}

void Counter::counter_merge(vector<pair<int, int> >::iterator iter){
    if(iter==element.begin()){		// if this is the first block, namely the largest and oldest block, end the procedure.
        return;
    }
    int cnt = 1;
    vector<pair<int, int> >::iterator iter_last = iter - 1;
    while(iter_last->first==iter->first)	// count the number of blocks with the same size;
    {
    	cnt++;
    	iter = iter_last;
    	if(iter_last == element.begin())
    		break;
    	iter_last = iter_last - 1;
	}
	if(cnt >= 2 + (update_k / 2)) // merge
	{
		iter = counter_merge_value(iter);	// iter is  the first (oldest) block with this size
		counter_merge(iter);
	}
}

void Counter::counter_merge_d(vector<pair<int, int> >::iterator iter){
	if(iter==element_decrease.begin()){		// if this is the first block, namely the largest and oldest block, end the procedure.
        return;
    }
    int cnt = 1;
    vector<pair<int, int> >::iterator iter_last = iter - 1;
    while(iter_last->first==iter->first)	// count the number of blocks with the same size;
    {
    	cnt++;
    	iter = iter_last;
    	if(iter_last == element_decrease.begin())
    		break;
    	iter_last = iter_last - 1;
	}
	if(cnt >= 2 + (update_k / 2)) // merge
	{
		iter = counter_merge_value_d(iter);	// iter is  the first (oldest) block with this size
		counter_merge_d(iter);
	}
    }

vector<pair<int, int> >::iterator Counter::counter_merge_value(vector<pair<int, int> >::iterator iter){
    vector<pair<int, int> >::iterator iter_1 = element.insert(iter, make_pair(iter->first * 2, (iter + 1)->second));
    iter_1++; 
    element.erase(iter_1);
    element.erase(iter_1);
    iter_1--; 
    return iter_1;
}

vector<pair<int, int> >::iterator Counter::counter_merge_value_d(vector<pair<int, int> >::iterator iter){
	 vector<pair<int, int> >::iterator iter_1 = element_decrease.insert(iter, make_pair(iter->first * 2, (iter + 1)->second));
    iter_1++; 
    element_decrease.erase(iter_1);
    element_decrease.erase(iter_1);
    iter_1--; 
    return iter_1;
}

int Counter::query(int time_stamp){
    counter_delete(time_stamp);
    vector<pair<int, int> >::iterator iter = (element.begin());
    if(element.size() == 0){
        return 0;
    }
    
    int ans;

    if(iter->first == 1){
        ans = iter->first;
    }else{
        ans = iter->first / 2;
    }
    iter ++;
    for(;iter != element.end();iter++){
        ans = ans + iter->first;
    }
    return ans;
}

int Counter::query_all(int time_stamp){

    counter_delete(time_stamp);
    vector<pair<int, int> >::iterator iter = (element.begin());
    if(element.size() == 0){
        return 0;
    }
    
    int ans;

    if(iter->first == 1){
        ans = iter->first;
    }else{
        ans = iter->first / 2;
    }
    iter ++;
    for(;iter != element.end();iter++){
        ans = ans + iter->first;
    }
    //decrease
    counter_delete_d(time_stamp);
    vector<pair<int, int> >::iterator iter_d = (element_decrease.begin());
    if(element_decrease.size() == 0){
        return 0;
    }
    
    int ans_d;

    if(iter_d->first == 1){
        ans_d = iter_d->first;
    }else{
        ans_d = iter_d->first / 2;
    }
    iter_d ++;
    for(;iter_d != element_decrease.end();iter_d++){
        ans_d = ans_d + iter_d->first;
    }

    return ans - ans_d;
}

void Counter::counter_print(){
    vector<pair<int, int> >::iterator iter = element.begin();
    for(;iter!= element.end();iter++){
        cout << iter->first << "," << iter->second << " "; 
    }
    cout << endl;

}

int Counter::q_memory(){
    int ans = element.size() * (sizeof(int) + sizeof(int) + 8);	// suppose linked by pointers, which is more efficient than a vector.
    return ans;
}

int Counter::q_memory_all(){
    return element.size() * (sizeof(int) + sizeof(int)) + element_decrease.size() * (sizeof(int) + sizeof(int));
}

ECM::ECM(int _cycle, int _update_k, int _hash_number,  int _row_length){
    cycle = _cycle;
    update_k = _update_k;
    hash_number = _hash_number;
    row_length = _row_length;
    ECM_start = new Counter*[hash_number * row_length];
    for(int i = 0; i < hash_number * row_length; i++){
        ECM_start[i] = new Counter(update_k, cycle);
    }
}

void ECM::update(const unsigned char* str, int length, int time_stamp){
    for(int i = 0; i < hash_number; i++){
        int position = (Hash(str, i, length) % row_length) + (i * row_length);
        ECM_start[position]->update(time_stamp);
    }
}

int ECM::query(const unsigned char* str, int length, int time_stamp){
    int ans = 0x7fffffff;
    for(int i = 0; i < hash_number; i++){
        int position = (Hash(str, i, length) % row_length) + (i * row_length);
        ans = min(ans, ECM_start[position]->query(time_stamp));
    }
    return ans;
}

int ECM::q_memory(int time_stamp){
    int ans = 0;
    ans = ans + hash_number * row_length * 4;
    for(int i = 0; i < hash_number * row_length; i++){
        //ECM_start[i]->counter_delete(time_stamp);
        ans = ans + ECM_start[i]->q_memory();
    }
    return ans;
}

ECU::ECU(int _cycle, int _update_k, int _hash_number, int _row_length) {
	cycle = _cycle;
	update_k = _update_k;
	hash_number = _hash_number;
	row_length = _row_length;
	ECU_start = new Counter*[hash_number * row_length];
	for (int i = 0; i < hash_number * row_length; i++) {
		ECU_start[i] = new Counter(update_k, cycle);
	}
}

void ECU::update(const unsigned char* str, int length, int time_stamp) {
	int min_value = 0x7fffffff;
	for (int i = 0; i < hash_number; i++) {
		int position = (Hash(str, i, length) % row_length) + (i * row_length);
		min_value = min(min_value, ECU_start[position]->query(time_stamp));
	}
	for (int i = 0; i < hash_number; i++) {
		int position = (Hash(str, i, length) % row_length) + (i * row_length);
		if(ECU_start[position]->query(time_stamp)==min_value)
		    ECU_start[position]->update(time_stamp);
	}
}

int ECU::query(const unsigned char* str, int length, int time_stamp) {
	int ans = 0x7fffffff;
	for (int i = 0; i < hash_number; i++) {
		int position = (Hash(str, i, length) % row_length) + (i * row_length);
		ans = min(ans, ECU_start[position]->query(time_stamp));
	}
	return ans;
}

int ECU::q_memory(int time_stamp) {
	int ans = 0;
	for (int i = 0; i < hash_number * row_length; i++) {
		ECU_start[i]->counter_delete(time_stamp);
		ans = ans + ECU_start[i]->q_memory();
	}
	return ans;
}



ECO::ECO(int _cycle, int _update_k, int _hash_number,  int _row_length){
    cycle = _cycle;
    update_k = _update_k;
    hash_number = _hash_number;
    row_length = _row_length;
    ECO_start = new Counter*[hash_number * row_length];
    for(int i = 0; i < hash_number * row_length; i++){
        ECO_start[i] = new Counter(update_k, cycle);
    }
}

static int Count_Hash[2] = {-1, 1};
void ECO::update(const unsigned char* str, int length, int time_stamp){
    for(int i = 0; i < hash_number; i++){
        unsigned int position = (Hash(str, i, length) % row_length) + (i * row_length);
        int flag = Count_Hash[(str[length - 1] + position) & 1];
        if(flag == 1){
            ECO_start[position]->update(time_stamp);
        }else{
            ECO_start[position]->decrease(time_stamp);
        }
        
    }
}

int ECO::query(const unsigned char* str, int length, int time_stamp){
    int* n = new int[hash_number];
    memset(n, 0, hash_number * sizeof(int));
    for(int i = 0; i < hash_number; i++){
        int position = (Hash(str, i, length) % row_length) + (i * row_length);
        n[i] = ECO_start[position]->query_all(time_stamp) * Count_Hash[(str[length - 1] + position) & 1];
    }

    std::sort(n, n + hash_number);

    if (((n[hash_number / 2] + n[(hash_number / 2) - 1]) / 2 ) <= 0){
        return 0;
    }

    return (n[hash_number / 2] + n[(hash_number / 2) - 1]) / 2;
}

int ECO::q_memory(int time_stamp){
    int ans = 0;
    for(int i = 0; i < hash_number * row_length; i++){
        ECO_start[i]->counter_delete(time_stamp);
        ans = ans + ECO_start[i]->q_memory_all();
    }
    return ans;
}
