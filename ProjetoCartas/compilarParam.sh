#!/bin/bash
gcc -c main.c -std=c99 -Wall -o main.o
if [[ $? == 0 ]]; then 
	gcc -c deck.c -o deck.o -Wall -std=c99
	if [[ $? == 0 ]]; then
		gcc main.o deck.o -o ./Executaveis/deck
		if [[ $? == 0 ]]; then
			./Executaveis/deck
		fi
	fi
fi