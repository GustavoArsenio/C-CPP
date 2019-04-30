#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
	float A[20],B[20],C;
	for (int i = 0; i < 20; ++i)
	{
		printf("\n Digite o %d valor para gravar: ",i+1);
		scanf("%f",&A[i]);
	}
	for (int i = 0; i < 20; ++i)
	{
		printf("\n Digite o %d valor para gravar: ",i+1);
		scanf("%f",&B[i]);
	}
	for (int i = 0; i < 20; ++i)
	{
		C=A[i];
		A[i]=B[i];
		B[i]=C
	}
	for (int i = 0; i < 20; ++i)
	{
		printf("\n >>> A: %d | B: %d",A[i],B[i]);
	}

}