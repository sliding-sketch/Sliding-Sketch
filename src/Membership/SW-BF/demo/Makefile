main:main.o
        g++ -std=c++11 -o main main.o
main.o:main.cpp ../SWSketch.h ../BloomFilter.h ../BOBHash.h
        g++ -std=c++11 -c main.cpp
clean:
        rm -f *.o main
