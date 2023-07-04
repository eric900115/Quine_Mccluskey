# sample Makefile
RES = src/main.cpp
HDR = src/qm.h
CPP = src/qm.cpp

EXE = pa1
all:
	g++ -std=c++11 $(RES) $(HDR) $(CPP) -o $(EXE)
clean:
	rm $(EXE)