#include <stdio.h>
int main()
{
    int valorDeEntrada=1,tamanho,auxTam,raio;
    	printf("Digite o tamanho da Matriz: ");
        scanf("%d", &tamanho);
        if(tamanho<=0){
        	printf(">>> Tamanho invalido\n");
        }else
        {
            valorDeEntrada=1;
            int k=0;
            int l=0;
            int Matriz[tamanho][tamanho];
            auxTam=tamanho;
            if(tamanho%2==0)
                raio=tamanho/2;
            else if(tamanho%2==1)
                raio=(tamanho/2)+1;
            for(int c=1; c<=raio; c++)
            {
                for(int i=k; i<auxTam; i++)
                {
                    for(int j=l; j<auxTam; j++){
                    	Matriz[i][j]=valorDeEntrada;
					}
                }
                valorDeEntrada++;
                k++;
                l++; 
                auxTam--;
            }
            for(int i=0; i<tamanho; i++)
            {
                for(int j=0; j<tamanho; j++)
                {
                    printf("%3d  |",Matriz[i][j]);
                }
                printf("\n");
            }
        }
}