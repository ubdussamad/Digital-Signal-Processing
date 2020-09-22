CC	=	g++
CFLAGS	=	-Wextra -Wall -Werror -Wshadow -Wdouble-promotion -Wundef -fno-common
COPTIMIZATIONS	=	-o3

CSTACKUSG = -fstack-usage #C_STACK_USAGE_MONITORING
CCW = -Weverything #CROSS_COMPILER_WARININGS
DERR = -Wdocumentation #DOCUMENTATION_ERR


lex: SerialReader.o
	$(CC) $(CFLAGS) $(COPTIMIZATIONS) SerialReader.o -o lex
	rm *.o

SerialReader.o : SerialReader.cpp
	$(CC) $(CFLAGS) $(COPTIMIZATIONS) -c SerialReader.cpp

clean : 
	rm lex *.o

