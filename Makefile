all: main

main: src/main.cpp
	g++ src/main.cpp

clean:
	rm a.out
