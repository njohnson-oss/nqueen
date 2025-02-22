default: nqueen

main.o: src/main.cpp
	$(CXX) -c $(CPPFLAGS) src/main.cpp -o main.o

nqueen: main.o
	$(CXX) main.o -o nqueen

.PHONY: clean
clean:
	rm -f nqueen main.o
