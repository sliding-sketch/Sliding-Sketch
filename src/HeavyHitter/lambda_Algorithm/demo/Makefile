main:data.o hash_class.o main.o summary.o
        g++ -std=c++11 -o main data.o hash_class.o main.o summary.o
data.o:../data.cpp ../data.h ../definition.h ../hash_class.h
        g++ -std=c++11 -c ../data.cpp
hash_class.o:../hash_class.cpp ../hash_class.h
        g++ -std=c++11 -c ../hash_class.cpp
summary.o:../summary.cpp ../data.h ../definition.h ../hash_class.h ../summary.h
        g++ -std=c++11 -c ../summary.cpp
main.o:main.cpp ../data.h ../definition.h ../hash_class.h ../summary.h
        g++ -std=c++11 -c main.cpp
clean:
        rm -f *.o main

