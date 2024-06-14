#include "biblioteca.c"

int main() {

  printf("\nIniciando impressão...\n");

  //Limpa os blocos de bg (wbm)
  // for (int i = 0; i < 4799; i++) {
  //  wbm(0b110, 0b111, 0b111, i);
  //}

  // Edita um sprite - Bandeira
  for (int i = 1; i <= 200; i++) {
    wsm(0, 7, 0, i);
  }

  for (int j = 200; j <= 400; j++) {
    wsm(0, 7, 7, j);
  }

  // Adiciona sprite editado
  wbr02(380, 40, 0, 1, 14);

  // Define background cinza escuro
  wbr01(3, 3, 3);

  // Criando a grama
  for (int j = 0; j < 4800; j += 80) {
    for (int i = 0; i < 10; i++) {
      wbm(0,7,0, j + i);
    }
    for (int k = 69; k < 80; k++) {
      wbm(0, 7, 0, j + k);
    }
  }

  // Criando faixa quadriculada
  // Faixa da esquerda
  // Parte vermelha
  for (int j = 0; j < 4800; j += 160) {
    for (int i = 10; i < 12; i++) {
      wbm(7, 0, 0, j + i);
    }
  }

  // Parte branca
  for (int j = 80; j < 4800; j += 160) {
    for (int i = 10; i < 12; i++) {
      wbm(7, 7, 7, j + i);
    }
  }

  // Faixa da direita
  // Parte vermelha
  for (int j = 0; j < 4800; j += 160) {
    for (int i = 67; i < 69; i++) {
      wbm(7, 0, 0, j + i);
    }
  }
  // Parte branca
  for (int j = 80; j < 4800; j += 160) {
    for (int i = 67; i < 69; i++) {
      wbm(7, 7, 7, j + i);
    }
  }

  // Define sprite de troféu
  wbr02(320, 40, 24, 1, 11);

  // CARRO
  dp(1, 7, 7, 7, 0b0101, 320, 240, 0); // Triangulos brancos
  dp(0, 7, 0, 0, 0b0011, 320, 250, 1); // Base do meio azul
  dp(0, 7, 0, 0, 0b0101, 320, 300, 2); // Base inferior
  dp(0, 0, 0, 0, 0b0001, 280, 300, 3); // Roda traseira esquerda
  dp(0, 0, 0, 0, 0b0001, 360, 300, 4); // Roda traseira direita
  dp(0, 7, 0, 0, 0b0011, 320, 220, 5); // Topo azul
  dp(1, 7, 0, 0, 0b0010, 320, 190, 6); // Ponta azul
  dp(0, 0, 0, 0, 0b0001, 350, 220, 7); // Roda dianteira direita
  dp(0, 0, 0, 0, 0b0001, 290, 220, 8); // Roda dianteria esquerda

  //Desenha árvores    
  wbr02(20, 160, 16, 1, 2);
  wbr02(50, 240, 4, 1, 3);
  wbr02(570, 240, 4, 1, 6);
  wbr02(580, 120, 4, 1, 7);
  wbr02(585, 360, 4, 1, 8);
  wbr02(585, 40, 16, 1, 9);
  wbr02(50, 370, 4, 1, 10);
  wbr02(50, 40, 4, 1, 1);

  // Faz movimentação dos carros menores 
  
  for (int j = 3200; j >= 0; j--) {
    wbr02(460, j + 160, 17, 1, 5);
    wbr02(150, j + 380, 17, 1, 12);
    wbr02(460, j + 360, 17, 1, 13);
    wbr02(150, j + 180, 17, 1, 4);
  }
  
  printf("\nImagem finalizada!\n");

  return 0;

}

