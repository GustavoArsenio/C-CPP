#!/bin/bash
gcc -c lista.c -std=c99 -Wall -o lista.o
if [[ $? == 0 ]]; then
	gcc -c $1.c -o $1.o -Wall -std=c99
	if [[ $? == 0 ]]; then
		gcc lista.o $1.o -o ./Executaveis/$1
		if [[ $? == 0 ]]; then
			./Executaveis/$1
		fi
	fi
fi