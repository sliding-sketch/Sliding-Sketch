main:main.o
	g++ -o main main.o
main.o:main.cpp ../BOBHash32.h ../BOBHash64.h ../heavykeeper.h
	g++ -c main.cpp
clean:
	rm -f *.o main
