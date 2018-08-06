## Neural Network based regression code from Himangshu Saikia

This code is taken from Himangshu's github repo at 
<https://github.com/hsaikia/NeuralNetwork> .  For the first version, 
a few minor changes have been made to enable compilation of the code on 
the ninx system. 

The code is intended for use with the localisation database inorder to find 
the GRB location and check if the accuracy / precision of this method match
with the $\chi^2$ method.

A noteworthy difference with the code on [Github]([https://github.com/hsaikia/NeuralNetwork)
 is the directory structure. Here the library is separated from the main file,
which is put in localisation folder instead.

### List of changes in first version w.r.t Github code

    1. Added a Makefile for compiling the code to a build directory.
    2. Added <string> and <cmath> include files to get main.cpp and
	   NeuralNetwork.h to compile
    3. Added usage of namespace NN in main.cpp (since Neural Network is a
	   separate library now)

