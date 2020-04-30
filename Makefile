all: main.c config.h scheduler.c process.c
	gcc main.c scheduler.c process.c config.h -o main
	rm -rf *~

clean:
	rm -rf *.o
	rm -rf *.c

run:
	./main
