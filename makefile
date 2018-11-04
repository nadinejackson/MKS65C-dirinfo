all: dir.o
	gcc dir.o
dir.o: dir.c
	gcc -c dir.c
run: a.out
	./a.out
again: dir.c
	emacs dir.c
