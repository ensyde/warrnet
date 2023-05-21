###
# For FreeBSD
CFLAGS = -O2 -funroll-loops -fexpensive-optimizations
LIBS = -pthread

###
# For Linux
# CFLAGS = -O2 -funroll-loops -fexpensive-optimizations
# LIBS = -lpthread

###
# For all
OBJS = threads.o child.o network.o client.o channel.o globals.o clog.o \
	msgcentre.o ftpcentre.o gamecentre.o chatcentre.o main.o poppkt.o \
	database.o user.o msgqueue.o sha1.o obj.o

CPP = g++

default: $(OBJS)
	$(CPP) $(OBJS) $(LIBS) -o warnet

main.o: main.cpp
	$(CPP) -c -o main.o main.cpp $(CFLAGS)

threads.o: threads.cpp threads.h
	$(CPP) -c -o threads.o threads.cpp $(CFLAGS)

child.o: child.cpp child.h
	$(CPP) -c -o child.o child.cpp $(CFLAGS)

network.o: network.cpp network.h
	$(CPP) -c -o network.o network.cpp $(CFLAGS)

client.o: client.cpp client.h
	$(CPP) -c -o client.o client.cpp $(CFLAGS)

channel.o: channel.cpp channel.h
	$(CPP) -c -o channel.o channel.cpp $(CFLAGS)

globals.o: globals.cpp globals.h
	$(CPP) -c -o globals.o globals.cpp $(CFLAGS)

msgqueue.o: msgqueue.cpp msgqueue.h
	$(CPP) -c -o msgqueue.o msgqueue.cpp $(CFLAGS)

msgcentre.o: msgcentre.cpp msgcentre.h
	$(CPP) -c -o msgcentre.o msgcentre.cpp $(CFLAGS)

sha1.o: sha1.cpp sha1.h
	$(CPP) -c -o sha1.o sha1.cpp $(CFLAGS)

database.o: database.cpp database.h
	$(CPP) -c -o database.o database.cpp $(CFLAGS)

obj.o: obj.cpp obj.h
	$(CPP) -c -o obj.o obj.cpp $(CFLAGS)

ftpcentre.o: ftpcentre.cpp ftpcentre.h
	$(CPP) -c -o ftpcentre.o ftpcentre.cpp $(CFLAGS)

poppkt.o: poppkt.cpp poppkt.h
	$(CPP) -c -o poppkt.o poppkt.cpp $(CFLAGS)

gamecentre.o: gamecentre.cpp gamecentre.h chatcmds.h
	$(CPP) -c -o gamecentre.o gamecentre.cpp $(CFLAGS)

chatcentre.o: chatcentre.cpp chatcentre.h
	$(CPP) -c -o chatcentre.o chatcentre.cpp $(CFLAGS)

user.o: user.cpp user.h
	$(CPP) -c -o user.o user.cpp $(CFLAGS)

clog.o: clog.cpp clog.h
	$(CPP) -c -o clog.o clog.cpp $(CFLAGS)
clean:
	rm -f *~ *.o

count:
	@echo "Number of Source Lines:"
	@echo "  Documentation: `cat ../README | wc -l`"
	@echo "  Makefile:      `cat Makefile | wc -l`"
	@echo "  CPP:           `cat *.cpp | wc -l`"
	@echo "  H:             `cat *.h | wc -l`"
	@echo "  Source Tot:    `cat *.cpp *.h Makefile | wc -l`"
