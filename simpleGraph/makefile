CXX=g++

test:test.o graph.o
	g++ -o test test.cpp graph.o
	mv *.o ./obj
	mv *.gch ./obj


graph.o:graph.cpp
	$(CXX) -c graph.cpp graph.h

clean:
	rm test
