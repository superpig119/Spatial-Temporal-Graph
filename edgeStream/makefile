CXX=g++

test:test.o edgeStream.o
	g++ -o test test.cpp edgeStream.o
	mv *.o ./obj
	mv *.gch ./obj


edgeStream.o:edgeStream.cpp
	$(CXX) -c edgeStream.cpp edgeStream.h

clean:
	rm test
