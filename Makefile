all: main.o tuple.o relation.o
	gcc -o EXEC/sort_join main.o tuple.o relation.o -lm -g

main.o: FILES/CODE/main.c 
	gcc -c  FILES/CODE/main.c

tuple.o: FILES/CODE/tuple.c
	gcc -c FILES/CODE/tuple.c

relation.o: FILES/CODE/relation.c
	gcc -c FILES/CODE/relation.c


clean:
	rm *.o
	rm EXEC/sort_join

run:
	./EXEC/sort_join