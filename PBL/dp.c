#include <stdio.h>

// DEFINE UM POLIGONO

void dp(int forma,int r, int g, int b,int tamanho,long int x, long int y){
    //endereço = 4 bits
    unsigned int endereco = 0b0; // O ENDEREÇO É DEFINIDO AQUI OU É RECEBIDO COMO PARAMETRO?

    // Definir o opcode
    unsigned int opcode = 0b0000;
    //Declarando variaveis do barramento
    unsigned long int dataA = 0b0; 
    unsigned long int dataB = 0b0;

    // Definir o número de bits para cada campo
    int bits_coordenada = 9;
    int bits_cor = 3;
    int bits_tamanho = 4;
    int bits_opcode = 4;

    //definindo dataA
    dataA |= ((unsigned int)endereco) << (bits_opcode);
    dataA |= ((unsigned int) opcode);

    // definindo dataB
    dataB |= ((unsigned int)forma) <<( 2* bits_coordenada + bits_tamanho + 3 * bits_cor);
    dataB |= ((unsigned int)b) << (2* bits_cor + bits_tamanho + 2 * bits_coordenada);
    dataB |= ((unsigned int)g) << (bits_cor + bits_tamanho + 2* bits_coordenada);
    dataB |= ((unsigned int)r) << (bits_tamanho + 2* bits_coordenada);
    dataB |= ((unsigned int)tamanho) << (2 * bits_coordenada);
    dataB |= ((unsigned long int)y) << (bits_coordenada);
    dataB |= ((unsigned long int)x);

    //kernel(dataA, dataB);
}