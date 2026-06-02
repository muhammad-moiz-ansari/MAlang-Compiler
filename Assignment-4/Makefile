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

# Target to run all tests automatically from the tests/ folder
run-all: $(TARGET)
	@echo "\n========== RUNNING VALID TESTS =========="
	@for i in 1 2 3 4 5; do \
		echo "--- tests/test$$i.json ---"; \
		./$(TARGET) < tests/test$$i.json || true; \
		printf "\n"; \
	done
	@echo "--- tests/test9_unicode.json ---"
	@./$(TARGET) < tests/test9_unicode.json || true
	@printf "\n"
	@echo "\n========== RUNNING ERROR TESTS =========="
	@for i in 6 7 8; do \
		echo "--- tests/test$${i}_e.json ---"; \
		./$(TARGET) < tests/test$${i}_e.json || true; \
		printf "\n"; \
	done
	
run-all-TA: $(TARGET)
	@echo "\n========== RUNNING VALID TESTS =========="
	@for i in 1 2 3 4 5 6 7 8 9; do \
		echo "--- tests/T$$i.json ---"; \
		./$(TARGET) < tests/T$$i.json || true; \
		printf "\n"; \
	done

clean:
	rm -f $(TARGET) parser.tab.c parser.tab.h lex.yy.c
