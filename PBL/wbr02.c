#include <stdio.h>

// CONFIGURAR SPRITE

void wbr02(long int x,long int y, long int offset, int sp){
    //registrador = 5 bits
    unsigned int registrador = 0b0; // O REGISTRADOR É DEFINIDO AQUI OU É RECEBIDO COMO PARAMETRO?
    //ACHO QUE O REGISTRADOR TEM QUE SER UM PARÁMETRO
    // É UMA SPRITE POR REGISTRADOR?


    // Definir o opcode
    unsigned int opcode = 0b0000;
    //Declarando variaveis do barramento
    unsigned long int dataA = 0b0; 
    unsigned long int dataB = 0b0;

    // Definir o número de bits para cada campo
    int bits_cordenada = 10;
    int bits_offset = 9;
    int bits_opcode = 4;

    // Barramento dataB
    dataB |= ((unsigned int)sp) <<(2 * bits_cordenada + bits_offset);
    dataB |= ((unsigned long int)x) << (bits_cordenada + bits_offset);
    dataB |= ((unsigned long int)y) << (bits_offset);
    dataB |= ((unsigned long int)offset);

    // Barramento dataA
    dataA |= ((unsigned int)registrador ) << (bits_opcode); // Endereço
    dataA |= (opcode ); // Opcode
    //kernel(dataA, dataB);
}