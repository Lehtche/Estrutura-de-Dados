#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

int main() {
    const char *nome_arquivo = "processo_043_202409032338.csv";
    int total = contar_linhas(nome_arquivo) - 1;
    if (total <= 0) {
        printf("Erro ao contar linhas ou arquivo vazio.\n");
        return 1;
    }

    Processo *processos = malloc(sizeof(Processo) * total);
    if (!carregar_processos(nome_arquivo, processos, total)) {
        printf("Erro ao carregar dados.\n");
        free(processos);
        return 1;
    }

    int opcao;
    do {
        printf("\nMenu de Opcoes:\n");
        printf("1. Ordenar por ID e salvar CSV\n");
        printf("2. Ordenar por Data e salvar CSV\n");
        printf("3. Contar processos por Classe\n");
        printf("4. Contar ID de Assuntos Unicos\n");
        printf("5. Listar todos os processos que estao vinculados a mais de um assunto\n");
        printf("6. Calcular dias em tramitacao de um processo\n");
        printf("7. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // consumir '\n'

        switch (opcao) {
            case 1:
                ordenar_por_id(processos, total, "saida_ordenada_id.csv");
                printf("Processos ordenados por ID salvos em 'saida_ordenada_id.csv'\n");
                break;
            case 2:
                ordenar_por_data(processos, total, "saida_ordenada_data.csv");
                printf("Processos ordenados por data salvos em 'saida_ordenada_data.csv'\n");
                break;
            case 3:
                contar_por_classe(processos, total);
                break;
            case 4:
                contar_id_assuntos_unicos(processos, total);
                break;
            case 5:
                listar_processos_multiplos_assuntos(processos, total);
                break;
            case 6:
                dias_em_tramitacao(processos, total);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    liberar_assuntos(processos, total);
    free(processos);
    return 0;
}
