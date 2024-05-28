#include <stdio.h>

void wbm(int linha, int coluna, int r, int g, int b){
    // endereço  = 12 bits
    unsigned long int endereco = 0;

    //definindo opcode
    unsigned int opcode = 0b0010;
    //definindo os barramentos
    unsigned long int dataA = 0;
    unsigned long int dataB = 0;

    int bits_coordenada = 8;
    int bits_cor = 3;
    int bits_opcode = 4;

    // Barramento dataA
    dataA |= ((unsigned int)endereco ) << (bits_opcode); // Endereço
    dataA |= (opcode ); // Opcode

    //definindo dataB

    dataB |= ((unsigned int) coluna) << (bits_coordenada + 3 * bits_cor);
    dataB |= ((unsigned int) linha) << ( 3 * bits_cor);
    dataB |= ((unsigned int) b) << (2 * bits_cor);
    dataB |= ((unsigned int) g) << ( bits_cor);
    dataB |= ((unsigned int) r);

    //kernel(dataA, dataB);


}