BIN=ccl.exe
CC=gcc
CCFLAGS=-Wall -Wextra -pedantic -ansi -g

$(BIN): clean ccl.o checking_tools.o shading.o
	$(CC) ccl.o checking_tools.o shading.o -o $(BIN) $(CCFLAGS)

ccl.o: ccl.c
	$(CC) -c ccl.c $(CCFLAGS)

checking_tools.o: checking_tools.c
	$(CC) -c checking_tools.c $(CCFLAGS)

shading.o: shading.c
	$(CC) -c shading.c $(CCFLAGS)

clean:
	rm *.o $(BIN) -f