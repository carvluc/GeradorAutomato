#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX_STRING 100

/*
 
2
a
b
2
0
1
1
1
-1
1
-1
Nome
1

*/

// Variáveis globais
int nivelTab = 0;
char stringTab[TAM_MAX_STRING];

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
		 nomePrograma[TAM_MAX_STRING];
} Automato;

// Protótipos
void inicializa(Automato *);
void criaMatrizTransicao(Automato *);
void destroiObjeto(Automato *);
void geraCodigoAutomato(Automato *);
void insereBibliotecas(FILE *);
void insereFuncoes(FILE *, Automato *);
void inserePrototipos(FILE *, Automato *);
void insereMain(FILE *, Automato *);
void insereVariaveisGlobais(FILE *);
void fechaMain(FILE *);
void tab();
void iniVarsGlobais();

// Funções úteis
void tab(){
	int i;
	char str[TAM_MAX_STRING];
	for(i = 0; i < nivelTab; i++)
		str[i] = '\t';
	str[i] = '\0';	
	strcpy(stringTab, str);	
}

void iniVarsGlobais(){
	int i;
	nivelTab = 0;
}

// Funções principais
void inicializa(Automato *aut) {
	int i;
	printf("Quantos simbolos possui o alfabeto? ");
	scanf("%d", &aut->qtdAlfabeto);
	fflush(stdin);
	
	aut->alfabeto = malloc(aut->qtdAlfabeto * sizeof(char));
	for (i = 0; i < aut->qtdAlfabeto; i++){
		printf("Qual o simbolo %d? ", i);
		scanf("%c", aut->alfabeto + i);
		fflush(stdin);
	}
		
	printf("Quantos estados possui o automato? ");
	scanf("%d", &aut->qtdEstados);
	fflush(stdin);
	
	printf("Qual o estado inicial? ");
	scanf("%d", &aut->estadoInicial);
	fflush(stdin);
	
	printf("Quantos estados finais? ");
	scanf("%d", &aut->qtdEstadosFinais);
	fflush(stdin);
	
	aut->estadoFinais = malloc(aut->qtdEstadosFinais * sizeof(int));
	for (i = 0; i < aut->qtdEstadosFinais; i++){
		 printf("Qual o estado final %d? ", i);
		 scanf("%d", aut->estadoFinais + i);
		 fflush(stdin);
	}	
	
	criaMatrizTransicao(aut);
	
	printf("Qual o nome do programa? ");
	scanf("%s", &aut->nomePrograma);
	fflush(stdin);
	
    printf("(0) goto, (1) funcao? ");
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
	free(aut);
}

void insereBibliotecas(FILE *arq){
    fprintf(arq, "#include <stdio.h>\n#include <stdlib.h>\n\n");
}

void inserePrototipos(FILE *arq, Automato *aut){
	int i;
	fprintf(arq, "//Protótipos de função\n");
	for(i = 0; i < aut->qtdEstados; i++)
		fprintf(arq, "void e%d();\n", i);
	
	fprintf(arq, "\n");
}

void insereMain(FILE *arq, Automato *aut){
	nivelTab++;
	tab();
	
	printf("Está realizando o printf no arquivo com:\n");
	printf("%sx\n", stringTab);
	
	fprintf(arq, "int main(){\n%sgets(palavra);\n%spos=0;\n%se%d();\n", stringTab, stringTab, stringTab, aut->estadoInicial);
}

void fechaMain(FILE *arq){
	fprintf(arq, "}\n");
}

void insereVariaveisGlobais(FILE *arq){
	fprintf(arq, "//Variáveis globais\nchar palavra[100];\nint pos=0;\n\n");
}

void insereFuncoes(FILE *arq, Automato *aut){
	int i, j;
	for(i = 0; i < aut->qtdEstados; i++){
        fprintf(arq, "void e%d(){\n", i);
		for(j = 0; j < aut->qtdAlfabeto; j++){
			fprintf(arq, "if()");
		}
	}
}

void geraCodigoAutomato(Automato *aut){
	FILE * arq;	
	arq = fopen(strcat(aut->nomePrograma, ".c"), "w+");
	insereBibliotecas(arq);
	insereVariaveisGlobais(arq);
	inserePrototipos(arq, aut);
	insereMain(arq, aut);
	fechaMain(arq);
	if(aut->tipo == 1)
		insereFuncoes(arq, aut);
	else
		//insereGoTo(arq, aut);	
	
	fclose(arq);
}

// Main
int main()
{
	iniVarsGlobais();
    Automato *novo;
    novo = malloc(sizeof(Automato));
    printf("*********** GERADOR DE PROGRAMA ***********\n");
    inicializa(novo);
    
    geraCodigoAutomato(novo);		
	
    printf("*********** PROGRAMA GERADO COM SUCESSO ***********\n");
    destroiObjeto(novo);
    
	return 0;
}


