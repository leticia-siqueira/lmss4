processflow: main.o
	gcc main.o -o processflow

main.o: main.c
	gcc -c main.c

clean: 
	rm -f *.o *.txt processflow

.PHONY: clean