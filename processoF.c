#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

#define LINHA_TAM 512

char *strdup_safe(const char *src) {
    char *dest = malloc(strlen(src) + 1);
    if (dest) strcpy(dest, src);
    return dest;
}

int contar_linhas(const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return -1;
    int count = 0;
    char ch;
    while ((ch = fgetc(f)) != EOF) {
        if (ch == '\n') count++;
    }
    fclose(f);
    return count;
}

void parse_data(const char *str, struct tm *data) {
    memset(data, 0, sizeof(struct tm));
    sscanf(str, "%d-%d-%d %d:%d:%d", &data->tm_year, &data->tm_mon, &data->tm_mday, &data->tm_hour, &data->tm_min, &data->tm_sec);
    data->tm_year -= 1900;
    data->tm_mon -= 1;
}

void limpar_chaves(char *s) {
    int i = 0, j = 0;
    while (s[i]) {
        if (s[i] != '{' && s[i] != '}') {
            s[j++] = s[i];
        }
        i++;
    }
    s[j] = '\0';
}

void separar_assuntos(Processo *proc, char *campo) {
    limpar_chaves(campo);
    char *token = strtok(campo, ",");
    proc->total_assuntos = 0;
    proc->id_assunto = NULL;
    while (token) {
        proc->id_assunto = realloc(proc->id_assunto, (proc->total_assuntos + 1) * sizeof(char*));
        printf(" -> assunto %d: %s\n", proc->total_assuntos, token);

        proc->id_assunto[proc->total_assuntos++] = strdup_safe(token);
        token = strtok(NULL, ",");
    }
}

int carregar_processos(const char *nome_arquivo, Processo *p, int total) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return 0;
    char linha[LINHA_TAM];
    fgets(linha, LINHA_TAM, f); // pula o cabeçalho
    for (int i = 0; i < total && fgets(linha, LINHA_TAM, f); i++) {
        char *id = strtok(linha, ",");
        char *numero = strtok(NULL, ",");
        char *data_str = strtok(NULL, ",");
        char *id_classe = strtok(NULL, ",");
        char *id_assunto = strtok(NULL, ",");
        char *ano = strtok(NULL, "\n");

        printf("Linha %d - id_assunto bruto: %s\n", i + 2, id_assunto);

        p[i].id = strdup_safe(id);
        p[i].numero = strdup_safe(numero);
        p[i].data_ajuizamento_str = strdup_safe(data_str);
        parse_data(data_str, &p[i].data_ajuizamento);
        limpar_chaves(id_classe);
        p[i].id_classe = strdup_safe(id_classe);
        separar_assuntos(&p[i], id_assunto);
        p[i].ano_eleicao = atoi(ano);
    }
    fclose(f);
    return 1;
}

void liberar_assuntos(Processo *p, int total) {
    for (int i = 0; i < total; i++) {
        free(p[i].id);
        free(p[i].numero);
        free(p[i].data_ajuizamento_str);
        free(p[i].id_classe);
        for (int j = 0; j < p[i].total_assuntos; j++) {
            free(p[i].id_assunto[j]);
        }
        free(p[i].id_assunto);
    }
}

void salvar_csv(const char *nome, Processo *p, int n) {
    FILE *f = fopen(nome, "w");
    fprintf(f, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < n; i++) {
        fprintf(f, "%s,%s,%s,%s,{", p[i].id, p[i].numero, p[i].data_ajuizamento_str, p[i].id_classe);
        for (int j = 0; j < p[i].total_assuntos; j++) {
            fprintf(f, "%s%s", p[i].id_assunto[j], j < p[i].total_assuntos - 1 ? "," : "");
        }
        fprintf(f, "},%d\n", p[i].ano_eleicao);
    }
    fclose(f);
}

void ordenar_por_id(Processo *p, int n, const char *saida_csv) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp(p[i].id, p[j].id) > 0) {
                Processo tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }
        }
    }
    salvar_csv(saida_csv, p, n);
}

void ordenar_por_data(Processo *p, int n, const char *saida_csv) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (difftime(mktime(&p[i].data_ajuizamento), mktime(&p[j].data_ajuizamento)) < 0) {
                Processo tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }
        }
    }
    salvar_csv(saida_csv, p, n);
}

void contar_por_classe(Processo *p, int n) {
    for (int i = 0; i < n; i++) {
        int count = 1;
        if (p[i].id_classe == NULL) continue;
        for (int j = i + 1; j < n; j++) {
            if (p[j].id_classe && strcmp(p[i].id_classe, p[j].id_classe) == 0) {
                count++;
                p[j].id_classe = NULL; // marca como já contado
            }
        }
        printf("Classe %s: %d processo(s)\n", p[i].id_classe, count);
    }
}

void contar_id_assuntos_unicos(Processo *p, int n) {
    char **unicos = NULL;
    int total = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p[i].total_assuntos; j++) {
            int existe = 0;
            for (int k = 0; k < total; k++) {
                if (strcmp(unicos[k], p[i].id_assunto[j]) == 0) {
                    existe = 1;
                    break;
                }
            }
            if (!existe) {
                unicos = realloc(unicos, (total + 1) * sizeof(char*));
                unicos[total++] = strdup_safe(p[i].id_assunto[j]);
            }
        }
    }
    printf("Total de id_assuntos únicos: %d\n", total);
    for (int i = 0; i < total; i++) free(unicos[i]);
    free(unicos);
}

void listar_processos_multiplos_assuntos(Processo *p, int n) {
    printf("Processos com mais de um assunto:\n");
    for (int i = 0; i < n; i++) {
        if (p[i].total_assuntos >= 1) {
            printf("ID: %s | Número: %s | Data: %s | Classe: %s | Assuntos: {", 
                p[i].id, p[i].numero, p[i].data_ajuizamento_str, p[i].id_classe);
            for (int j = 0; j < p[i].total_assuntos; j++) {
                printf("%s%s", p[i].id_assunto[j], j < p[i].total_assuntos - 1 ? ", " : "");
            }
            printf("} | Ano eleição: %d\n", p[i].ano_eleicao);
        }
    }
}

void dias_em_tramitacao(Processo *p, int n) {
    char id_busca[50];
    printf("Digite o ID do processo: ");
    scanf("%s", id_busca);
    for (int i = 0; i < n; i++) {
        if (strcmp(p[i].id, id_busca) == 0) {
            time_t agora = time(NULL);
            double dias = difftime(agora, mktime(&p[i].data_ajuizamento)) / (60 * 60 * 24);
            printf("Dias em tramitação: %.0f\n", dias);
            return;
        }
    }
    printf("Processo não encontrado.\n");
}