#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int main(int argc, char const *argv[])
{
	int M[10];
	printf("Digite o valor inicial: ");
	scanf("%d",&M[0]);
	for (int i = 1; i < 10; ++i)
	{
		M[i]=M[i-1]*2;
	}
	for (int i = 0; i < 10; ++i)
	{
		printf("%d\n",M[i]);
	}
}