#!/bin/bash
gcc -c $1.c -std=c99 -Wall -o ./Executaveis/$1.o
gcc ./Executaveis/$1.o -o ./Executaveis/$1
./Executaveis/$1