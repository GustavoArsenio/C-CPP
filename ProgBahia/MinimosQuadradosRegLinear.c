#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int N;
float X[30],Y[30];
float A[2][3];

void LeDados(){

    printf("Numero de pontos: n = ");
    scanf("%d",&N);
    for(int i=0;i<N;i++){
        printf("\n ------------------------------- \n");
        printf("\nX [%d]: ",i+1);
        scanf("%f",&X[i]);
        printf("\nY [%d]: ",i+1);
        scanf("%f",&Y[i]);
        printf("\n ------------------------------- \n");
    }
}
void MontaSistema(){
    A[0][1]=0;
    A[1][1]=0;
    A[0][2]=0;
    A[1][2]=0;
    for(int i=0;i<N;i++){
        A[0][1] += X[i];
        A[1][1] += X[i]*X[i];
        A[0][2] += Y[i];
        A[1][2] += X[i]*Y[i];
    }
    A[0][0] = N;
    A[1][0] = A[0][1];
}
float det(float a,float b,float c,float d){
    return ( a * d ) - ( b * c );
}



int main(){
    float a,b;

    LeDados();
    MontaSistema();
    a = det(A[0][2], A[0][1], A[1][2],A[1][1]) / det(A[0][0], A[0][1], A[1][0],A[1][1]);
    b = det(A[0][0], A[0][2], A[1][0],A[1][2]) / det(A[0][0], A[0][1], A[1][0],A[1][1]);
    printf("\nG(x) = %10.6f + %10.6f x\n <ENTER> ",a,b);
    return 0;
    
}