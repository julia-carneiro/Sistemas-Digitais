#include <stdio.h>
#include "biblioteca.c"

int main() {
    FILE *arquivo;
    int valor1, valor2, valor3;
    int linhas_processadas = 0;

    // Abre o arquivo para leitura
    arquivo = fopen("gas2.sprite", "r");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Processa cada linha do arquivo
    while (fscanf(arquivo, "%d %d %d", &valor1, &valor2, &valor3) == 3 && linhas_processadas < 400) {
        printf("%d %d %d", valor1, valor2, valor3);
        // Chama a função wsm com os valores lidos e 9600 como último parâmetro
        wsm(valor1, valor2, valor3, linhas_processadas);

        linhas_processadas++;
    }

    // Fecha o arquivo após o processamento
    fclose(arquivo);

    return 0;
}