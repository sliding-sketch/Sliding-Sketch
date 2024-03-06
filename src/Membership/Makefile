main:hash.o basic.o bitset.o data.o bloom.o forget_bloom.o sliding_bloom.o main.o
        g++ -std=c++11 -o main hash.o data.o basic.o bitset.o bloom.o forget_bloom.o sliding_bloom.o main.o
hash.o:hash.cpp hash.h definition.h
        g++ -std=c++11 -g -c hash.cpp
basic.o:basic.cpp basic.h definition.h
        g++ -std=c++11 -g -c basic.cpp
bitset.o:bitset.cpp bitset.h definition.h
        g++ -std=c++11 -g -c bitset.cpp
data.o:data.cpp data.h hash.h definition.h
        g++ -std=c++11 -g -c data.cpp
bloom.o:bloom.cpp bloom.h basic.h data.h bitset.h hash.h
        g++ -std=c++11 -g -c bloom.cpp
forget_bloom.o:forget_bloom.cpp forget_bloom.h bloom.h basic.h data.h bitset.h hash.h
        g++ -std=c++11 -g -c forget_bloom.cpp
sliding_bloom.o:sliding_bloom.cpp sliding_bloom.h bloom.h basic.h data.h bitset.h hash.h
        g++ -std=c++11 -g -c sliding_bloom.cpp
main.o:main.cpp definition.h hash.h basic.h bitset.h data.h bloom.h forget_bloom.h sliding_bloom.h
        g++ -std=c++11 -g -c main.cpp
clean:
        rm -f *.o main
