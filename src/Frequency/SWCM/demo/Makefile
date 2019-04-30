main:main.o data.o hash_class.o splitter.o
	g++ -o main main.o data.o hash_class.o splitter.o
main.o:main.cpp ../data.h ../definition.h ../hash_class.h ../splitter.h
	g++ -c main.cpp
data.o:../data.cpp ../data.h ../definition.h ../hash_class.h ../splitter.h
	g++ -c ../data.cpp
hash_class.o:../hash_class.cpp ../data.h ../definition.h ../hash_class.h ../splitter.h
	g++ -c ../hash_class.cpp
splitter.o:../splitter.cpp ../data.h ../definition.h ../hash_class.h ../splitter.h
	g++ -c ../splitter.cpp
clean:
	rm -f *.o main