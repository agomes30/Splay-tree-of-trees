CXX = g++
CXXFLAGS = -Wall

driver: BST.o Scanner.o scanTest.cpp
	$(CXX) $(CXXFLAGS) BST.o Scanner.o scanTest.cpp -o driver

Scanner.o: Scanner.h Scanner.cpp
	$(CXX) $(CXXFLAGS) -c Scanner.cpp

BST.o: BST.h BST.cpp
	$(CXX) $(CXXFLAGS) -c BST.cpp

run:
	./driver

val:
	valgrind ./driver
