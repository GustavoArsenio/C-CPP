
/*

###################################
# Construido por: Gustavo Arsenio #
# Curso: Engenharia da computação #
# RA: 0074830                     #
# UNASP Campus São Paulo          #
###################################

*/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algebra.h"


extern struct conjuntoMatrizes *filaMatrizes;
extern int tamanho;
/*
Função: Exponencial
Descrição: Precisa se passar de parametro a base e o valor do esponencial, respectivamente!
OBS: Pode ser positivo ou negativo
*/
int exponencial(int i, int j){
	int aux=1;
	if(j>0)
	{
		for(int k=0;k<j;k++){
			aux*=i;
		}
	}else 
		if( (j=0) )
	{
		aux=1;
	}else if(j<0)
	{
		for(int k=0;k>j;k--){
			aux*=1/i;
		}
	}
	return aux;	
}
/*
Função: Determinante
Descrição: Precisa se passar de parametro a Matriz, no formato "struct MatrixM", para se realizar o feito, está sendo ultilizado como 
função recursiva ( O encadeamento se dá para calcular as determinantes da matrizes reduzidas).

Cálculo: Realizado por Laplace

*/
int determinante(struct MatrixM Matrix){
	int det=0;
	if(Matrix.x==Matrix.y)
	{
		if(Matrix.x==2 && Matrix.y==2)
		{
			return (Matrix.Matriz[0][0]*Matrix.Matriz[1][1]) - (Matrix.Matriz[0][1]*Matrix.Matriz[1][0]);
			
		}else if(Matrix.x==1 && Matrix.y==1)
		{
			return Matrix.Matriz[0][0];
		}
		for(int i=0;i<Matrix.x;i++){
			det +=( determinante(red(Matrix,0,i)))*Matrix.Matriz[0][i]*(exponencial(-1,i));
		}
		return (det);
	}
	printf("\n\n \t **** >>>> Error: Invalid Matrix Dimensions <<<< **** ");		
	return -1;
}
/*
	Função: Fatorial

	Descrição: Recebe um número inteiro. Para se calcula o fatorial por função recursiva

*/
int fatorial(int i){
	if(i<=1){
		return 1;
	}
	return	i*(fatorial(i-1));	
}
/*
	Função: BuildV

	Descrição: Precisa se passar de parametro uma Matriz, no formato "struct MatrixM", para solicitar o nome ele solicita para o usuario que escreva no "buffer"
	para que não venha lixo de memória limpamos o buffer e após isso solicitamos o nome.
	Depois solicita as dimensões da matriz em X e Y.
	Instancia a matriz, assim preenchendo todos os campos da "struct MatrixM", do "algebra.h".
*/
struct MatrixM buildV(struct MatrixM Matrix){
	int showPosX=0,showPosY=0;
	setbuf(stdin,NULL);
	printf("\n\n Digite o nome: ");
	gets(Matrix.nome);
	printf("\t >>> %s",Matrix.nome);
	printf("\n\n Digite a quantidade de linhas: ");
	scanf("%d",&Matrix.x);
	printf("\n Digite a quantidade de colunas: ");
	scanf("%d",&Matrix.y);
	printf("\n\n\n \t x: %d \n \t y: %d",Matrix.x,Matrix.y);
	Matrix.Matriz = instMatrix(Matrix.Matriz,Matrix.x,Matrix.y);
	printf("\n \n \t Bem vindo ao MatrixBuilder! \t \n\n\n");
	for(int i = 0; i < Matrix.x; ++i)
	{
		for(int j=0; j < Matrix.y; j++){
			if(showPosX>=Matrix.x){
				showPosX = 0;
				showPosY++;
			}
			printf("\n\nDigite o valor M[%d][%d]: ",showPosX,showPosY);
			scanf("%d",&Matrix.Matriz[i][j]);
			showPosX++;
		}
	}	
	return Matrix;
}
/*
	Função: ShowV

	Descrição: Precisa se passar de parametro uma Matriz, no formato "struct MatrixM", mostra todos os valores da matriz,
	para se aplicar a delimitação, é aplicado um "\n" quando vai para a próxima linha
*/
void showV(struct MatrixM Matrix){
	printf("\n\n\n\n \t\t\t ***  Mostrando Matriz *** \n\n");
	for(int i = 0; i < Matrix.x; i++)
	{	
	
		for(int j =0; j<Matrix.y; j++){
			printf(" | x[%d]y[%d] %d | ",i,j,Matrix.Matriz[i][j]);
		}
				printf("\n");
	}
	printf("\n\n\n\n \t\t *** Fim mostrar Matriz*** ");
}
void showVT(struct MatrixM Matrix){
	printf("\n\n\n\n \t\t\t ***  Mostrando Matriz *** \n\n");
	for(int i = 0; i < Matrix.y; i++)
	{	
	
		for(int j =0; j<Matrix.x; j++){
			printf(" | x[%d]y[%d] %d | ",i,j,Matrix.matrizTransp[i][j]);
		}
				printf("\n");
	}
	printf("\n\n\n\n \t\t *** Fim mostrar Matriz*** ");
}
/*
	Função: ShowM

	Descrição: Precisa se passar de parametro uma Matriz, no formato "struct MatrixM", mostra o nome salvo na struct, dimensões e por fim chama a matriz ShowV.
*/
void showM(struct MatrixM Matrix){
	printf("\n\n ----------------------------- ");
	printf("\n\n| >>> \t **** Matriz: %s *****",Matrix.nome);
	printf("\n| >>> \tde dimensao x:[%d] e y:[%d] ",Matrix.x,Matrix.y);
	printf("\n| >>>\t  Possui os valores de: ");
	showV(Matrix);
	printf("\n\n ----------------------------- ");
	
}
/*
	Função: Soma

	Descrição: Precisa se passar de parametro uma Matriz, do tipo inteiro e soma todos os valores dentro da matriz;
*/
void soma(int **pMatriz,int x,int y){
	printf(" \n>>>>>>>>> Entrou na soma");
	int aux=0;
	printf("\n\n x: %d \t y: %d",x,y);
	for (int i = 0; i < x; ++i)
	{
		for(int j =0; j<y; j++){
			printf("\n\n Posicao: %d %d, valor %d",i,j,pMatriz[i][j]);
			aux = aux + pMatriz[i][j];
		}
	}
	printf("\n\n Resultado: %d",aux);
}
/*
	Função: SumMatrix

		Descrição: Precisa se passar de parametro duas Matrizes, no formato "struct MatrixM", devolve uma matriz com cada posição preenchida soma dos 
		valores daquela respectiva posição (Nas matrizes recebidas de parametro)
*/
struct MatrixM sumMatrix(struct MatrixM pMatrizResult,struct MatrixM pMatriz, struct MatrixM pMatriz2){
	pMatrizResult.Matriz = instMatrix(pMatrizResult.Matriz,pMatriz.x,pMatriz.y);
	setbuf(stdin,NULL);
	printf("\n\n Digite o nome para a matriz de soma: ");
	gets(pMatrizResult.nome);
	for (int i = 0; i <pMatriz.x; ++i)
	{
		for(int j =0; j<pMatriz.y; j++){
			pMatrizResult.Matriz[i][j] = (int)pMatriz2.Matriz[i][j] + (int)pMatriz.Matriz[i][j];
		}
	}
	pMatrizResult.x = pMatriz.x;
	pMatrizResult.y = pMatriz.y;
	return pMatrizResult;
}
/*
	Função: transpose

		Descrição: Precisa se passar de parametro uma Matriz, no formato "struct MatrixM", devolve a matriz transposta
*/
int **transpose(struct MatrixM pMatriz){
	int **MTransp=NULL;
	MTransp	= instMatrix(MTransp,pMatriz.y,pMatriz.x);
	for(int i =0;i<pMatriz.x;i++){
		for(int j =0;j<pMatriz.y;j++){
			MTransp[j][i]=pMatriz.Matriz[i][j];
		}
	}
	return MTransp;
}
/*
	Função: instMatrix

	Descrição: Precisa se passar de parametro uma Matriz (ponteiro de ponteiro), do tipo inteiro, tamnaho em X e tamanho em Y, aloca um espaço de memória equivalente a X*Y
	
*/
int **instMatrix(int **pMatriz, int x, int y){
	pMatriz = (int **) malloc (x *sizeof(int *));
	
	for(int i = 0; i < x; ++i)
	{
		pMatriz[i] = (int *) malloc(y * sizeof(int));
	} 
	return pMatriz;
}
/*
	Função: liberaM

	Descrição: Precisa se passar de parametro uma Matriz (ponteiro de ponteiro), do tipo inteiro, tamnaho em X, para liberar os ponteiros em X e depois o último poeiro.
	
*/
void liberaM(int **pMatriz,int x){
	for(int i = 0; i < x; i++)
	{
		free(pMatriz[i]);
	}
		free(pMatriz);
}
/*
	Função: red

	Descrição: Precisa se passar de parametro uma Matriz, no formato "struct MatrixM", e qual a coluna em X e Y para ser ignorada e devolve a matriz sem a coluna e linha especificada.
*/
struct MatrixM red(struct MatrixM Matrix, int x, int y){
	struct MatrixM mRed;
	int k=0,l=0;
	mRed.Matriz = NULL;
	mRed.Matriz = instMatrix(mRed.Matriz,((Matrix.x) -1),((Matrix.y) -1));
	mRed.x = ((Matrix.x) -1);
	mRed.y = ((Matrix.y) -1);
	mRed.matrizTransp = transpose(mRed);
	strcpy(mRed.nome," ** e uma Matriz reduzida ** ");
	mRed.description = sprintf(mRed.nome," Tamanho [%d][%d] \n",mRed.x,mRed.y);
	for(int i = 0; i < Matrix.x; i++)
	{	
		for(int j =0; j<Matrix.y; j++){
			if(i==x){
				i++;
				if(i==Matrix.x){
					break;
				}
			}			
			if(j==y){
				j++;
				if(j==Matrix.y){
					break;
				}
			}
			mRed.Matriz[k][l] = Matrix.Matriz[i][j];
			l++;
		}
		l=0;
		k++;
	}
	return mRed;
}
/*
	Função: SumMatrix

	Descrição: Precisa se passar de parametro duas Matrizes, no formato "struct MatrixM", devolve a matriz resultado da multiplicação das matrizes de parametro
*/
struct MatrixM multMatrix(struct MatrixM pMatriz, struct MatrixM pMatriz2){
	printf("\n\n\n \t\t\t  >>>>>>>>>>>>>  Entrou <<<<<<<<<<<<<<<<");
	struct MatrixM matrixResult;
	matrixResult.Matriz = instMatrix(matrixResult.Matriz,pMatriz.x,pMatriz2.y);
	int auxX=0,auxXf=0, auxY=0,resultM=0;
	int **pMatrizTransp = NULL;
	pMatrizTransp = instMatrix(pMatrizTransp,pMatriz.y,pMatriz.x);
	pMatrizTransp = transpose(pMatriz);
	strcpy(matrixResult.nome,"Matriz_Multip");
	matrixResult.x = pMatriz.x;
	matrixResult.y = pMatriz2.y;
	if(pMatriz.y==pMatriz2.x)
	{
	
		for(int k =0;k<pMatriz.x;k++)
		{
			for(int i =0;i<pMatriz2.x;i++)
			{
				for(int j =0;j<pMatriz2.y;j++)
				{
					if( !(auxX%pMatriz2.y) ){
						auxX=0;
					}
					resultM+=pMatriz2.Matriz[j][i] * pMatrizTransp[auxX][k];
					auxX++;
				}
				if( !(auxX%pMatriz.y) ){
						auxX=0;
				}
				if( !(auxXf%pMatriz.y) ){
						auxXf=0;
				}
				matrixResult.Matriz[auxY][auxXf] = resultM;
				auxXf++;
				resultM=0;
			}
			auxY++;
		}
	}else{
		printf("\n\n \t **** >>>> Error: Invalid Matrix Dimensions <<<< **** ");		
		strcpy(matrixResult.nome,"**** >>>> Error: Invalid Matrix Dimensions <<<< ****");
	}
	return matrixResult;
}
/*

void showMatriz(struct conjuntoMatrizes matrizes,int mSelected){
	showM(matrizes[mSelected].Matriz);
}
void liberaAllM(struct conjuntoMatrizes *matrizes){
	free(matrizes.auxMatriz);
	liberaM(matrizes.Matriz);
}
*/
struct conjuntoMatrizes *aloca(){
	 struct conjuntoMatrizes *novo=(struct conjuntoMatrizes *) malloc(sizeof(struct conjuntoMatrizes));
	 if(!novo){
		  printf("Sem memoria disponivel!\n");
		  exit(1);
	 }else{
		  printf("\n Nova matriz: ");
		  novo->Matriz = buildV(novo->Matriz);
		  return novo;
	 }
 
}
int vazia(struct conjuntoMatrizes *FILA)
{
	if(FILA == NULL){
		return 1;
	}
	else{
		  return 0;
	}
}
void addMatriz(){
	struct conjuntoMatrizes *novaMatriz=aloca();
	novaMatriz->proxItem= NULL;
	 if(vazia(filaMatrizes)){
		filaMatrizes = novaMatriz;
	 }else{
		struct conjuntoMatrizes *temp = filaMatrizes;
		while(temp->proxItem != NULL){
			temp = temp->proxItem;
		}
		temp->proxItem = novaMatriz;
	 }
	 tamanho++;
}
void somarMatriz(int indicador1, int indicador2){
	struct conjuntoMatrizes *temp = filaMatrizes,*temp2 = filaMatrizes;
	if(vazia(filaMatrizes)){
		printf("\n\n \t\t Error: Nenhuma Matriz Cadastrada");
	}else{
		if(indicador1>tamanho || indicador2>tamanho){
			printf("\n\n \t\t Error: Posicao invalida ");
			return;
		}
		for(int i=0;i<tamanho;i++){
			if(i==indicador1-1){
				temp = temp->proxItem;
				break;
			}
		}
		for(int i=0;i<tamanho;i++){
			if(i==indicador2-1){
				temp2 = temp2->proxItem;
				break;
			}
		}
		struct conjuntoMatrizes *novaMatriz=(struct conjuntoMatrizes *) malloc(sizeof(struct conjuntoMatrizes));
		novaMatriz->Matriz=sumMatrix(novaMatriz->Matriz,temp->Matriz,temp2->Matriz);
		novaMatriz->proxItem=NULL;
		struct conjuntoMatrizes *temp = filaMatrizes;
		while(temp->proxItem != NULL){
			temp = temp->proxItem;
		}
		temp->proxItem = novaMatriz;
		showM(novaMatriz->Matriz);
		tamanho++;
	}
}
void exibir(int indicador){
	if(vazia(filaMatrizes)){
		printf("\n\n \t\t Error: Nenhuma Matriz Cadastrada");
	}else{
	struct conjuntoMatrizes *temp = filaMatrizes;
	if(indicador<0){
		while(temp->proxItem != NULL){
			showM(temp->Matriz);
			temp = temp->proxItem;
		}
			showM(temp->Matriz);
		return;
	}
	if(indicador>tamanho){
		printf("\n\n \t\t Error: Posicao invalida ");
		return;
	}
	for(int i=0;i<tamanho;i++){
		if(i==indicador-1){
			temp = temp->proxItem;
				break;
		}
	}
			showM(temp->Matriz);
	}
}
void multpMatriz(int indicador1, int indicador2){
	struct conjuntoMatrizes *temp = filaMatrizes,*temp2 = filaMatrizes;
	if(vazia(filaMatrizes)){
		printf("\n\n \t\t Error: Nenhuma Matriz Cadastrada");
	}else{
		if(indicador1>tamanho || indicador2>tamanho){
			printf("\n\n \t\t Error: Posicao invalida ");
			return;
		}
		for(int i=0;i<tamanho;i++){
			if(i==indicador1-1){
				temp = temp->proxItem;
				break;
			}
		}
		for(int i=0;i<tamanho;i++){
			if(i==indicador2-1){
				temp2 = temp2->proxItem;
				break;
			}
		}
		struct conjuntoMatrizes *novaMatriz=(struct conjuntoMatrizes *) malloc(sizeof(struct conjuntoMatrizes));
		novaMatriz->Matriz=multMatrix(temp->Matriz,temp2->Matriz);
		novaMatriz->proxItem=NULL;
		struct conjuntoMatrizes *temp = filaMatrizes;
		while(temp->proxItem != NULL){
			temp = temp->proxItem;
		}
		temp->proxItem = novaMatriz;
		showM(novaMatriz->Matriz);
		tamanho++;
	}
}
int detMatriz(int indicador){
	struct conjuntoMatrizes *temp = filaMatrizes;
	if(vazia(filaMatrizes)){
		printf("\n\n \t\t Error: Nenhuma Matriz Cadastrada");
	}else{
		if(indicador>tamanho){
			printf("\n\n \t\t Error: Posicao invalida ");
			return -1;
		}
		for(int i=0;i<tamanho;i++){
			if(i==indicador-1){
				temp = temp->proxItem;
				break;
			}
	}
		return determinante(temp->Matriz);
	}
	return 0;
}
void transpMatriz(int indicador){
		struct conjuntoMatrizes *temp = filaMatrizes;
	if(vazia(filaMatrizes)){
		printf("\n\n \t\t Error: Nenhuma Matriz Cadastrada");
	}else{
		if(indicador>tamanho){
			printf("\n\n \t\t Error: Posicao invalida ");
			return;
		}
		for(int i=0;i<tamanho;i++){
			if(i==indicador-1){
				temp = temp->proxItem;
				break;
			}
		}
		printf("\n\n \t\t A transposta eh: \n ");
		showVT(temp->Matriz);
	}
}