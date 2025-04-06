#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

int main() {
    Processo processos[MAX_PROCESSOS];
    int total = carregar_processos("processo_043_202409032338.csv", processos);

    if (total == -1) {
        printf("Erro ao carregar o arquivo.\n");
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
        printf("6. Quantidade de dias de tramitacao de um processo \n");
        printf("7. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // consumir \n
        //limpar tela

        switch (opcao) {
            case 1:
                printf("\n--- Ordem crescente por id ---\n");
                //adicinar função
                break;

            case 2:
                printf("\n--- Ordem decrescente por data de ajuizamento ---\n");
                //função
                break;

            case 3:
                printf("\n--- Quantidade de processos por id_classe ---\n");
                contar_por_classe(processos, total);
                break;

            case 4:
                printf("\n--- Quantidade total de id_assuntos unicos ---\n");
                contar_id_assuntos_unicos(processos, total);
                break;

            case 5:
                printf("\n--- Processos com mais de um assunto ---\n");
                listar_processos_multiplos_assuntos(processos, total);
                break;
            case 6:
                printf("\n--- Quantidade de dias de tramitacao de um processo ---\n");
                //função
                break;

            case 7:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 7);

    return 0;
}
