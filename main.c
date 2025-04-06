#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "processo.h"

int main() {
    setlocale(LC_ALL, "");  // Suporte a acentuação (Windows)

    Processo processos[MAX_PROCESSOS];
    int total = carregar_processos("processo_043_202409032338.csv", processos);

    if (total < 0) {
        printf("Erro ao carregar os processos.\n");
        return 1;
    }

    int opcao;
    do {
        printf("\n--- MENU ---\n");
        printf("1. Ordem crescente por id\n");
        printf("2. Ordem decrescente por data de ajuizamento\n");
        printf("3. Quantidade de processos por id_classe\n");
        printf("4. Quantidade total de id_assuntos unicos\n");
        printf("5. Listar processos com mais de um assunto\n");
        printf("6. Quantidade de dias de tramitação de um processo\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // consumir \n

        switch (opcao) {
            case 1:
                ordenar_por_id(processos, total, "ordenado_por_id.csv");
                printf("Processos ordenados por ID foram salvos em 'ordenado_por_id.csv'.\n");
                break;
            case 2:
                ordenar_por_data(processos, total, "ordenado_por_data.csv");
                printf("Processos ordenados por data foram salvos em 'ordenado_por_data.csv'.\n");
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
            case 7:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 7);

    return 0;
}
