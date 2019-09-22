CXX = g++
CXXFLAGS = -g -O3 -std=c++17 -Wpedantic -Wall -Wextra -Werror
LINKER = g++
LFLAGS = -lcryptominisat5
OBJS = Solver.o OneHotByte.o OneHotConstraint.o OneHotByteOperation.o CompressionMatrix.o

main : main.o $(OBJS)
	$(LINKER) $(LFLAGS) -o $@ $^

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $*.o $*.cpp

clean :
	rm main *.o
