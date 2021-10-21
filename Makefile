CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall

CPP = $(CROSS_COMPILE)gcc

INCLUDE =  .
INCLUDE += ./include
INCLUDE += ./include/dci/include
INCLUDE += ./include/log_codes
INCLUDE += ./dci
INCLUDE += ./parser
INCLUDE += ./server

INC_PARAMS=$(foreach d, $(INCLUDE), -I$d)

CPPFLAGS  = -std=c++11 -Wall -pthread $(INC_PARAMS)

VPATH = ./dci:./parser:./server

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $^

DCIBase.o: DCIBase.cpp DCIBase.hh
	$(CPP) $(CPPFLAGS) -c $<

DCI.o: DCI.cpp DCI.hh
	$(CPP) $(CPPFLAGS) -c $<

Parser.o: Parser.cpp Parser.hh ParserBase.cpp ParserBase.hh
	$(CPP) $(CPPFLAGS) -c $<

TcpServer.o: TcpServer.cpp TcpServer.hh  
	$(CPP) $(CPPFLAGS) -c $<

CommandProcessor.o: \
	CommandProcessor.cpp CommandProcessor.hh
	$(CPP) $(CPPFLAGS) -c $<

# Unit test for simpulation on a PC
ut-qscan: \
	qscan.cpp \
	Parser.o \
	DCIBase.o \
	TcpServer.o \
	CommandProcessor.o
	$(CPP) $(CPPFLAGS) -DUNITTEST $^ -o $@

# TODO add Modem AT Commands
qscan: \
	qscan.cpp \
	Parser.o \
	DCI.o \
	TcpServer.o \
	CommandProcessor.o
	$(CPP) $(CPPFLAGS) $^ -o $@

clean-obj:
	rm -f *.o

clean-bin:
	rm -f ut-DCIBase-Parser
	rm -f ut-DCIBase-Parser-Tcp

clean:
	rm -f *.o
	rm -f ut-DCIBase-Parser
	rm -f ut-DCIBase-Parser-Tcp
