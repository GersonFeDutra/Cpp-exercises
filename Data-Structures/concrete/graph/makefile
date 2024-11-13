../.bin/graph.out : ../.bin/graph/main.o ../.bin/utils/parser.o
	g++ -I. -o ../.bin/graph.out ../.bin/graph/main.o ../.bin/utils/parser.o

../.bin/graph/main.o : main.cpp
	g++ -c main.cpp -o ../.bin/graph/main.o

../.bin/utils/parser.o : ../utils/parser.cpp ../utils/parser.hpp
	g++ -c ../utils/parser.cpp -o ../.bin/utils/parser.o

clear :
	rm ../.bin/graph.out ../.bin/graph/*.o ../.bin/utils/*.o
