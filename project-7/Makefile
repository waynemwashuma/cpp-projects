CXX		=	g++
CXXFLAGS	=	-std=c++11 -Wall
SOURCE_FILES = main.cc lexer_updated.cc inputbuf.cc parser.cc symbol_table.cc

all:	production debug

production:
		$(CXX) $(CXXFLAGS) -o lexer.out $(SOURCE_FILES)
debug:
		$(CXX) $(CXXFLAGS) -g -o lexer-debug.out $(SOURCE_FILES)
clean:
		rm -f lexer.out lexer-debug.out
