# Built by refrencing to: https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags
# Author: ubdussamad <ubdussamad@gmail.com>
CC	=	g++

CSTACKUSG = -fstack-usage #C_STACK_USAGE_MONITORING
CCW = -Weverything #CROSS_COMPILER_WARININGS
DERR = -Wdocumentation #DOCUMENTATION_ERR
IMPLICT_CONV = -Wconversion #Warns if it sees any implicit conversion.
DBG = -g3 # When debugging use g3, 3 specific the compiler to even include the macro names as well.

# Keeps symbols in a exclusive sections to avoid keeping redundant data.
EXCLUSIVE_SECTIONS = -ffunction-sections

CFLAGS	=	-Wextra -Wall -Werror -Wshadow -Wdouble-promotion -Wundef -fno-common $(IMPLICT_CONV)
COPTIMIZATIONS	=	-o3



# lex: SerialReader.o
# 	$(CC) $(CFLAGS) $(COPTIMIZATIONS) SerialReader.o -o lex
# 	rm *.o

# SerialReader.o : SerialReader.cpp
# 	$(CC) $(CFLAGS) $(COPTIMIZATIONS) -c SerialReader.cpp

generic : SerialReaderGeneric.cpp
	$(CC) $(CFLAGS) $(COPTIMIZATIONS) SerialReader.cpp -o lexg

clean : 
	rm le* *.o

