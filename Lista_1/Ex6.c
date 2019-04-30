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
		B[i] = A[i]*3;
	}
	for (int i = 0; i < 30; ++i)
	{
		printf("\n >>> A: %d | B: %d",A[i],B[i]);
	}
}