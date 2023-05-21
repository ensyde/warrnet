CPP = g++
TESTS = poppkt

default: $(TESTS)

poppkt: ../poppkt.o checkpop.o
	$(CPP) -o poppkt ../poppkt.o checkpop.o

../poppkt.o: ../poppkt.cpp ../poppkt.h
	$(CPP) -c -o ../poppkt.o ../poppkt.cpp
checkpop.o: checkpop.cpp
	$(CPP) -c -o checkpop.o checkpop.cpp

clean:
	rm -f *.o *~ ../poppkt.o
