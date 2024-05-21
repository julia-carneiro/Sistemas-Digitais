#include <stdio.h>

// Função para retornar a instrução binária
unsigned int instrucao(int r, int g, int b, int endereco) {
    // Checar limites dos valores
    if (r < 0 || r > 7 || g < 0 || g > 7 || b < 0 || b > 7 || endereco < -8192 || endereco > 8191) {
        return 0;
    }

    // Definir o opcode
    unsigned int opcode = 0b0001;

    // Definir o número de bits para cada campo
    int bits_endereco = 14;
    int bits_cor = 3;
    int bits_opcode = 4;

    // Combinação dos campos em um único número 
    unsigned int instrucao = 0;
    instrucao |= ((unsigned int)b) << (bits_opcode + bits_endereco + 2 * bits_cor  ); // B
    instrucao |= ((unsigned int)g ) << (bits_opcode + bits_endereco + bits_cor); // G
    instrucao |= ((unsigned int)r) << (bits_opcode + bits_endereco ); // R
    instrucao |= ((unsigned int)endereco ) << (bits_opcode); // Endereço
    instrucao |= (opcode ); // Opcode
    return instrucao;
}
void imprimirBinario(unsigned int numero) {
    // Tamanho do tipo unsigned int em bits
    int tamanho = sizeof(unsigned int) * 8;

    // Máscara para extrair cada bit
    unsigned int mascara = 1 << (tamanho - 1);

    // Loop para percorrer cada bit e imprimir 0 ou 1
    for (int i = 0; i < tamanho; i++) {
        // Use o operador ternário para imprimir 0 ou 1, dependendo do bit
        printf("%d", (numero & mascara) ? 1 : 0);
        
        // Desloca a máscara para a direita para verificar o próximo bit
        mascara >>= 1;
    }
    printf("\n");
}

int main() {
    int r = 5, g = 5, b = 6, endereco = 5;

    // Chamada da função
    unsigned int resultado = instrucao(r, g, b, endereco);

    printf("Instrução binária:\n");
    imprimirBinario(resultado);

    return 0;
}
