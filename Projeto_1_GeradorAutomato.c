#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX_STRING 200

/*
Fácil
2
a
b
2
0
1
1
1
-1
-1
1
Nome
1

Difícil
3
a
b
c
3
0
1
2
1
-1
-1
-1
2
0
-1
-1
-1
Nome
1

*/

// Variáveis globais
int nivelTab = 0;

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
char* tab(int);
void iniVarsGlobais();
int buscaLinear();

// Funções úteis
char* tab(int tam){
	int i;
	char * str = malloc(tam + 1);
	for(i = 0; i < tam; i++)
		str[i] = '\t';
	str[i] = '\0';	
	
	return str;
}

int buscaLinear(int *v, int tam, int n){
	int i;
	for(i = 0; i < tam; i++)
		if(v[i] == n)
			return 1;
			
	return 0;
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
	fprintf(arq, "//Protótipos de função\nvoid aceita();\nvoid rejeita();\n");
	for(i = 0; i < aut->qtdEstados; i++)
		fprintf(arq, "void e%d();\n", i);
	
	fprintf(arq, "\n");
}

void insereMain(FILE *arq, Automato *aut){
	fprintf(arq, "//Main\nint main(){\n%sgets(palavra);\n%spos = 0;\n%se%d();\n", tab(1), tab(1), tab(1), aut->estadoInicial);
}

void fechaMain(FILE *arq){
	fprintf(arq, "}\n\n");
}

void insereVariaveisGlobais(FILE *arq){
	fprintf(arq, "//Variáveis globais\nchar palavra[100];\nint pos = 0;\n\n");
}

void insereFuncoes(FILE *arq, Automato *aut){
	int i, j, auxInicio=0, auxFim=0;
	
	fprintf(arq, "void aceita(){\n%sprintf(\"Aceita!\");\n}\n\nvoid rejeita(){\n%sprintf(\"Rejeita\");\n}\n\n", tab(1), tab(1));
	
	for(i = 0; i < aut->qtdEstados; i++){
        fprintf(arq, "void e%d(){\n", i);
        auxFim = 0;
        auxInicio = 0;
		for(j = 0; j < aut->qtdAlfabeto; j++){
			if(aut->tabelaTransicao[i][j] != -1){
				fprintf(arq, "%s%s(palavra[pos] == '%c'){\n%spos++;\n%se%d();\n%s}", auxInicio == 0 ? tab(1) : "", auxInicio == 0 ? "if" : "else if", aut->alfabeto[j], tab(2), tab(2), aut->tabelaTransicao[i][j], tab(1));	
				auxFim++;
				auxInicio++;
			}
		}
		
		if(buscaLinear(aut->estadoFinais, aut->qtdEstadosFinais, i))
			fprintf(arq, "%s%sif(palavra[pos] == '\\0'){\n%saceita();\n%s}", auxInicio == 0 ? tab(1) : "", auxFim == 0 ? "" : "else ", tab(2), tab(1));
			
		fprintf(arq, "else\n%srejeita();\n}\n\n", tab(2));	
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

int main()
{
	char opcao;
    Automato *novo;
    novo = malloc(sizeof(Automato));
    
    iniVarsGlobais();
    
    printf("*********** GERADOR DE PROGRAMA ***********\n");
    inicializa(novo);
    geraCodigoAutomato(novo);		
    printf("*********** PROGRAMA GERADO COM SUCESSO ***********\n\n");
    
    printf("Deseja testar o automato em questao? [S/N]");
	scanf("%c", &opcao);
	
	//Tentar compilar e abrir o programa gerado pelo comando system
	
	destroiObjeto(novo);
    
	return 0;
}


