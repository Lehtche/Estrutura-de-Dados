#ifndef PROCESSO_H
#define PROCESSO_H

typedef struct {
    int id; // Identificador do processo
    char numero[30]; // Número do processo
    char data_ajuizamento[20]; 
    int id_classe; // Classe do processo
    int id_assunto[10]; 
    int qtd_assuntos; 
    int ano_eleicao; 
} Processo;

// Funções 
void carregarDados(Processo processos[], int *qtd); // Carrega os dados do arquivo CSV para a estrutura Processo

void ordenarPorId(Processo processos[], int qtd); // Ordena os processos pelo id em ordem crescente

void ordenarPorData(Processo processos[], int qtd); // Ordena os processos pela data de ajuizamento em ordem decrescente

int contarPorClasse(Processo processos[], int qtd, int id_classe); 

void identificarAssuntos(Processo processos[], int qtd); 

void lislistarProcessMultiAssuntos(Processo processos[], int qtd); 

int calcularTramitacao(Processo processo); // Calcula o número de dias que um processo está em tramitação

#endif /* se PROCESSO_H não foi definida anteriormente, o
código entre #ifndef e #endif será processado. Se não, ele será ignorado.*/
