CPP = g++

INCLUDE =  ..
INCLUDE += ../include
INCLUDE += ../include/dci/include
INCLUDE += ../include/log_codes
INCLUDE += ../dci
INCLUDE += ../parser
INCLUDE += ../server

INC_PARAMS=$(foreach d, $(INCLUDE), -I$d)

CPPFLAGS = -std=c++11 -pthread $(INC_PARAMS)

VPATH = ..:../dci:../parser:../server

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $^

DCIBase.o: DCIBase.cpp DCIBase.hh
	$(CPP) $(CPPFLAGS) -c $<

Parser.o: Parser.cpp Parser.hh ParserBase.cpp ParserBase.hh
	$(CPP) $(CPPFLAGS) -c $<

TcpServer.o: TcpServer.cpp TcpServer.hh  
	$(CPP) $(CPPFLAGS) -c $<

CommandProcessor.o: \
	CommandProcessor.cpp CommandProcessor.hh
	$(CPP) $(CPPFLAGS) -c $<

ut-DCIBase-Parser: \
	ut-DCIBase-Parser.cpp \
	Parser.o \
	DCIBase.o
	$(CPP) $(CPPFLAGS) $^ -o $@

ut-DCIBase-Parser-Tcp: \
	ut-DCIBase-Parser-Tcp.cpp \
	Parser.o \
	DCIBase.o \
	TcpServer.o
	$(CPP) $(CPPFLAGS) $^ -o $@

ut-CommandProcessor: \
	ut-CommandProcessor.cpp \
	CommandProcessor.o
	$(CPP) $(CPPFLAGS) $^ -o $@

ut-CommandProcessor-TcpServer: \
	ut-CommandProcessor-TcpServer.cpp \
	CommandProcessor.o \
	TcpServer.o
	$(CPP) $(CPPFLAGS) $^ -o $@

clean-obj:
	rm -f *.o

clean-bin:
	rm -f ut-DCIBase-Parser
	rm -f ut-DCIBase-Parser-Tcp
	rm -f ut-CommandProcessor
	rm -f ut-CommandProcessor-TcpServer

clean:
	rm -f *.o
	rm -f ut-DCIBase-Parser
	rm -f ut-DCIBase-Parser-Tcp
	rm -f ut-CommandProcessor
	rm -f ut-CommandProcessor-TcpServer
