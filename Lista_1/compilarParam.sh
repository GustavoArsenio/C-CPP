#!/bin/bash
gcc -c $1.c -o $1.o -Wall -std=c99

if [ -f "$Executaveis" ]
then
	echo "Gerando..."
else
		mkdir Executaveis
fi
gcc $1.o -o ./Executaveis/$1
./Executaveis/$1