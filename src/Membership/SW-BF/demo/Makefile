main:main.o
	g++ -o main main.o
main.o:main.cpp ../SWSketch.h ../BloomFilter.h ../BOBHash.h 
	g++ -c main.cpp
clean:
	rm -f *.o main
