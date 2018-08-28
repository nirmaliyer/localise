CXXFLAGS += -std=c++11 `root-config --cflags --libs`  
OPTIMFLAGS += -O3 -funroll-loops 
LDFLAGS += -lstdc++
DBGFLAGS += -g
NEURNWPATH = ../neurnwlib
all : neurnw

OBJDIR :
	mkdir -p build

neurnw: src/localise.cpp 
	g++ src/localise.cpp -o build/neurnw -I$(NEURNWPATH)/src -Wl,-rpath=$(NEURNWPATH)/build -lNN -L$(NEURNWPATH)/build $(CXXFLAGS) $(LDFLAGS) $(OPTIMFLAGS)

neurnw2: src/train.cpp 
	g++ src/train.cpp -o build/neurnw2 -I$(NEURNWPATH)/src -Wl,-rpath=../$(NEURNWPATH)/build -lNN -L$(NEURNWPATH)/build $(CXXFLAGS) $(LDFLAGS) $(OPTIMFLAGS)

dbgneurnw2:  src/train.cpp 
	g++ src/train.cpp -o build/dbgneurnw2 -I$(NEURNWPATH)/src -Wl,-rpath=../$(NEURNWPATH)/build -lNN -L$(NEURNWPATH)/build $(CXXFLAGS) $(LDFLAGS) $(DBGFLAGS)

clean :
	rm -vf build/* 
