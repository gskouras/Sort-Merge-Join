all: main.o tuple.o relation.o batch.o inbetween.o
	gcc -o EXEC/sort_join main.o tuple.o relation.o batch.o inbetween.o -lm -g

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


clean:
	rm *.o
	rm EXEC/sort_join

run:
	./EXEC/sort_join