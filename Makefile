# THE FOLLOWING TWO LINES MUST BE SPECIFIED FOR LUA-DEVEL
EXT_HEADER=../lua/install/include
EXT_LIB=../lua/install/lib

# Here, we begin the building process
CC=g++
CFLAGS=-march=native -O3 -std=c++1y -m64 -I$(EXT_HEADER)
TARGET=upse
SRC=$(wildcard *.cpp)
OBJECTS=$(patsubst %cpp, %o, $(SRC))
LD_OPTS=-L$(EXT_LIB) -llua -ldl -lm -march=native

SO_BASE_SRC=BioNotations.cpp PCProperty.cpp Common.cpp
SO_BASE_OBJS=$(patsubst %cpp, bso/%o, $(SO_BASE_SRC))
SO_TEMPL_SRC=bso/Template/PseModInterface.cpp
SO_TEMPL_OBJS=$(patsubst %cpp, %o, $(SO_TEMPL_SRC))
SO_MODES_SRC=bso/OneHot/PseModOneHot.cpp
SO_MODES_OBJS=$(patsubst %cpp, %o, $(SO_MODES_SRC))
SO_MODES_SOS=OneHot.so

all:$(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

$(TARGET):$(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LD_OPTS)

modules: $(SO_MODES_SOS)
	
$(SO_MODES_SOS):%.so: $(SO_BASE_OBJS) $(SO_MODES_OBJS) $(SO_TEMPL_OBJS)
	$(CC) -shared -fPIC -o $@ $(SO_BASE_OBJS) $(SO_TEMPL_OBJS) $(SO_MODES_OBJS)
	
$(SO_BASE_OBJS):%.o: $(SO_BASE_SRC)
	$(CC) -shared -fPIC $(CFLAGS) -o $@ -c $(patsubst %o, %cpp, $(@F))
	
$(SO_TEMPL_OBJS):%.o: $(SO_TEMPL_SRC)
	$(CC) -shared -fPIC $(CFLAGS) -o $@ -c $(patsubst %o, %cpp, $(@))

$(SO_MODES_OBJS):%.o: $(SO_MODES_SRC)
	$(CC) -shared -fPIC $(CFLAGS) -o $@ -c $(patsubst %o, %cpp, $(@))
	
.PHONY: clean test 
clean:
	rm *.o
	rm bso/*.o
	rm upse
	rm *.so

test: upse tdfs/classic-pseaac.lua test/tiny.fas
	./upse -i test/tiny.fas -f svm -u tdfs/classic-pseaac.lua -t 1 -l 10 -w 0.05

test_mod: upse OneHot.so test/tiny-one-hot-protein.fas
	./upse -i test/tiny-one-hot-protein.fas -f svm -n stdprot -m user,./OneHot.so,ONEHOT -l 19


