#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
	int entrada,impar[5],par[5],contImpar=0,contPar=0;
	for (int i = 0; i < 15; ++i)
	{
		printf("Digite o %d valor de entrada: ",i);
		scanf("%d",&entrada);
		if((entrada%2) ==0){
			par[contPar]=entrada;
			contPar++;
			if (contPar==5)
			{
				for (int i = 0; i < 5; ++i)
				{
					printf("Par[%d]: %d\n",i+1,par[i]);
				}
				contPar=0;
			}
		}else{
			impar[contImpar]=entrada;
			contImpar++;
			if (contImpar==5)
			{
				for (int i = 0; i < 5; ++i)
				{
					printf("Impar[%d]: %d\n",i+1,impar[i]);
				}
				contImpar=0;
			}
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		printf("Impar[%d]: %d\n",i+1,impar[i]);
	}
	for (int i = 0; i < 5; ++i)
	{
		printf("Par[%d]: %d\n",i+1,par[i]);
	}
}