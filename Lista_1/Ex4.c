#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
	int soma = 0, novoNumero = 0;
	for (int i = 0; i < 100; ++i)
	{
		printf("\n Digite o %d numero: ",i+1);
		scanf("%d",&novoNumero);
		soma += novoNumero;
	}
	printf("\n \t\t *********** A media eh: %d ***********\n", soma/18);
	return 0;
}