all: main.o tuple.o relation.o sort_join.o result_list.o job_list.o threadpool.o query.o
	gcc -o EXEC/sort_join main.o tuple.o relation.o sort_join.o  result_list.o job_list.o  threadpool.o query.o -g -lm -pthread

main.o: FILES/CODE/main.c 
	gcc -c  FILES/CODE/main.c

threadpool.o: FILES/CODE/threadpool.c
	gcc -c FILES/CODE/threadpool.c

query.o: FILES/CODE/query.c
	gcc -c FILES/CODE/query.c

job_list.o: FILES/CODE/job_list.c
	gcc -c FILES/CODE/job_list.c

result_list.o : FILES/CODE/result_list.c
	gcc -c FILES/CODE/result_list.c

tuple.o: FILES/CODE/tuple.c
	gcc -c FILES/CODE/tuple.c

relation.o: FILES/CODE/relation.c
	gcc -c FILES/CODE/relation.c

sort_join.o: FILES/CODE/sort_join.c
	gcc -c FILES/CODE/sort_join.c


clean:
	rm *.o
	rm EXEC/sort_join

run:
	./EXEC/sort_join
