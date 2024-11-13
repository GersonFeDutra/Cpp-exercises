# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic

# Build the executable
main.out: main.o main.cpp array.hpp
	$(CXX) $(CXXFLAGS) -o main.out main.cpp

# Build object files
main.o: main.cpp array.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# Debug target
debug: CXXFLAGS += -DDEBUG
debug: main.out

# Clean target
clean:
	rm -f *.o main.out
