all: main.o tuple.o relation.o batch.o sort_join.o result_list.o inbetween.o
	gcc  -o EXEC/sort_join main.o tuple.o relation.o batch.o sort_join.o  result_list.o inbetween.o -g -lm 
main.o: FILES/CODE/main.c 
	gcc -c  FILES/CODE/main.c

inbetween.o: FILES/CODE/inbetween.c
	gcc -c FILES/CODE/inbetween.c

result_list.o : FILES/CODE/result_list.c
	gcc -c FILES/CODE/result_list.c

tuple.o: FILES/CODE/tuple.c
	gcc -c FILES/CODE/tuple.c

relation.o: FILES/CODE/relation.c
	gcc -c FILES/CODE/relation.c

batch.o: FILES/CODE/batch.c
	gcc -c FILES/CODE/batch.c

sort_join.o: FILES/CODE/sort_join.c
	gcc -c FILES/CODE/sort_join.c


clean:
	rm *.o
	rm EXEC/sort_join

run:
	./EXEC/sort_join
