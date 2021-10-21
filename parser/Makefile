CPP = g++

INCLUDE += ../include
INCLUDE += ../include/log_codes

INC_PARAMS=$(foreach d, $(INCLUDE), -I$d)

CPPFLAGS  = -std=c++11 $(INC_PARAMS)

ut-Parser: Parser.cpp Parser.hh ut-Parser.cpp
	$(CPP) $(CPPFLAGS) ut-Parser.cpp Parser.cpp -o $@

ut-ParserBase: ParserBase.cpp ParserBase.hh
	$(CPP) $(CPPFLAGS) ParserBase.cpp -o $@

clean-obj:
	rm -f *.o

clean-bin:
	rm -f Parser
	rm -f ParserBase
	rm -f ut-Parser
	rm -f ut-ParserBase

clean:
	rm -f *.o
	rm -f ParserBase
	rm -f Parser
	rm -f ut-Parser
	rm -f ut-ParserBase

