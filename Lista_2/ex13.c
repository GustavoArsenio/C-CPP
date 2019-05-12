#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
	int M[12][12];
	float soma=0;
	char opcao;
	printf("\nDigite S para soma e M para media: ");
	scanf("%s",&opcao);
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			printf("\nDigite o valor de M[%d][%d]: ",i+1,j+1);
			scanf("%d",&M[i][j]);
		}
	}
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			if(i==j){
				break;
			}else{
				soma+=M[i][j];
			}
		}
	}
	if (opcao=='S')
	{
		printf("\n A soma eh : %.1f\n",soma);
	}else if(opcao=='M'){
		printf("\n A media eh : %.1f\n",soma/12);
	}
}