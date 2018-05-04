all: a.out signal

a.out: main.cpp
	g++ -Wall -g --std=c++14 main.cpp

signal: signal.cpp
	g++ -Wall -g --std=c++14 signal.cpp -o signal

clean:
	rm a.out signal