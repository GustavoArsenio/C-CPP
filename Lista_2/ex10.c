#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
	int linha,M[12][12];
	float soma=0;
	char opcao;
	printf("\nQual a linha desejada para a operacao: ");
	scanf("%d",&linha);
	printf("\nDigite S para soma e M para media: ");
	scanf("%s",&opcao);
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			printf("\nDigite o valor de M[%d][%d]: ",i+1,j+1);
			scanf("%d",&M[i][j]);
			if (linha==(i+1))
			{
				soma+=M[i][j];
			}
		}
	}
	if (opcao=='S')
	{
		printf("\n A soma da linha %d eh: %.1f\n",linha,soma);
	}else if(opcao=='M'){
		printf("\n A media da linha %d eh: %.1f\n",linha,soma/12);
	}
}