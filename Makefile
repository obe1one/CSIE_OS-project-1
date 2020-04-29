all: main.c config.h proc_schd.c
	gcc main.c proc_schd.c config.h -o main
	rm -rf *~

clean:
	rm -rf *.o
	rm -rf *.c

run:
	./main
