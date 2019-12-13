all: main.o tuple.o relation.o batch.o inbetween.o sort_join.o
	gcc -o EXEC/sort_join main.o tuple.o relation.o batch.o inbetween.o sort_join.o -lm -g

main.o: FILES/CODE/main.c 
	gcc -c  FILES/CODE/main.c

tuple.o: FILES/CODE/tuple.c
	gcc -c FILES/CODE/tuple.c

relation.o: FILES/CODE/relation.c
	gcc -c FILES/CODE/relation.c

batch.o: FILES/CODE/batch.c
	gcc -c FILES/CODE/batch.c

inbetween.o: FILES/CODE/inbetween.c
	gcc -c FILES/CODE/inbetween.c

sort_join.o: FILES/CODE/sort_join.c
	gcc -c FILES/CODE/sort_join.c


clean:
	rm *.o
	rm EXEC/sort_join

run:
	./EXEC/sort_join