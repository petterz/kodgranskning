

runupdate: lagerupdate
	./lagerupdate #< input.txt

lagerupdate: tree.o utils.o list.o lagerupdate.c
	gcc -ggdb -Wall lagerupdate.c tree.o utils.o list.o -o lagerupdate



run: lager
	./lager #< input.txt

lager: tree.o utils.o list.o lager.c
	gcc -ggdb -Wall lager.c tree.o utils.o list.o -o lager


utils.o: utils.c utils.h
	gcc -Wall utils.c -c -o utils.o

tree.o: tree.c tree.h
	gcc -Wall tree.c -c -o tree.o

list.o: list.c list.h
	gcc -Wall list.c -c -o list.o
