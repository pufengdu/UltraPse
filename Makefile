# THE FOLLOWING TWO LINES MUST BE SPECIFIED FOR LUA-DEVEL
EXT_HEADER=
EXT_LIB=

# Here, we begin the building process
CC=g++
CFLAGS=-march=native -O3 -std=c++1y -m64 -I$(EXT_HEADER)
TARGET=upse
SRC=$(wildcard *.cpp)
OBJECTS=$(patsubst %cpp, %o, $(SRC))
all:$(TARGET)
LD_OPTS=-L$(EXT_LIB) -llua -ldl -lm -march=native

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

$(TARGET):$(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LD_OPTS)

.PHONY: clean
clean:
	rm *.o
	rm upse
