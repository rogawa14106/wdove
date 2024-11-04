PROG=wdove
TARGET=./build/$(PROG)
SOURCES=$(wildcard src/*.c)
OBJECTS=$(addprefix obj/,$(notdir $(SOURCES:.c=.o)))
INCLUDES=./src

CC=gcc
CFLAGS= -g -Wall

$(TARGET): $(OBJECTS)
	rm -f $@
	$(CC) $(CFLAGS) -I$(INCLUDES) -o $@ $^

obj/%.o : src/%.c
	$(CC) $(CFLAGS) -I$(INCLUDES) -o $@ -c $<

.PHONY: all clean run
all:
	@echo src: $(SOURCES)
	@echo obj: $(OBJECTS)
clean:
	rm -f build/* obj/*
run:
	@./build/$(PROG)

test:
	# 正常系
	./build/$(PROG) "http://abehiroshi.la.coocan.jp/top.htm"
	./build/$(PROG) "http://abehiroshi.la.coocan.jp:80/top.htm?id=10&name=rogawa"
	./build/$(PROG) "http://abehiroshi.la.coocan.jp:80?id=10&name=rogawa"
	./build/$(PROG) "http://abehiroshi.la.coocan.jp:80/top.htm"
	./build/$(PROG) "http://abehiroshi.la.coocan.jp/top.htm"
	./build/$(PROG) "http://abehiroshi.la.coocan.jp:80"
	./build/$(PROG) "http://abehiroshi.la.coocan.jp"
	./build/$(PROG) "http://abehiroshi.la.coocan.jp/top.htm?id=10&name=rogawa"
	./build/$(PROG) "http://abehiroshi.la.coocan.jp?id=10&name=rogawa"
	# 異常系
	# ./build/$(PROG) "abehiroshi.la.coocan.jp"
	# ./build/$(PROG) "http://"

## directory structure 
# root
#  -src
#   *.c
#   *.h
#  -obj
#   *.o
#  -build
#   $(TARGET)
#  Makefile
