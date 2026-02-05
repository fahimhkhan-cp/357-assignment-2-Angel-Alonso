CC = gcc
CFLAGS = -Wall -std=c99 -pedantic
MAIN = fsmin
OBJS = asign2.o

all : $(MAIN)

$(MAIN) : $(OBJS) 
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)


asign2.o : asign2.c
	$(CC) $(CFLAGS) -c asign2.c

clean :
	rm *.o $(MAIN) core