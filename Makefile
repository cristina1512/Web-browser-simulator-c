build:
	gcc -o file main.c fct.c
run:
	./file
clean:
	rm -f file
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./file