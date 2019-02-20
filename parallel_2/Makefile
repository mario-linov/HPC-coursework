#CXXFLAGS = -std=c++11 -Wall O2
HDRS = $(wildcard *.h)
OBJS = $(patsubst %.cpp,%.o, $(wildcard *.cpp))
EXE = BURGERS

default: compile

.o.cpp: $(HDRS)
	mpicxx -c $<

compile: $(OBJS)
	mpicxx -o $(EXE) $^

diff: $(EXE)
	mpiexec ./$(EXE) diff.in

advx: $(EXE)
	mpiexec ./$(EXE) advx.in

advy: $(EXE)
	mpiexec ./$(EXE) advy.in

burg: $(EXE)
	mpiexec ./$(EXE) burg.in

.PHONY: clean
clean:
	$(RM) *.o $(EXE)