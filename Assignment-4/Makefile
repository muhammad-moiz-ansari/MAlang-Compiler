# Use g++ because we are using C++ classes
CC = g++
CFLAGS = -Wall -std=c++11

# The final executable name required by the assignment
TARGET = json2xml

all: $(TARGET)

$(TARGET): parser.tab.c lex.yy.c ast.cpp main.cpp
	$(CC) $(CFLAGS) -o $(TARGET) parser.tab.c lex.yy.c ast.cpp main.cpp

# Bison generates the parser C file and the header file
parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

# Flex generates the lexer C file, and it depends on Bison's header
lex.yy.c: scanner.l parser.tab.h
	flex scanner.l

clean:
	rm -f $(TARGET) parser.tab.c parser.tab.h lex.yy.c