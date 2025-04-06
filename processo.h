#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_PROCESSOS 1000
#define MAX_ASSUNTOS 20

typedef struct {
    char id[20];
    char data_ajuizamento[20];
    char id_classe[50];
    char id_assunto[200];
    char assuntos[MAX_ASSUNTOS][50];
    int total_assuntos;
} Processo;

// Funções principais
int carregar_processos(const char *nome_arquivo, Processo processos[]);
void ordenar_por_id(Processo p[], int n, const char *saida_csv);
void ordenar_por_data(Processo p[], int n, const char *saida_csv);
void contar_por_classe(Processo p[], int n);
void contar_id_assuntos_unicos(Processo p[], int n);
void listar_processos_multiplos_assuntos(Processo p[], int n);
void dias_em_tramitacao(Processo p[], int n);

// Função auxiliar
void separar_assuntos(Processo *proc);
void imprimir_processos(Processo p[], int n);

#endif
