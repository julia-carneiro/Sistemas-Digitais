#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>

int FD;
/* Open /dev/mem to give access to physical addresses */
int open_physical (int fd) {
  
        if ((fd = open( "/dev/GPU", O_RDWR )) == -1) {
            printf ("ERROR: could not open \"/dev/GPU\"...\n");
            return (-1);
        }
    FD = fd;
    return fd;
}

void close_physical(fd){
    close(fd);
}

void imprimirBinario_32(unsigned int numero) {
    // Tamanho do tipo unsigned int em bits
    int tamanho = sizeof(unsigned int) * 8;

    // Máscara para extrair cada bit
    unsigned int mascara = 1 << (tamanho - 1);

    // Loop para percorrer cada bit e imprimir 0 ou 1
    for (int i = 0; i < tamanho; i++) {
        // Use o operador ternário para imprimir 0 ou 1, dependendo do bit
        printf("%u", (numero & mascara) ? 1 : 0);
        
        // Desloca a máscara para a direita para verificar o próximo bit
        mascara >>= 1;
    }
    printf("\n");
}


void imprimirBinario_64(unsigned long long int numero) {
    // Tamanho do tipo unsigned long long em bits
    int tamanho = sizeof(unsigned long long int) * 8;

    // Máscara para extrair cada bit
    unsigned long long int mascara = 1ULL << (tamanho - 1);

    // Loop para percorrer cada bit e imprimir 0 ou 1
    for (int i = 0; i < tamanho; i++) {
        // Use o operador ternário para imprimir 0 ou 1, dependendo do bit
        printf("%llu", (numero & mascara) ? 1ULL : 0ULL);
        
        // Desloca a máscara para a direita para verificar o próximo bit
        mascara >>= 1;
    }
    printf("\n");
}

// DEFINE UM POLIGONO
int dp(int forma,int r, int g, int b,int tamanho,long int x, long int y,unsigned int endereco ){
    //endereço = 4 bits
    // unsigned int endereco = 0b0; // O ENDEREÇO É DEFINIDO AQUI OU É RECEBIDO COMO PARAMETRO?
     
    // Definir o opcode
    unsigned int opcode = 0b0011;
    
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

    unsigned long long  dados = 0;
    dados |= ((unsigned long long int) dataB) << 32;
    dados |= dataA;
    printf("POLIGONO\n");
    printf("dataA\n");
    imprimirBinario_32(dataA);
    printf("dataB\n");
    imprimirBinario_32(dataB);
    printf("\n");
    printf("dados\n");
    imprimirBinario_64(dados); 
    printf("\n\n\n\n");

    int fd;

    if ((fd = open_physical (fd)) == -1) return -1;
    
    ssize_t tam;
    tam = write(fd, &dados, sizeof(dados));
    unsigned long long  teste;

    tam = read(fd,&teste, sizeof(teste));
    imprimirBinario_64(teste);
    close_physical(fd);

}

void wbm(int r, int g, int b,unsigned long int endereco ){
    // endereço  = 12 bits

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

    // dataB |= ((unsigned int) coluna) << (bits_coordenada + 3 * bits_cor);
    // dataB |= ((unsigned int) linha) << ( 3 * bits_cor);
    dataB |= ((unsigned int) b) << (2 * bits_cor);
    dataB |= ((unsigned int) g) << ( bits_cor);
    dataB |= ((unsigned int) r);

    //kernel(dataA, dataB);
     unsigned long long  dados = 0;
    dados |= ((unsigned long long int) dataB) << 32;
    dados |= dataA;
    printf("WBM\n");
    printf("dataA\n");
    imprimirBinario_32(dataA);
    printf("dataB\n");
    imprimirBinario_32(dataB);
    printf("\n");
    printf("dados\n");
    imprimirBinario_64(dados); 
    printf("\n\n\n\n");

    int fd;

    if ((fd = open_physical (fd)) == -1) return -1;
    
    ssize_t tam;
    tam = write(fd, &dados, sizeof(dados));
    unsigned long long  teste;

    tam = read(fd,&teste, sizeof(teste));
    imprimirBinario_64(teste);
    close_physical(fd);

}

// MODIFICAR COR DE BACKGROUND
int wbr01(int r, int g, int b){
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

    unsigned long long  dados = 0;
    dados |= ((unsigned long long int) dataB) << 32;
    dados |= dataA;
    printf("background\n");
    printf("dataA\n");
    imprimirBinario_32(dataA);
    printf("dataB\n");
    imprimirBinario_32(dataB);
    printf("\n");
    printf("dados\n");
    imprimirBinario_64(dados); 
    printf("\n\n\n");
    //kernel(dataA, fddataB);
    
 
    int fd;

    if ((fd = open_physical (fd)) == -1) return -1;
    
    ssize_t tam;
    tam = write(fd, &dados, sizeof(dados));
    unsigned long long  teste;

    tam = read(fd,&teste, sizeof(teste));
    // imprimirBinario(teste);
    close_physical(fd);

}


// CONFIGURAR SPRITE
int wbr02(long int x,long int y, long int offset, int sp, unsigned int registrador ){
    //registrador = 5 bits
   // O REGISTRADOR É DEFINIDO AQUI OU É RECEBIDO COMO PARAMETRO?
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
     unsigned long long  dados = 0;
    dados |= ((unsigned long long int) dataB) << 32;
    dados |= dataA;
    printf("WBR02 - ALGUMA COISA DE SPRITE\n");
    printf("dataA\n");
    imprimirBinario_32(dataA);
    printf("dataB\n");
    imprimirBinario_32(dataB);
    printf("\n");
    printf("dados\n");
    imprimirBinario_64(dados); 
    printf("\n\n\n\n");

    int fd;

    if ((fd = open_physical (fd)) == -1) return -1;
    
    ssize_t tam;
    tam = write(fd, &dados, sizeof(dados));
    unsigned long long  teste;

    tam = read(fd,&teste, sizeof(teste));
    imprimirBinario_64(teste);
    close_physical(fd);
}

//ESCRITA NA MEMÓRIA DE SPRITES
void wsm(int r, int g, int b,unsigned int endereco ){
    //endereço = 14 bits
    // O ENDEREÇO É DEFINIDO AQUI OU É RECEBIDO COMO PARAMETRO?

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

     unsigned long long  dados = 0;
    dados |= ((unsigned long long int) dataB) << 32;
    dados |= dataA;
    printf("WSM\n");
    printf("dataA\n");
    imprimirBinario_32(dataA);
    printf("dataB\n");
    imprimirBinario_32(dataB);
    printf("\n");
    printf("dados\n");
    imprimirBinario_64(dados); 
    printf("\n\n\n\n");

    int fd;

    if ((fd = open_physical (fd)) == -1) return -1;
    
    ssize_t tam;
    tam = write(fd, &dados, sizeof(dados));
    unsigned long long  teste;

    tam = read(fd,&teste, sizeof(teste));
    imprimirBinario_64(teste);
    close_physical(fd);

    //RETORNAR O VALORES DO BARRAMENTO OU CHAMAR O KERNEL AQUI?
    
}