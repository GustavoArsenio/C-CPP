#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[]){
	int contaPar = 0,contaImpar = 0, novoNumero, maior,menor,sumPar=0,sumImpar=0,maiorPar;
	for (int i = 0; i < 4; ++i){
		printf("\nDigite o %d numero: ",i+1);
		scanf("%d",&novoNumero);
		if (contaImpar==0 && contaPar==0){
			menor = novoNumero;
			maior = menor;
			printf("\n --- Definiu maior menor %d e %d\n",novoNumero,novoNumero%2);
		}
		if(contaPar==0 && (novoNumero%2)==0){
			maiorPar=novoNumero;
			printf("\n --- Definiu maior par %d\n",novoNumero );
		}
		if(novoNumero%2 == 0){
			// Se for par
			printf("\n--- Somou Par\n");
			sumPar+=novoNumero;
			if (novoNumero > maiorPar)
			{
				printf("\n --- Definiu maior par %d\n",novoNumero );
				maiorPar=novoNumero;
			}
			contaPar++;
		}else{
			// Se for impar
			printf("\n --- Somou impar\n");
			sumImpar+=novoNumero;
			contaImpar++;
		}
		if (novoNumero>maior)
		{
			maior=novoNumero;
		}else if(menor > novoNumero){
			menor=novoNumero;
		}

	}
	printf("\n>>>> A media dos impares eh: %f", ((float)sumImpar/(float)contaImpar) );
	if (contaPar>0)
	{
		printf("\n>>>> O maior numero par eh: %d",maiorPar);
	}else{
		printf("\n>>>> Nao teve par como entrada");
	}
	printf("\n>>>> A diferenca entre o maior e menor eh: %d", maior-menor );
}