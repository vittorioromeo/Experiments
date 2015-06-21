CXX = clang++
CXX_FLAGS = -std=c++14 -Ofast

default: program

t1.o: shared.hpp
	$(CXX) $(CXX_FLAGS) -c t1.cpp -o t1.o

t2.o: shared.hpp
	$(CXX) $(CXX_FLAGS) -c t2.cpp -o t2.o

program: t1.o t2.o shared.hpp
	$(CXX) $(CXX_FLAGS) main.cpp t1.o t2.o -o program

clean:
	rm ./t1.o
	rm ./t2.o
	rm ./program