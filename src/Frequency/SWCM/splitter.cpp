#include "splitter.h"

void Splitter::print(){
    cout << init << "," << last << "," << counter <<endl; 
}

void Bucket::print(int i1, int i2){
    // vector<Splitter> myq = q[i1 * row_length + i2];
    // for(int j =0; j < myq.size(); j++){
    //     cout << myq[j].init << "," << myq[j].last << "," << myq[j].counter <<" ";
    // }
    // cout <<"value:"<<v[i1 * row_length + i2] << endl;
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
    
    q = new deque<Splitter>[hash_number * row_length];
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
            deque<Splitter>&myq = q[i1 * row_length + i2];
            
            int &myv = v[i1 * row_length + i2];
            if(Hash(str, i1, length) % row_length == i2){
               Hash(str, i1, length) % row_length;
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
                            // deque<Splitter>::iterator iter = myq.end();
                            myq.pop_back();
                            // iter = iter - 2;
                            Splitter&pred = myq.back();
                            // myq.push_back(last);
                            if(err(pred, last) <= mu){
                                pred.counter +=  last.counter;
                                pred.init = pred.init;
                                pred.last = time_stamp;
                                pred.counter ++;
                                // myq.erase(iter);
                            }
                            else{
                                myq.push_back(last);
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
            // q[i1 * row_length + i2] = myq;
            // v[i1 * row_length + i2] = myv;
            phaseout(i1, i2, time_stamp);
    }
}


int Bucket::query(const unsigned char* str, int length){
    int my_min_num = 0x7fffffff;

    for(int i = 0;i < hash_number;++i){
        //q[Hash(str, i, length) % row_length + i * row_length].front().print();
        my_min_num = my_min(v[Hash(str, i, length) % row_length + i * row_length], my_min_num);
    }
    return my_min_num;
}



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
    deque<Splitter>& myq = q[i1 * row_length + i2];
    int& myv = v[i1 * row_length + i2];
    if(!myq.empty()){
        while(myq.front().init <= time_stamp - cycle){
            Splitter myqf = myq.front();
            if(myqf.init + (time_stamp - cycle - myqf.init + 1) > myqf.last){
                myv = myv - myqf.counter;
                // myq.erase(myq.begin());
                myq.pop_front();
                if(myq.empty()){
                    break;
                }
            }else{
                int t = time_stamp - cycle - myqf.init  +1;
                int v_prime = myqf.counter / (myqf.last - myqf.init + 1);
                myv = myv - (t*v_prime);
                myq.front().counter -= (t * v_prime);
                myq.front().init = time_stamp - cycle + 1;//= myq.front().init + 1;?
            }
        }
    }
}
