gcc -c lista.c -std=c99 -Wall -o lista.o
gcc -c main.c -o main.o -Wall -std=c99
gcc lista.o main.o -o main
./main
