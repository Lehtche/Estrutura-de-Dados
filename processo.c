#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

int carregar_processos(const char *nome_arquivo, Processo processos[]) {
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp) return -1;

    char linha[1024];
    fgets(linha, sizeof(linha), fp); // Ignora cabeçalho

    int i = 0;
    while (fgets(linha, sizeof(linha), fp) && i < MAX_PROCESSOS) {
        char *token = strtok(linha, ",");
        if (!token) continue;
        strcpy(processos[i].id, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(processos[i].data_ajuizamento, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(processos[i].id_classe, token);

        token = strtok(NULL, "\n");
        if (!token) continue;
        strcpy(processos[i].id_assunto, token);

        separar_assuntos(&processos[i]);
        i++;
    }

    fclose(fp);
    return i;
}

void separar_assuntos(Processo *proc) {
    proc->total_assuntos = 0;
    char buffer[200];
    strcpy(buffer, proc->id_assunto);

    char *start = strchr(buffer, '{');
    char *end = strchr(buffer, '}');
    if (!start || !end || start > end) return;

    *end = '\0';
    start++;

    char *token = strtok(start, "|");
    while (token != NULL && proc->total_assuntos < MAX_ASSUNTOS) {
        strcpy(proc->assuntos[proc->total_assuntos], token);
        proc->total_assuntos++;
        token = strtok(NULL, "|");
    }
}

void ordenar_por_id(Processo p[], int n, const char *saida_csv) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp(p[i].id, p[j].id) > 0) {
                Processo tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }
        }
    }

    FILE *fp = fopen(saida_csv, "w");
    if (!fp) {
        printf("Erro ao criar o arquivo: %s\n", saida_csv);
        return;
    }

    fprintf(fp, "id,data_ajuizamento,id_classe,id_assunto\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s,%s,%s,%s\n", p[i].id, p[i].data_ajuizamento, p[i].id_classe, p[i].id_assunto);
    }
    fclose(fp);
}

void ordenar_por_data(Processo p[], int n, const char *saida_csv) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp(p[i].data_ajuizamento, p[j].data_ajuizamento) < 0) {
                Processo tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }
        }
    }

    FILE *fp = fopen(saida_csv, "w");
    if (!fp) {
        printf("Erro ao criar o arquivo: %s\n", saida_csv);
        return;
    }

    fprintf(fp, "id,data_ajuizamento,id_classe,id_assunto\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s,%s,%s,%s\n", p[i].id, p[i].data_ajuizamento, p[i].id_classe, p[i].id_assunto);
    }
    fclose(fp);
}

void contar_por_classe(Processo p[], int n) {
    for (int i = 0; i < n; i++) {
        int ja_contado = 0;
        for (int j = 0; j < i; j++) {
            if (strcmp(p[i].id_classe, p[j].id_classe) == 0) {
                ja_contado = 1;
                break;
            }
        }
        if (!ja_contado) {
            int cont = 1;
            for (int k = i + 1; k < n; k++) {
                if (strcmp(p[i].id_classe, p[k].id_classe) == 0) cont++;
            }
            printf("Classe %s: %d processo(s)\n", p[i].id_classe, cont);
        }
    }
}

void contar_id_assuntos_unicos(Processo p[], int n) {
    char unicos[MAX_PROCESSOS * MAX_ASSUNTOS][50];
    int total = 0;

    for (int i = 0; i < n; i++) {
        for (int a = 0; a < p[i].total_assuntos; a++) {
            int encontrado = 0;
            for (int u = 0; u < total; u++) {
                if (strcmp(p[i].assuntos[a], unicos[u]) == 0) {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(unicos[total], p[i].assuntos[a]);
                total++;
            }
        }
    }

    printf("Total de id_assuntos únicos: %d\n", total);
}

void listar_processos_multiplos_assuntos(Processo p[], int n) {
    int contador = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].total_assuntos > 1) {
            printf("ID: %s - Assuntos: %s\n", p[i].id, p[i].id_assunto);
            contador++;
        }
    }
    printf("Total de processos com mais de um assunto: %d\n", contador);
}

void dias_em_tramitacao(Processo p[], int n) {
    time_t agora = time(NULL);
    for (int i = 0; i < n; i++) {
        int ano, mes, dia;
        if (sscanf(p[i].data_ajuizamento, "%d-%d-%d", &ano, &mes, &dia) == 3) {
            struct tm ajuiza = {0};
            ajuiza.tm_year = ano - 1900;
            ajuiza.tm_mon = mes - 1;
            ajuiza.tm_mday = dia;

            time_t t_ajuiza = mktime(&ajuiza);
            double dias = difftime(agora, t_ajuiza) / (60 * 60 * 24);
            printf("ID: %s - Dias em tramitação: %.0f\n", p[i].id, dias);
        } else {
            printf("Data inválida no processo ID: %s\n", p[i].id);
        }
    }
}
