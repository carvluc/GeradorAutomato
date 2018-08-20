#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tipos
typedef struct {
	int qtdAlfabeto,
		qtdEstados,
		qtdEstadosFinais,
		estadoInicial,
		*estadoFinais,
		**tabelaTransicao,
		tipo;
		
	char *alfabeto,
		 *nomePrograma;
} Automato;

// Protótipos
void inicializa(Automato *);
void criaMatrizTransicao(Automato *);
void destroiObjeto(Automato *);
void geraCodigoAutomato(Automato *);
void insereBibliotecas(FILE *);
void insereFuncoes(FILE *, Automato *);
void msg(char const *);
char* concatenaString(char const *, char const *);

// Funções úteis
void msg(char const * msg){
	printf(msg);
}

char* concatenaString(char const *base, char const *add){
	char* temp = NULL;
	temp = malloc((strlen(base) + strlen(add) + 1));
	strcpy(temp, base);
	strcat(temp, add);
	return temp;
}

// Funções principais
void inicializa(Automato *aut) {
	int i;
	msg("Quantos simbolos possui o alfabeto? ");
	scanf("%d", &aut->qtdAlfabeto);
	fflush(stdin);
	
	aut->alfabeto = malloc(aut->qtdAlfabeto * sizeof(char));
	for (i = 0; i < aut->qtdAlfabeto; i++){
		printf("Qual o simbolo %d? ", i);
		scanf("%c", aut->alfabeto + i);
		fflush(stdin);
	}
		
	msg("Quantos estados possui o automato? ");
	scanf("%d", &aut->qtdEstados);
	fflush(stdin);
	
	msg("Qual o estado inicial? ");
	scanf("%d", &aut->estadoInicial);
	fflush(stdin);
	
	msg("Quantos estados finais? ");
	scanf("%d", &aut->qtdEstadosFinais);
	fflush(stdin);
	
	aut->estadoFinais = malloc(aut->qtdEstadosFinais * sizeof(int));
	for (i = 0; i < aut->qtdEstadosFinais; i++){
		 printf("Qual o estado final %d? ", i);
		 scanf("%d", aut->estadoFinais + i);
		 fflush(stdin);
	}	
	
	criaMatrizTransicao(aut);
	
	msg("Qual o nome do programa? ");
	scanf("%s", &aut->nomePrograma);
	fflush(stdin);
	
    msg("(0) goto, (1) funcao? ");
    scanf("%d", &aut->tipo);
	fflush(stdin);
}

void criaMatrizTransicao(Automato *aut) {
	int i, j;
	aut->tabelaTransicao = malloc(aut->qtdEstados * sizeof(int*));
	for (i = 0; i < aut->qtdEstados; i++) 
		aut->tabelaTransicao[i] = malloc(aut->qtdAlfabeto * sizeof(int));
		
	for (i = 0; i < aut->qtdEstados; i++)
		for (j = 0; j < aut->qtdAlfabeto; j++){
			printf("Para o estado E%d e o simbolo %c, qual o proximo estado? ", i, aut->alfabeto[j]);
			scanf("%d", *(aut->tabelaTransicao + i) + j);	
			fflush(stdin);
		}	
}

void destroiObjeto(Automato *aut){
	int i;
	for (i = 0; i < aut->qtdEstados; i++) 
		free(aut->tabelaTransicao[i]);
		
	free(aut->tabelaTransicao);
	free(aut->alfabeto);
	free(aut->estadoFinais);
	free(aut->nomePrograma);
	free(aut);
}

void insereBibliotecas(FILE *arq){
    fprintf(arq, "#include <stdio.h>\n#include <stdlib.h>");
}

void insereFuncoes(FILE *arq, Automato *aut){
	int i, j;
	for(i = 0; i < aut->qtdEstados; i++){
        fprintf(arq, "void");
		for(j = 0; j < aut->qtdAlfabeto; j++){

		}
	}
}

void geraCodigoAutomato(Automato *aut){
	FILE * arq;
	aut->nomePrograma = "Teste";
	arq = fopen(concatenaString(aut->nomePrograma, ".c"), "w+");
	
	insereBibliotecas(arq);
	if(aut->tipo == 1)
		insereFuncoes(arq, aut);
	else
		//insereGoTo(arq, aut);
	
	fclose(arq);
}



// Main
int main()
{
    Automato *novo;
    novo = malloc(sizeof(Automato));
    msg("*********** GERADOR DE PROGRAMA ***********\n");
    //inicializa(novo);
    
    geraCodigoAutomato(novo);		
	
    msg("*********** PROGRAMA GERADO COM SUCESSO ***********\n");
    destroiObjeto(novo);
    
	return 0;
}


