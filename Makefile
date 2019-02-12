# ------------------------------------------------------------
# Makefile: Makefile for Lab 3
#   author: Luke Slevinsky, 1453199
#						Logan McDonald, 1401297
#
# Usage: make  // compiles the program server program
#				 make serialtester // compiles the serialtester
#				 make datagen // compiles the datagen
#        make tar   	 // create a 'tar.gz' archive of 'allFiles'
#				 make client // compile client
#				 make attacker // compile attacker
#        make clean 	 // remove unneeded files
# ------------------------------------------------------------

target := lab3
allFiles := Makefile timer.h datagen.c Lab3IO.c Lab3IO.h serialtester.c main.c
objects :=
headers := timer.h Lab3IO.h
CFLAGS :=  -Wall -lm
threadSan := -fsanitize=thread -g
LDFLAGS= -pthread -lpthread
DEBUGFLAG := -ggdb
CC := gcc
# ------------------------------------------------------------

default: $(headers) main.o
	$(CC) $(LDFLAGS) $(DEBUGFLAG) $(CFLAGS) -o main server.o

serialtester: serialtester.o Lab3IO.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o serialtester serialtester.o Lab3IO.o

datagen: datagen.o Lab3IO.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o datagen datagen.o Lab3IO.o

main.o: main.c
	$(CC) $(CFLAGS) main.c -c

datagen.o: datagen.c
	$(CC) $(CFLAGS) datagen -c

serialtester.o: serialtester.c
	$(CC) $(CFLAGS) serialtester.c -c

Lab3IO.o: Lab3IO.c
	$(CC) $(CFLAGS) Lab3IO.c -c

tar:
	touch $(target).tar.gz
	mv $(target).tar.gz  x$(target).tar.gz
	tar -cvf $(target).tar $(allFiles)
	gzip $(target).tar

clean:
	rm *.o main
