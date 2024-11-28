CXX		=	g++
CXXFLAGS	= -Wall -std=c++11
SOURCES = main.cpp graph.cpp util.cpp

all:		main debug
main:
		$(CXX) $(CXXFLAGS) $(SOURCES) -o PJ3 
debug:
		$(CXX) $(CXXFLAGS) $(SOURCES) -o PJ3-debug -g
clean:
		rm -f main main-debug
