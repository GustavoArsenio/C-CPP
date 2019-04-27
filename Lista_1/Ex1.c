#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
	int soma = 0, novoNumero = 0;
	for (int i = 0; i < 18; ++i)
	{
		printf("\n Digite o %d numero: ",i+1);
		scanf("%d",&novoNumero);
		soma += novoNumero;
	}
	printf("\n \t\t *********** A soma eh: %d ***********\n", soma);
	return 0;
}