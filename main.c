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

    printf("\n Ordenando por ID de forma crescente \n");
    ordenar_por_id(processos, total, "ordenado_por_id.csv");

    printf("\n Ordenando por data_ajuizamento de forma decrescente\n");
    ordenar_por_data(processos, total, "ordenado_por_data.csv");

    printf("\n Contagem por id_classe \n");
    contar_por_classe(processos, total);

    printf("\n Total de id_assuntos distintos \n");
    contar_id_assuntos_unicos(processos, total);

    printf("\n Processos com mais de um assunto \n");
    listar_processos_multiplos_assuntos(processos, total);

    printf("\n Dias em tramitação \n");
    dias_em_tramitacao(processos, total);

    return 0;
}

//gcc main.c processo.c -o programa.exe
//.\programa.exe

//.\output\main.exe

