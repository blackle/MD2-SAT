CXX = g++
CXXFLAGS = -g -O3 -std=c++11 -Wall -Wextra -Werror
LINKER = g++
LFLAGS = -lcryptominisat5
OBJS = Solver.o OneHotByte.o CompressionMatrix.o

main : main.o $(OBJS)
	$(LINKER) $(LFLAGS) -o $@ $^

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $*.o $*.cpp

clean :
	rm main *.o
