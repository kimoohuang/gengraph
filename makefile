gengraph: gengraph.o graph.o
	g++  -std=gnu++11 -o  gengraph -g gengraph.o graph.o
gengraph.o: graph.h
	g++  -std=gnu++11 -c -g gengraph.cpp
graph.o:graph.h
	g++ -std=gnu++11 -c -g graph.cpp

clean:
	rm *.o gengraph 
