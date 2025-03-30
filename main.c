#include <stdio.h>
#include "processo.h"

int main() {
    Processo processos[1000]; // Array para armazenar até 1000 processos
    int qtd_processos = 0;

   
    carregarDados(processos, &qtd_processos); //arquivo csv

    // Ordena os processos pelo id em ordem crescente
    ordenarPorId(processos, qtd_processos);
    printf("Processos ordenados por ID:\n");
    for (int i = 0; i < qtd_processos; i++) {
        printf("%d - %s\n", processos[i].id, processos[i].numero);
    }

    // Ordena os processos pela data de ajuizamento em ordem decrescente
    ordenarPorData(processos, qtd_processos);
    printf("\nProcessos ordenados por data de ajuizamento:\n");
    for (int i = 0; i < qtd_processos; i++) {
        printf("%d - %s\n", processos[i].id, processos[i].data_ajuizamento);
    }

    // Conta quantos processos estão vinculados a uma classe específica
    int id_classe = 12554; // Exemplo de id_classe
    int count_classe = contarPorClasse(processos, qtd_processos, id_classe);
    printf("\nNúmero de processos com id_classe %d: %d\n", id_classe, count_classe);

    // Identifica os assuntos únicos nos processos
    identificarAssuntos(processos, qtd_processos);

    // Lista os processos que têm múltiplos assuntos
    listarProcessMultiAssuntos(processos, qtd_processos);

    // Calcula a tramitação de um processo em dias
    int tramitacao = calcularTramitacao(processos[0]);
    printf("Processo %d esta em tramitação há %d dias.\n", processos[0].id, tramitacao);

    return 0;
}
