#ifndef PROCESSO_H
#define PROCESSO_H

#include <time.h>

typedef struct {
    char *id;
    char *numero;
    char *data_ajuizamento_str;
    struct tm data_ajuizamento;
    char *id_classe;
    char **id_assunto;
    int total_assuntos;
    int ano_eleicao;
} Processo;

int contar_linhas(const char *nome_arquivo);
int carregar_processos(const char *nome_arquivo, Processo *p, int total);
void liberar_assuntos(Processo *p, int total);
void ordenar_por_id(Processo *p, int n, const char *saida_csv);
void ordenar_por_data(Processo *p, int n, const char *saida_csv);
void contar_por_classe(Processo *p, int n);
void contar_id_assuntos_unicos(Processo *p, int n);
void listar_processos_multiplos_assuntos(Processo *p, int n);
void dias_em_tramitacao(Processo *p, int n);

#endif
