main:main.o sketch.o hash_class.o data.o
	g++ -o main main.o sketch.o hash_class.o data.o
main.o:main.cpp ../sketch.h ../hash_class.h ../data.h ../definition.h
	g++ -c main.cpp
sketch.o:../sketch.cpp ../sketch.h ../hash_class.h ../data.h ../definition.h
	g++ -c ../sketch.cpp
hash_class.o:../hash_class.cpp ../sketch.h ../hash_class.h ../data.h ../definition.h
	g++ -c ../hash_class.cpp
data.o:../data.cpp ../sketch.h ../hash_class.h ../data.h ../definition.h
	g++ -c ../data.cpp
clean:
	rm -f *.o main