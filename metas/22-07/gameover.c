#include <stdio.h>


int telagameover() {
    FILE *arquivo;
    int valor1, valor2, valor3;
    int linhas_processadas = 0;

    // Abre o arquivo para leitura
    arquivo = fopen("gameover.sprite", "r");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Processa cada linha do arquivo
    while (fscanf(arquivo, "%d %d %d", &valor1, &valor2, &valor3) == 3 && linhas_processadas < 4800) {
        // Chama a função wsm com os valores lidos e 9600 como último parâmetro
        if(valor1 == 6 && valor2 == 7 && valor3 == 7){

        }else{

            wbm(valor1, valor2, valor3, linhas_processadas-1);
        }
    
        linhas_processadas++;
    }

    // Fecha o arquivo após o processamento
    fclose(arquivo);

    return 0;
}