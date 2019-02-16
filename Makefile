OBJFILES = World.o Robot.o NLinkArm.o
HEADERS = World.h Robot.h NLinkArm.h
EXE = client
CPPFLAGS = -Wall -g -std=c++11

$(EXE): $(OBJFILES)
	g++ $(CPPFLAGS) -o $(EXE) $(OBJFILES)

%.o: %.cc $(HEADERS)
	g++ $(CPPFLAGS) -c $<

.PHONY: clean
clean:
	rm *.o $(EXE)
