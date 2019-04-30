#include <stdlib.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
	float A[100],B[100],C[100];
	for (int i = 0; i < 100; ++i)
	{
		printf("\n Digite o %d valor para gravar: ",i+1);
		scanf("%f",&A[i]);
	}
	for (int i = 0; i < 100; ++i)
	{
		if (A[i]%2==0)
		{
			B[i] = A[i]*5;
		}else{
			B[i] = A[i]+5;
		}
	}
	for (int i = 0; i < 100; ++i)
	{
		printf("\n >>> A: %d | B: %d",A[i],B[i]);
	}
}