all: 
	gcc dir.c

run: all
	./a.out

clean:
	rm -f *~ *.gch* a.out *.o *#*
