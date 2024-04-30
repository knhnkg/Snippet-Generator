CC = clang
CFLAGS = -Wall -Wextra -Werror -Wno-newline-eof -Wno-unused-parameter -pedantic -std=c99 -ferror-limit=1
LDFLAGS = -lm
DIFF = diff
# -y diffs side by side.

PROGRAM = program
SOURCE_FILES = $(shell find . -type f -name '*.c')
HEADER_FILES = $(shell find . -type f -name '*.h')
OUTPUT_FILES = $(shell find . -type f -name '*myoutput.txt')
OBJECT_FILES = $(SOURCE_FILES:.c=.o)

.PHONY: all build clean run run1 run2 run3 test1 test2 test3

all: build

testall: test1 test2 test3

build: $(PROGRAM)

clean:
	rm -f $(PROGRAM) $(OBJECT_FILES) $(OUTPUT_FILES)

run: run1

test1: build
	./$(PROGRAM) alice feet < data1.txt > test1-myoutput.txt
	$(DIFF) test1-myoutput.txt test1-out.txt

test2: build
	./$(PROGRAM) down australia new zealand < data2.txt > test2-myoutput.txt
	$(DIFF) test2-myoutput.txt test2-out.txt

test3: build
	./$(PROGRAM) alice children poison drink bottle < data3.txt > test3-myoutput.txt
	$(DIFF) test3-myoutput.txt test3-out.txt

run1: build
	./$(PROGRAM) alice feet < data1.txt

run2: build
	./$(PROGRAM) down australia new zealand < data2.txt

run3: build
	./$(PROGRAM) alice children poison drink bottle < data3.txt

$(PROGRAM): $(OBJECT_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)