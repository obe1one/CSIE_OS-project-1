all: main.c config.h
	gcc main.c config.h -o main

clean:
	rm -rf *.o

run:
	./main
