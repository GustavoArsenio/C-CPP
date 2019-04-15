gcc -c algebra.c -std=c99 -Wall -o algebra.o
gcc -c Matriz.c -o Matriz.o -Wall -std=c99
gcc algebra.o Matriz.o -o Matriz.exe
Matriz.exe 