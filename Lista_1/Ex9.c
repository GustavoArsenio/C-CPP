#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
	float A[30],B[30],C[30];
	for (int i = 0; i < 30; ++i)
	{
		printf("\n Digite o %d valor para gravar: ",i+1);
		scanf("%f",&A[i]);
	}
	for (int i = 0; i < 30; ++i)
	{
		printf("\n Digite o %d valor para gravar: ",i+1);
		scanf("%f",&B[i]);
	}
	for (int i = 0; i < 30; ++i)
	{
		if (i<15)
		{
			C[i]=A[i];
		}else{
			C[i]=B[i-15];
		}
	}
	for (int i = 0; i < 30; ++i)
	{
		printf("\n >>> C: %d",C[i]);
	}

}