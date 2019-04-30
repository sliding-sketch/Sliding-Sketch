main:data.o hash_class.o main.o summary.o
	g++ -o main data.o hash_class.o main.o summary.o
data.o:../data.cpp ../data.h ../definition.h ../hash_class.h ../summary.h
	g++ -c ../data.cpp
hash_class.o:../hash_class.cpp ../data.h ../definition.h ../hash_class.h ../summary.h
	g++ -c ../hash_class.cpp
summary.o:../summary.cpp ../data.h ../definition.h ../hash_class.h ../summary.h
	g++ -c ../summary.cpp
main.o:main.cpp ../data.h ../definition.h ../hash_class.h ../summary.h
	g++ -c main.cpp
clean:
	rm -f *.o main
