
all: sort_join.o main.o relation.o tuple.o list.o CuTest.o unitest.o
	gcc -o sort_join main.o sort_join.o tuple.o relation.o list.o CuTest.o unitest.o

main.o: FILES/CODE/main.c 
	gcc -c -g FILES/CODE/main.c

list.o: FILES/CODE/list.c
	gcc -c -g FILES/CODE/list.c

sort_join.o: FILES/CODE/sort_join.c
	gcc -c -g FILES/CODE/sort_join.c

tuple.o: FILES/CODE/tuple.c
	gcc -c -g FILES/CODE/tuple.c

relation.o: FILES/CODE/relation.c
	gcc -c -g FILES/CODE/relation.c
	
unitest.o: FILES/CODE/unitest.c
	gcc -c -g FILES/CODE/unitest.c

CuTest.o: FILES/CODE/CuTest.c
	gcc -c -g FILES/CODE/CuTest.c

clean:
	rm *.o
	rm sort_join
	rm UnityTest.txt
	rm Join_Results.txt

