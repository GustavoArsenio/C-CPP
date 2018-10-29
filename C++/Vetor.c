#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int x,y;
	printf("\n\n Digite a quantidade de linhas: ");
	scanf("%d",&x);
	printf("\n Digite a quantidade de colunas ");
	scanf("%d",&y);
	printf("\n\n\n \t x: %d \n \t y: %d",x,y);
	int *pMatriz = (int *) malloc ((y*x) *sizeof(int)); 
	for (int i = 0; i < x*y; ++i)
	{
		scanf("\n\n\t\t %d",&pMatriz[i]);
		/* code */
	}
	for (int i = 0; i < x*y; ++i)
	{
		printf("\n\n\t\t %d ",pMatriz[i]);
		/* code */
	}
	printf("\n Tamanho: %d", sizeof(pMatriz) );
	/* code */
	return 0;
}