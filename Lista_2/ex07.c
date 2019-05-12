#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
	float *valor = (float *) malloc(sizeof(float)*100), entrada;
	printf("Digite o valor de entrada: ");
	scanf("%f",&entrada);
	valor[0]=entrada;
	for (int i = 1; i < 100; ++i)
	{
		valor[i]=valor[i-1]/2;
	}
	for (int i = 0; i < 100; ++i)
	{
		printf("valor: %.4f << \n",valor[i]);
	}
	free(valor);
}