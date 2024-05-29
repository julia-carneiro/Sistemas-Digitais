#include <stdio.h>
#include <asm/uacess.h>

// DEFINE UM POLIGONO
void dp(int forma,int r, int g, int b,int tamanho,long int x, long int y){
    //endereço = 4 bits
    unsigned int endereco = 0b0; // O ENDEREÇO É DEFINIDO AQUI OU É RECEBIDO COMO PARAMETRO?

    // Definir o opcode
    unsigned int opcode = 0b0011;
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