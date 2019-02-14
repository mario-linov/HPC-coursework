#CXXFLAGS = -std=c++11 -Wall O2
HDRS = $(wildcard *.h)
LDLIBS = -lmpi
OBJS = $(patsubst %.cpp,%.o, $(wildcard *.cpp))
EXE = BURGERS

default: compile

.o.cpp: $(HDRS)

compile: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $^ $(LDLIBS)

diff: $(EXE)
	./$(EXE) diff.in

advx: $(EXE)
	./$(EXE) advx.in

advy: $(EXE)
	./$(EXE) advy.in

burg: $(EXE)
	./$(EXE) burg.in

.PHONY: clean
clean:
	$(RM) *.o $(EXE)