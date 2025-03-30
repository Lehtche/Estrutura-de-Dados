#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

//abrir arquivo CSV
void carregarDados(Processo processos[], int *qtd) {
    FILE *arquivo = fopen("processo_043_202409032338.csv", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo CSV.\n");
        return;
    }


    char linha[200];
    fgets(linha, sizeof(linha), arquivo); // para ignorar a primeira linha 

    while (fgets(linha, sizeof(linha), arquivo)) {
        // Lê os dados de cada linha e armazena na estrutura Processo
        sscanf(linha, "%d,\"%[^\"]\",%s,%d,{%d},%d", 
            &processos[*qtd].id, 
            processos[*qtd].numero,
            processos[*qtd].data_ajuizamento,
            &processos[*qtd].id_classe,
            &processos[*qtd].id_assunto[0], // Assumindo um único valor inicial
            &processos[*qtd].ano_eleicao);
        
        processos[*qtd].qtd_assuntos = 1; // acessa o próximo índice do vetor processos[]
        (*qtd)++;                    //assumimos que o processo tem 1 assunto, então a quantidade de assuntos é definida como 1
    }

    fclose(arquivo);
}

// Função para ordenar os processos pelo id (crescente)
void ordenarPorId(Processo processos[], int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = i + 1; j < qtd; j++) {
            if (processos[i].id > processos[j].id) {
                Processo temp = processos[i];
                processos[i] = processos[j];
                processos[j] = temp;
            }
        }
    }
}

// Função para ordenar os processos pela data de ajuizamento (decrescente)
void ordenarPorData(Processo processos[], int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = i + 1; j < qtd; j++) {
            if (strcmp(processos[i].data_ajuizamento, processos[j].data_ajuizamento) < 0) {
                Processo temp = processos[i];
                processos[i] = processos[j];
                processos[j] = temp;
            }
        }
    }
}

// Função para contar processos por id_classe
int contarPorClasse(Processo processos[], int qtd, int id_classe) {
    int count = 0;
    for (int i = 0; i < qtd; i++) {
        if (processos[i].id_classe == id_classe) {
            count++;
        }
    }
    return count;
}

// Função para identificar os assuntos únicos nos processos
void identificarAssuntos(Processo processos[], int qtd) {
    int assuntos[1000] = {0}; // Array para verificar assuntos únicos
    int contador = 0;

    for (int i = 0; i < qtd; i++) {
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            if (!assuntos[processos[i].id_assunto[j]]) {
                assuntos[processos[i].id_assunto[j]] = 1;
                contador++;
            }
        }
    }

    printf("Número de assuntos únicos: %d\n", contador);
}

// Função para listar processos com múltiplos assuntos
void listarProcessMultiAssuntos(Processo processos[], int qtd) {
    for (int i = 0; i < qtd; i++) {
        if (processos[i].qtd_assuntos > 1) {
            printf("Processo %d com múltiplos assuntos:\n", processos[i].id);
        }
    }
}

int calcularTramitacao(Processo processo) {
    struct tm tm_ajuizamento = {0};
    strptime(processo.data_ajuizamento, "%Y-%m-%d %H:%M:%S", &tm_ajuizamento);
    time_t t_ajuizamento = mktime(&tm_ajuizamento);
    time_t t_atual = time(NULL);
    double diff = difftime(t_atual, t_ajuizamento);
    return diff / (60 * 60 * 24); // Retorna a diferença em dias
}
