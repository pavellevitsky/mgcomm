CPP = g++

INCLUDE =  ../include
INCLUDE += ../include/dci
INCLUDE += ../include/log_codes

INC_PARAMS=$(foreach d, $(INCLUDE), -I$d)

CPPFLAGS  = -std=c++11 $(INC_PARAMS)

ut-ParserBase: ParserBase.cpp ParserBase.hh
	$(CPP) $(CPPFLAGS) ParserBase.cpp -o $@

ut-Parser: Parser.cpp Parser.hh ut-Parser.cpp
	$(CPP) $(CPPFLAGS) ut-Parser.cpp Parser.cpp -o $@

clean-obj:
	rm -f *.o

clean-bin:
	rm -f ParserBase
	rm -f Parser

clean:
	rm -f *.o
	rm -f ParserBase
	rm -f Parser

all: ParserBase
