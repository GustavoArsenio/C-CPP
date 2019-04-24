gcc -c lista.c -std=c99 -Wall -o lista.o
if [[ $? == 0 ]]; then
	gcc -c main.c -o main.o -Wall -std=c99
	if [[ $? == 0 ]]; then
		gcc lista.o main.o -o main
		if [[ $? == 0 ]]; then
			./main
		fi
	fi
fi