# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic

queue.out : main.o queue.o
	$(CXX) $(CXXFLAGS) -o queue.out main.o queue.o

queue.o : queue.cpp queue.hpp
	$(CXX) $(CXXFLAGS) -c queue.cpp

main.o : main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# Compile using modules : support is not complete
# queue.out : main.cpp gcm.cache/Vector.gcm
# 	g++ -std=c++20 -fmodules-ts main.cpp -o queue.out
# 
# gcm.cache/Vector.gcm : queue.cpp
# 	g++ -std=c++20 -fmodules-ts -c queue.cpp
# 
# clear :
# 	rm -rf gcm.cache

# Debug target
debug: CXXFLAGS += -DDEBUG
debug: queue.out

clear :
	rm *.o queue.out
