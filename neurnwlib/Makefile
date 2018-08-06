###############################################################
# MakeFile for Himagshu's test Neural Network  codes
# First made on :Thu 22 Feb 2018 13:56:50 CET 
# Project : SPHiNX
# Author : Nirmal
# Last Modified :
# #############################################################

CXX = g++
CXXFLAGS += -Wall -I${src} -std=c++11 -m64 -lm 
#ROOTFLAGS= -lFITSIO `root-config --cflags --libs --ldflags`  
OPTIMFLAGS = -O3 -funroll-loops 
SHAREFLAGS = -fPIC 
DBGFLAGS = -g -Og -DDBGLEV=1
LDFLAGS += -lstdc++

DEPS = $(src)/NeuralNetwork.h $(src)/Neuron.h
lNNOBJ = $(lib)/NeuralNetwork.o $(lib)/Neuron.o
lNNSO = $(lib)/libNN.so 

src = src
lib = build

libNN : $(lNNOBJ) | OBJDIR
	$(CXX) -shared -Wl,-soname,libNN.so -o $(lNNSO) $(lNNOBJ)

OBJDIR :
	mkdir -p $(lib)

$(lib)/%.o : $(src)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(OPTIMFLAGS) $(SHAREFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(lib)/*.o

.PHONY: cleanall

cleanall:
	rm -f $(lib)/*.o $(lib)/*.so 
