all: main.o 
	gcc -o EXEC/sort_join main.o  -lm -g

main.o: FILES/CODE/main.c 
	gcc -c -g FILES/CODE/main.c


clean:
	rm *.o
	rm EXEC/sort_join

