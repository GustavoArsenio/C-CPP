#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int main(int argc, char const *argv[])
{
	int input,*fib;
	printf("Digite o valor inicial: ");
	scanf("%d",&input);
	fib = (int *) malloc(sizeof(int) * input);
	fib[0] = 0;
	fib[1] = 1;
	for (int i = 2; i <= input; ++i)
	{
		fib[i]=fib[i-1]+fib[i-2];
	}
	(input <1)? printf(" 0 \n") : (input == 1) ? printf(" 1 \n"): printf(" %d \n",fib[input]);
	free(fib);
}