#include <stdio.h>

// MODIFICAR COR DE BACKGROUND

void wbr01(int r, int g, int b){
    //registrador = 5 bits
    unsigned int registrador = 0b0; // O REGISTRADOR É DEFINIDO AQUI OU É RECEBIDO COMO PARAMETRO?

    // Definir o opcode
    unsigned int opcode = 0b0000;
    //Declarando variaveis do barramento
    unsigned long int dataA = 0b0; 
    unsigned long int dataB = 0b0;
    // Definir o número de bits para cada campo
    int bits_registrador = 5;// inútil
    int bits_cor = 3;
    int bits_opcode = 4;

    // Barramento dataB
    dataB |= ((unsigned int)b) << ( 2 * bits_cor  ); // B
    dataB |= ((unsigned int)g ) << (bits_cor); // G
    dataB |= ((unsigned int)r); // R

    // Barramento dataA
    dataA |= ((unsigned int)registrador ) << (bits_opcode); // Endereço
    dataA |= (opcode ); // Opcode
    //kernel(dataA, dataB);

    //RETORNAR O VALORES DO BARRAMENTO OU CHAMAR O KERNEL AQUI?
    return 0;
}