#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>

/**
 * \brief           Abre arquivo GPU para comunicação com o Kernel
 * \param[out]      fd: Resultado se o arquivo foi aberto com sucesso
 * \return          Inteiro que sinaliza se arquivo GPU foi aberto (-1 em caso de falha)
 */
int open_physical(int fd) {
    if ((fd = open("/dev/GPU", O_RDWR)) == -1) {
        printf("ERROR: could not open \"/dev/GPU\"...\n");
        return (-1);
    }
    return fd;
}

/**
 * \brief           Fecha arquivo GPU para comunicação com o Kernel
 * \param[out]       fd: Resultado se o arquivo foi aberto com sucesso
 * \return          Inteiro que sinaliza se arquivo GPU foi aberto (-1 em caso de falha)
 */
void close_physical(fd) {
    close(fd);
}

/**
 * \brief            [DEBUG] Função que imprime um binário de 32 bits da instrução
 * \param[out]       numero: Dados de 32 bits a ser impresso em binário
 */
void imprimirBinario_32(unsigned int numero) {
    // Tamanho do tipo unsigned int em bits
    int tamanho = sizeof(unsigned int) * 8;

    // Máscara para extrair cada bit
    unsigned int mascara = 1 << (tamanho - 1);

    // Loop para percorrer cada bit e imprimir 0 ou 1
    for (int i = 0; i < tamanho; i++)
    {
        // Use o operador ternário para imprimir 0 ou 1, dependendo do bit
        printf("%u", (numero & mascara) ? 1 : 0);

        // Desloca a máscara para a direita para verificar o próximo bit
        mascara >>= 1;
    }
    printf("\n");
}

/**
 * \brief            [DEBUG] Função que imprime um binário de 64 bits da instrução
 * \param[out]       numero: Dados de 64 bits a ser impresso em binário
 */
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

/**
 * \brief           Define um polígono na tela
 * \param[in]       forma: 1 bit, define se é um triângulo (1) ou quadrado (0)
 * \param[in]       r: Valor de até 3 bits que representa o vermelho
 * \param[in]       g: Valor de até 3 bits que representa o verde
 * \param[in]       b: Valor de até 3 bits que representa o azul
 * \param[in]       tamanho: 4 bits - Define o tamanho do polígono que varia de 20x20 a 160x160
 * \param[in]       x: 9 bits - Define o ponto de refência do polígono no eixo x 
 * \param[in]       y: 9 bits - Define o ponto de refência do polígono no eixo y 
 * \param[in]       endereco: 4 bits - Define a posição de memória que a instrução será armazenada
 * \return          Inteiro que sinaliza se arquivo GPU foi aberto (-1 em caso de falha)
 */
int dp(int forma, int r, int g, int b, int tamanho, long int x, long int y, unsigned int endereco) {
    
    // Define o opcode para indicar a instrução a ser executada
    unsigned int opcode = 0b0011;
    
    // Declarando barramentos - dataA: Endereço e opcode / dataB: Dados 
    unsigned long int dataA = 0;
    unsigned long int dataB = 0;

    // Define o número de bits para cada campo
    int bits_coordenada = 9;
    int bits_cor = 3;
    int bits_tamanho = 4;
    int bits_opcode = 4;

    // Definindo os dados a ser enviados ao dataA 
    dataA |= ((unsigned int) endereco) << (bits_opcode);
    dataA |= ((unsigned int) opcode);

    // Definindo os dados a ser enviados ao dataB
    dataB |= ((unsigned int) forma) << (2 * bits_coordenada + bits_tamanho + 3 * bits_cor);
    dataB |= ((unsigned int) b) << (2 * bits_cor + bits_tamanho + 2 * bits_coordenada);
    dataB |= ((unsigned int) g) << (bits_cor + bits_tamanho + 2 * bits_coordenada);
    dataB |= ((unsigned int) r) << (bits_tamanho + 2 * bits_coordenada);
    dataB |= ((unsigned int) tamanho) << (2 * bits_coordenada);
    dataB |= ((unsigned long int) y) << (bits_coordenada);
    dataB |= ((unsigned long int) x);

    // Concatena dataA e dataB em um único conjunto de bits
    unsigned long long dados = 0;
    dados |= ((unsigned long long int)dataB) << 32;
    dados |= dataA;
    
    /* DEBUG
        printf("POLIGONO\n");
        printf("dataA\n");
        imprimirBinario_32(dataA);
        printf("dataB\n");
        imprimirBinario_32(dataB);
        printf("\n");
        printf("dados\n");
        imprimirBinario_64(dados);
        printf("\n\n\n\n");
    */

    // Abre arquivo GPU para escrever os dados
    int fd;
    if ((fd = open_physical(fd)) == -1)
        return -1;

    ssize_t tam;
    
    //Escreve os dados no arquivo GPU para que o kernel tenha acesso
    tam = write(fd, &dados, sizeof(dados));
    
    /* DEBUG
        unsigned long long teste;
        tam = read(fd, &teste, sizeof(teste));
        imprimirBinario_64(teste);
    */

    //Fecha arquivo
    close_physical(fd);
}

/**
 * \brief           Configura valores RGB para o preenchimento de áreas do background (Blocos de 8x8)        
 * \param[in]       r: Valor de até 3 bits que representa o vermelho
 * \param[in]       g: Valor de até 3 bits que representa o verde
 * \param[in]       b: Valor de até 3 bits que representa o azul 
 * \param[in]       endereco: 12 bits - Endereço de memória referente a uma posição da tela, que vai de 0 a 4799
 * \return          Inteiro que sinaliza se arquivo GPU foi aberto (-1 em caso de falha)
 */
int wbm(int r, int g, int b, unsigned long int endereco)
{
    // Define o opcode para indicar a instrução a ser executada
    unsigned int opcode = 0b0010;
    
    // Definindo os barramentos
    unsigned long int dataA = 0;
    unsigned long int dataB = 0;

    // Define o número de bits para cada campo
    int bits_cor = 3;
    int bits_opcode = 4;

    // Definindo os dados a ser enviados ao dataA 
    dataA |= ((unsigned int)endereco) << (bits_opcode); // Endereço
    dataA |= (opcode);                                  // Opcode

    // Definindo os dados a ser enviados ao dataB
    dataB |= ((unsigned int)b) << (2 * bits_cor);
    dataB |= ((unsigned int)g) << (bits_cor);
    dataB |= ((unsigned int)r);

    // Concatena dataA e dataB em um único conjunto de bits
    unsigned long long dados = 0;
    dados |= ((unsigned long long int)dataB) << 32;
    dados |= dataA;
    
    /* DEBUG
        printf("WBM\n");
        printf("dataA\n");
        imprimirBinario_32(dataA);
        printf("dataB\n");
        imprimirBinario_32(dataB);
        printf("\n");
        printf("dados\n");
        imprimirBinario_64(dados);
        printf("\n\n\n\n");
    */

    // Abre arquivo GPU para escrever os dados
    int fd;
    if ((fd = open_physical(fd)) == -1)
        return -1;

    ssize_t tam;
    
    //Escreve os dados no arquivo GPU para que o kernel tenha acesso
    tam = write(fd, &dados, sizeof(dados));

    /* DEBUG
        unsigned long long teste;
        tam = read(fd, &teste, sizeof(teste));
        imprimirBinario_64(teste);
    */

    close_physical(fd);
}

/**
 * \brief           Modifica cor do background 
 * \param[in]       r: Valor de até 3 bits que representa o vermelho
 * \param[in]       g: Valor de até 3 bits que representa o verde
 * \param[in]       b: Valor de até 3 bits que representa o azul 
 * \return          Inteiro que sinaliza se arquivo GPU foi aberto (-1 em caso de falha)
 */
int wbr01(int r, int g, int b)
{
    // Define registrador (5 bits)
    unsigned int registrador = 0;

    // Definir o opcode para indicar a instrução a ser executada
    unsigned int opcode = 0b0000;

    // Declarando variáveis do barramento
    unsigned long int dataA = 0b0;
    unsigned long int dataB = 0b0;

    // Definir o número de bits para cada campo
    int bits_cor = 3;
    int bits_opcode = 4;

    // Barramento dataB
    dataB |= ((unsigned int) b) << (2 * bits_cor); // B
    dataB |= ((unsigned int) g) << (bits_cor);     // G
    dataB |= ((unsigned int) r);                   // R

    // Barramento dataA
    dataA |= ((unsigned int) registrador) << (bits_opcode); // Endereço
    dataA |= (opcode);                                     // Opcode

    // Concatena dataA e dataB em um único conjunto de bits
    unsigned long long dados = 0;
    dados |= ((unsigned long long int) dataB) << 32;
    dados |= dataA;
    
    /* DEBUG
        printf("background\n");
        printf("dataA\n");
        imprimirBinario_32(dataA);
        printf("dataB\n");
        imprimirBinario_32(dataB);
        printf("\n");
        printf("dados\n");
        imprimirBinario_64(dados);
        printf("\n\n\n");
    */

    // Abre arquivo GPU para escrever os dados
    int fd;
    if ((fd = open_physical(fd)) == -1) return -1;
    
    ssize_t tam;
    
    //Escreve os dados no arquivo GPU para que o kernel tenha acesso
    tam = write(fd, &dados, sizeof(dados));

    /* DEBUG
        unsigned long long teste;
        tam = read(fd, &teste, sizeof(teste));
        imprimirBinario_64(teste);
    */
    
    // Fech arquivo GPU 
    close_physical(fd);
}

/**
 * \brief           Definir uma sprite 
 * \param[in]       x: 10 bits - Define a coordenada x do sprite na tela
 * \param[in]       y: 10 bits - Define a coordenada y do sprite na tela
 * \param[in]       offset: 9 bits - Escolhe o sprite que está na memória.
 * \param[in]       sp: 1 bit - Permite habilitar/desabilitar o desenho de um sprite na tela.
 * \param[in]       registrador: 5 bit - Define em qual registrador os parâmetros de impressão serão armazenados.
 * \return          Inteiro que sinaliza se arquivo GPU foi aberto (-1 em caso de falha)
 */
int wbr02(long int x, long int y, long int offset, int sp, unsigned int registrador)
{
    // Define o opcode
    unsigned int opcode = 0b0000;

    // Declarando variáveis do barramento
    unsigned long int dataA = 0b0;
    unsigned long int dataB = 0b0;

    // Define o número de bits para cada campo
    int bits_cordenada = 10;
    int bits_offset = 9;
    int bits_opcode = 4;

    // Barramento dataB
    dataB |= ((unsigned int)sp) << (2 * bits_cordenada + bits_offset);
    dataB |= ((unsigned long int)x) << (bits_cordenada + bits_offset);
    dataB |= ((unsigned long int)y) << (bits_offset);
    dataB |= ((unsigned long int)offset);

    // Barramento dataA
    dataA |= ((unsigned int)registrador) << (bits_opcode); // Endereço
    dataA |= (opcode);                                     // Opcode
    
    //Concatena dataB e dataA em uma única cadeia de bits.
    unsigned long long dados = 0;
    dados |= ((unsigned long long int) dataB) << 32;
    dados |= dataA;
    
    /* DEBUG
        printf("WBR02 - ALGUMA COISA DE SPRITE\n");
        printf("dataA\n");
        imprimirBinario_32(dataA);
        printf("dataB\n");
        imprimirBinario_32(dataB);
        printf("\n");
        printf("dados\n");
        imprimirBinario_64(dados);
        printf("\n\n\n\n");
    */

    //Abre arquivo GPU
    int fd;
    if ((fd = open_physical(fd)) == -1)
        return -1;

    //Escreve os dados no arquivo GPU para que o kernel tenha acesso
    ssize_t tam;
    tam = write(fd, &dados, sizeof(dados));
    
    /* DEBUG
        unsigned long long teste;
        tam = read(fd, &teste, sizeof(teste));
        imprimirBinario_64(teste);
    */

    //Fecha arquivo GPU
    close_physical(fd);
}

/**
 * \brief           Armazena ou modifica o conteúdo presente na Memória de Sprites
 * \param[in]       r: Valor de até 3 bits que representa o vermelho
 * \param[in]       g: Valor de até 3 bits que representa o verde
 * \param[in]       b: Valor de até 3 bits que representa o azul 
 * \param[in]       endereco: 14 bits - Especifica qual local da memoria será alterado  
 * \return          Inteiro que sinaliza se arquivo GPU foi aberto (-1 em caso de falha)
 */
int wsm(int r, int g, int b, unsigned int endereco)
{
    // endereço = 14 bits
    //  O ENDEREÇO É DEFINIDO AQUI OU É RECEBIDO COMO PARAMETRO?

    // Definir o opcode
    unsigned int opcode = 0b0001;
    // Declarando variaveis do barramento
    unsigned long int dataA = 0b0;
    unsigned long int dataB = 0b0;
    // Definir o número de bits para cada campo
    int bits_endereco = 14; // inútil
    int bits_cor = 3;
    int bits_opcode = 4;

    // Barramento dataB
    dataB |= ((unsigned int)b) << (2 * bits_cor); // B
    dataB |= ((unsigned int)g) << (bits_cor);     // G
    dataB |= ((unsigned int)r);                   // R

    // Barramento dataA
    dataA |= ((unsigned int)endereco) << (bits_opcode); // Endereço
    dataA |= (opcode);                                  // Opcode

    //Concatena dataB e dataA em uma única cadeia de bits.
    unsigned long long dados = 0;
    dados |= ((unsigned long long int)dataB) << 32;
    dados |= dataA;
    
    /* DEBUG
        printf("WSM\n");
        printf("dataA\n");
        imprimirBinario_32(dataA);
        printf("dataB\n");
        imprimirBinario_32(dataB);
        printf("\n");
        printf("dados\n");
        imprimirBinario_64(dados);
        printf("\n\n\n\n");
    */

    // Abre arquivo GPU 
    int fd;
    if ((fd = open_physical(fd)) == -1)
        return -1;

    ssize_t tam;
    
    //Escreve os dados no arquivo GPU para que o kernel tenha acesso
    tam = write(fd, &dados, sizeof(dados));
    
    /* DEBUG
        unsigned long long teste;
        tam = read(fd, &teste, sizeof(teste));
        imprimirBinario_64(teste);
    */

    //Fechando arquivo GPU 
    close_physical(fd);
}