gcc -c algebra.c -std=c99 -o algebra.o
gcc -c Matriz.c -o Matriz.o -std=c99
gcc algebra.o Matriz.o -o Matriz.exe
Matriz.exe < in.txt