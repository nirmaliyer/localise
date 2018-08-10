CXXFLAGS += -std=c++11 -O3 -funroll-loops `root-config --cflags --libs  `
LDFLAGS += -lstdc++

NEURNWPATH = ../neurnwlib
all : neurnw

OBJDIR :
	mkdir -p build

neurnw: src/localise.cpp 
	g++ src/localise.cpp -o build/neurnw -I$(NEURNWPATH)/src -Wl,-rpath=$(NEURNWPATH)/build -lNN -L$(NEURNWPATH)/build $(CXXFLAGS) $(LDFLAGS)

neurnw2: src/train.cpp 
	g++ src/train.cpp -o build/neurnw2 -I$(NEURNWPATH)/src -Wl,-rpath=../$(NEURNWPATH)/build -lNN -L$(NEURNWPATH)/build $(CXXFLAGS) $(LDFLAGS)

clean :
	rm -vf build/* 
