
output: main.o
	g++ main.o -o doit

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o doit