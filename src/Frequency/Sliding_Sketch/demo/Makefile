main:bloom.o clock.o data.o hash_class.o main.o
	g++ -o main bloom.o clock.o data.o hash_class.o main.o
bloom.o:../bloom.cpp ../bloom.h ../clock.h ../data.h ../definition.h ../hash_class.h
	g++ -c ../bloom.cpp
clock.o:../clock.cpp ../bloom.h ../clock.h ../data.h ../definition.h ../hash_class.h
	g++ -c ../clock.cpp
data.o:../data.cpp ../bloom.h ../clock.h ../data.h ../definition.h ../hash_class.h
	g++ -c ../data.cpp
hash_class.o:../hash_class.cpp ../bloom.h ../clock.h ../data.h ../definition.h ../hash_class.h
	g++ -c ../hash_class.cpp
main.o:main.cpp ../bloom.h ../clock.h ../data.h ../definition.h ../hash_class.h
	g++ -c main.cpp
clean:
	rm -f *.o main