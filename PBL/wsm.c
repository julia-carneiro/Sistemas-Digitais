#include <stdio.h>

//ESCRITA NA MEMÓRIA DE SPRITES

void wsm(int r, int g, int b){
    //endereço = 14 bits
    unsigned int endereco = 0b00000000; // O ENDEREÇO É DEFINIDO AQUI OU É RECEBIDO COMO PARAMETRO?

    // Definir o opcode
    unsigned int opcode = 0b0001;
    //Declarando variaveis do barramento
    unsigned long int dataA = 0b0; 
    unsigned long int dataB = 0b0;
    // Definir o número de bits para cada campo
    int bits_endereco = 14;//inútil
    int bits_cor = 3;
    int bits_opcode = 4;

    // Barramento dataB
    dataB |= ((unsigned int)b) << ( 2 * bits_cor  ); // B
    dataB |= ((unsigned int)g ) << (bits_cor); // G
    dataB |= ((unsigned int)r); // R

    // Barramento dataA
    dataA |= ((unsigned int)endereco ) << (bits_opcode); // Endereço
    dataA |= (opcode ); // Opcode

    //kernel(dataA, dataB);

    //RETORNAR O VALORES DO BARRAMENTO OU CHAMAR O KERNEL AQUI?
    return 0;
}