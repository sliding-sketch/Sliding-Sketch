#include "splitter.h"

void Splitter::print(){
    cout << init << "," << last << "," << counter <<endl; 
}

void Bucket::print(int i1, int i2){
    vector<Splitter> myq = q[i1 * row_length + i2];
    for(int j =0; j < myq.size(); j++){
        cout << myq[j].init << "," << myq[j].last << "," << myq[j].counter <<" ";
    }
    cout <<"value:"<<v[i1 * row_length + i2] << endl;
}

double err(Splitter s1, Splitter s2){
    double new_length = my_max(fabs(s1.init - s2.last) + 1, fabs(s2.init - s1.last) + 1);
    double ori_length = my_min(fabs(s1.last - s1.init) + 1, fabs(s2.last - s2.init) + 1);
    return new_length/ori_length;
}

Bucket::Bucket(int _cycle, double _tau, double _mu, int _hash_number, int _row_length){
    cycle = _cycle;
    tau = _tau;
    mu = _mu;
    hash_number = _hash_number;
    row_length = _row_length;
    
    q = new vector<Splitter>[hash_number * row_length];
    v = new int[hash_number * row_length];
    memset(v, 0, hash_number * row_length * sizeof(int));

    //q_d = new vector<Splitter>[hash_number * row_length];
    //v_d = new int[hash_number * row_length];
    //memset(v_d, 0, hash_number * row_length * sizeof(int));
}

void Bucket::update(const unsigned char *str, int length, int time_stamp){
    for(int i1 = 0; i1 < hash_number; i1++){
        int i2 = Hash(str, i1, length) % row_length;
        //print(i1, i2);
            vector<Splitter> myq = q[i1 * row_length + i2];
            
            int myv = v[i1 * row_length + i2];
            if(Hash(str, i1, length) % row_length == i2){
                
                myv = myv + 1;
                if(myq.empty()){
                    
                    Splitter new_s;
                    new_s.counter = 1;
                    new_s.init = time_stamp;
                    new_s.last = time_stamp;
                    myq.push_back(new_s);
                }else{
                    
                    Splitter last = myq.back();
                    if(last.counter < (tau * cycle / row_length)){
                        myq.back().counter ++;
                        myq.back().last = time_stamp;
                    }
                    else{
                        if(myq.size() >= 2){
                            vector<Splitter>::iterator iter = myq.end();
                            iter = iter - 2;
                            Splitter pred = *iter;
                            if(err(pred, last) <= mu){
                                myq.back().counter = myq.back().counter + pred.counter;
                                myq.back().init = pred.init;
                                myq.back().last = time_stamp;
                                myq.back().counter ++;
                                myq.erase(iter);
                            }
                            else{
                                Splitter new_s;
                                new_s.counter = 1;
                                new_s.init = time_stamp;
                                new_s.last = time_stamp;
                                myq.push_back(new_s);
                            }
                        }else{
                            Splitter new_s;
                            new_s.counter = 1;
                            new_s.init = time_stamp;
                            new_s.last = time_stamp;
                            myq.push_back(new_s);
                        }
                    }
                }
            }
            q[i1 * row_length + i2] = myq;
            v[i1 * row_length + i2] = myv;
            phaseout(i1, i2, time_stamp);
    }
}
/*
void Bucket::cu_update(const unsigned char* str, int length, int time_stamp){
    int my_min_v = 0x7fffffff;
    for(int i = 0; i < hash_number;i ++){
        my_min_v = my_min(my_min_v, v[(Hash(str, i, length) % row_length) + i * row_length]);
    }
    for(int i1 = 0; i1 < hash_number; i1++){
        for(int i2 = 0; i2 < row_length; i2++){
            vector<Splitter> myq = q[i1 * row_length + i2];
            int myv = v[i1 * row_length + i2];
            if(!myq.empty()){
                if(myq.front().init == time_stamp - cycle){
                    Splitter first = myq.front();
                    int v_prime = first.counter / (first.last - first.init + 1);
                    myv = myv - v_prime;
                    first.counter = first.counter - v_prime;
                    first.init = first.init + 1;
                    if(first.init > first.last){
                        myq.erase(myq.begin());
                    }else{
                        myq.front().counter = first.counter;
                        myq.front().init = first.init;
                        myq.front().last = first.last;
                    }
                }
                v[i1 * row_length + i2] = myv;
            }

            if( (Hash(str, i1, length) % row_length == i2) && (v[i1 * row_length + i2] == my_min_v) ){
                myv = myv + 1;
                if(myq.empty()){
                    Splitter new_s;
                    new_s.counter = 1;
                    new_s.init = time_stamp;
                    new_s.last = time_stamp;
                    myq.push_back(new_s);
                }else{
                    Splitter last = myq.back();
                    if(last.counter < (tau * cycle / row_length)){
                        myq.back().counter ++;
                        myq.back().last = time_stamp;
                    }
                    else{
                        if(myq.size() >= 2){
                            vector<Splitter>::iterator iter = myq.end();
                            iter = iter - 2;
                            Splitter pred = *iter;
                            if(err(pred, last) <= mu){
                                myq.back().counter = myq.back().counter + pred.counter;
                                myq.back().init = pred.init;
                                myq.back().last = time_stamp;
                                myq.back().counter ++;
                                myq.erase(iter);
                            }
                            else{
                                Splitter new_s;
                                new_s.counter = 1;
                                new_s.init = time_stamp;
                                new_s.last = time_stamp;
                                myq.push_back(new_s);
                            }
                        }else{
                            Splitter new_s;
                            new_s.counter = 1;
                            new_s.init = time_stamp;
                            new_s.last = time_stamp;
                            myq.push_back(new_s);
                        }
                    }
                }
            }
            q[i1 * row_length + i2] = myq;
            v[i1 * row_length + i2] = myv;
        }
    }
}*/

/*
static int Count_Hash[2] = {-1, 1};

void Bucket::count_update(const unsigned char* str, int length, int time_stamp){
    //positive
    for(int i1 = 0; i1 < hash_number; i1++){
        for(int i2 = 0; i2 < row_length; i2++){
            vector<Splitter> myq = q[i1 * row_length + i2];
            int myv = v[i1 * row_length + i2];
            if(!myq.empty()){
                if(myq.front().init == time_stamp - cycle){
                    Splitter first = myq.front();
                    int v_prime = first.counter / (first.last - first.init + 1);
                    myv = myv - v_prime;
                    first.counter = first.counter - v_prime;
                    first.init = first.init + 1;
                    if(first.init > first.last){
                        myq.erase(myq.begin());
                    }else{
                        myq.front().counter = first.counter;
                        myq.front().init = first.init;
                        myq.front().last = first.last;
                    }
                }
                v[i1 * row_length + i2] = myv;
            }

            if(Hash(str, i1, length) % row_length == i2){
                int position = Hash(str, i1, length) % row_length + i1 * row_length;
                if(Count_Hash[(str[length - 1] + position) & 1] == 1){
                myv = myv + 1;
                if(myq.empty()){
                    Splitter new_s;
                    new_s.counter = 1;
                    new_s.init = time_stamp;
                    new_s.last = time_stamp;
                    myq.push_back(new_s);
                }else{
                    Splitter last = myq.back();
                    if(last.counter < (tau * cycle / row_length)){
                        myq.back().counter ++;
                        myq.back().last = time_stamp;
                    }
                    else{
                        if(myq.size() >= 2){
                            vector<Splitter>::iterator iter = myq.end();
                            iter = iter - 2;
                            Splitter pred = *iter;
                            if(err(pred, last) <= mu){
                                myq.back().counter = myq.back().counter + pred.counter;
                                myq.back().init = pred.init;
                                myq.back().last = time_stamp;
                                myq.back().counter ++;
                                myq.erase(iter);
                            }
                            else{
                                Splitter new_s;
                                new_s.counter = 1;
                                new_s.init = time_stamp;
                                new_s.last = time_stamp;
                                myq.push_back(new_s);
                            }
                        }else{
                            Splitter new_s;
                            new_s.counter = 1;
                            new_s.init = time_stamp;
                            new_s.last = time_stamp;
                            myq.push_back(new_s);
                        }
                    }
                }
            }
            }
            q[i1 * row_length + i2] = myq;
            v[i1 * row_length + i2] = myv;
        }
    }

    
    //negative
    for(int i1 = 0; i1 < hash_number; i1++){
        for(int i2 = 0; i2 < row_length; i2++){
            vector<Splitter> myq = q_d[i1 * row_length + i2];
            int myv = v_d[i1 * row_length + i2];
            if(!myq.empty()){
                if(myq.front().init == time_stamp - cycle){
                    Splitter first = myq.front();
                    int v_prime = first.counter / (first.last - first.init + 1);
                    myv = myv - v_prime;
                    first.counter = first.counter - v_prime;
                    first.init = first.init + 1;
                    if(first.init > first.last){
                        myq.erase(myq.begin());
                    }else{
                        myq.front().counter = first.counter;
                        myq.front().init = first.init;
                        myq.front().last = first.last;
                    }
                }
                v_d[i1 * row_length + i2] = myv;
            }
            if(Hash(str, i1, length) % row_length == i2){
                int position = Hash(str, i1, length) % row_length + i1 * row_length;
                if(Count_Hash[(str[length - 1] + position) & 1] == -1){
                myv = myv + 1;
                if(myq.empty()){
                    Splitter new_s;
                    new_s.counter = 1;
                    new_s.init = time_stamp;
                    new_s.last = time_stamp;
                    myq.push_back(new_s);
                }else{
                    Splitter last = myq.back();
                    if(last.counter < (tau * cycle / row_length)){
                        myq.back().counter ++;
                        myq.back().last = time_stamp;
                    }
                    else{
                        if(myq.size() >= 2){
                            vector<Splitter>::iterator iter = myq.end();
                            iter = iter - 2;
                            Splitter pred = *iter;
                            if(err(pred, last) <= mu){
                                myq.back().counter = myq.back().counter + pred.counter;
                                myq.back().init = pred.init;
                                myq.back().last = time_stamp;
                                myq.back().counter ++;
                                myq.erase(iter);
                            }
                            else{
                                Splitter new_s;
                                new_s.counter = 1;
                                new_s.init = time_stamp;
                                new_s.last = time_stamp;
                                myq.push_back(new_s);
                            }
                        }else{
                            Splitter new_s;
                            new_s.counter = 1;
                            new_s.init = time_stamp;
                            new_s.last = time_stamp;
                            myq.push_back(new_s);
                        }
                    }
                }
            }
            }
            q_d[i1 * row_length + i2] = myq;
            v_d[i1 * row_length + i2] = myv;
        }
    }



}*/


int Bucket::query(const unsigned char* str, int length){
    int my_min_num = 0x7fffffff;

    for(int i = 0;i < hash_number;++i){
        //q[Hash(str, i, length) % row_length + i * row_length].front().print();
        my_min_num = my_min(v[Hash(str, i, length) % row_length + i * row_length], my_min_num);
    }
    return my_min_num;
}

/*
int Bucket::count_query(const unsigned char* str, int length){
    int* n = new int[hash_number];
    memset(n, 0, hash_number * sizeof(unsigned int));

    for(int i = 0;i < hash_number;++i){
        int position = Hash(str, i, length) % row_length + i * row_length;
        n[i] = (v[position] - v_d[position]) * Count_Hash[(str[length - 1] + position) & 1];
        //n[i] = (counter[position].future + counter[position].now) * Count_Hash[(str[length - 1] + position) & 1];
    }

    std::sort(n, n + hash_number);

    if (((n[hash_number / 2] + n[(hash_number / 2) - 1]) / 2 ) <= 0){
        return 0;
    }

    return (n[hash_number / 2] + n[(hash_number / 2) - 1]) / 2;

}*/


int Bucket::q_memory(){
    int ans = 0;
    ans = ans + row_length * hash_number * 4;
    for(int i = 0; i < hash_number; i++){
        for(int j = 0; j < row_length; j++){
            ans = ans + q[i * row_length + j].size() * (sizeof(Splitter) + 4) + sizeof(int);
        }
    }
    return ans;
}

void Bucket::phaseout(int i1, int i2, int time_stamp){
    vector<Splitter>& myq = q[i1 * row_length + i2];
    int& myv = v[i1 * row_length + i2];
    if(!myq.empty()){
        while(myq.front().init <= time_stamp - cycle){
            if(myq.front().init + (time_stamp - cycle - myq.front().init + 1) > myq.front().last){
                myv = myv - myq.front().counter;
                myq.erase(myq.begin());
                if(myq.empty()){
                    break;
                }
            }else{
                int t = time_stamp - cycle - myq.front().init  +1;
                int v_prime = myq.front().counter / (myq.front().last - myq.front().init + 1);
                myv = myv - (t*v_prime);
                myq.front().counter = myq.front().counter - (t * v_prime);
                myq.front().init = myq.front().init + 1;
            }
        }
    }
}
