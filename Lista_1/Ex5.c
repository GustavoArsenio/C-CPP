#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
	float A[50],B[50],C[50];
	for (int i = 0; i < 50; ++i)
	{
		printf("\n Digite o %d valor para gravar: ",i+1);
		scanf("%f",&A[i]);
	}
	for (int i = 0; i < 50; ++i)
	{
		printf("\n Digite o %d valor para gravar: ",i+1);
		scanf("%f",&B[i]);
	}
	for (int i = 0; i < 50; ++i)
	{
		C[i]=(A[i]+B[i])/2;
	}
	for (int i = 0; i < 50; ++i)
	{
		printf("\n >>> A media de (%.2f + %.2f)/2 = %.2f",A[i],B[i],C[i]);
	}
}
