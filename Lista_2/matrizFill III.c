#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int main(int argc, char const *argv[])
{
	int tamanho;
	
	printf("\nDigite o tamanho da matriz: ");
	scanf("%d",&tamanho);
	int M[tamanho][tamanho];
	
	for (int i = 0; i < tamanho; ++i)
	{
		for (int j = 0; j < tamanho; ++j)
		{
			M[i][j] = pow(2,i+j);
			printf("%3d", M[i][j]);
		}
		printf("\n");
	}
}