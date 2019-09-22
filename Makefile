CXX = g++
CXXFLAGS = -g -O3 -std=c++11 -march=native -Wall -Wextra -Werror
LINKER = g++
LFLAGS = -lcryptominisat5
OBJS = solver.o onehotbyte.o

main : main.o $(OBJS)
	$(LINKER) $(LFLAGS) -o $@ $^

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $*.o $*.cpp
