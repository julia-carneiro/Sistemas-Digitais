
void gerar_cenario(){
  // Cor do Fundo
  wbr01(3, 3, 3);
  // Criando a grama
  for (int j = 0; j < 4800; j += 80) {
    for (int i = 0; i < 15; i++) {
      wbm(0,7,0, j + i);
    }
    for (int k = 64; k < 80; k++) {
      wbm(0, 7, 0, j + k);
    }
  }

  // Criando faixa quadriculada
  // Faixa da esquerda
  // Parte vermelha
  for (int j = 0; j < 4800; j += 160) {
    for (int i = 15; i < 17; i++) {
      wbm(7, 0, 0, j + i);
    }
  }

  // Parte branca
  for (int j = 80; j < 4800; j += 160) {
    for (int i = 15; i < 17; i++) {
      wbm(7, 7, 7, j + i);
    }
  }

  // Faixa da direita
  // Parte vermelha
  for (int j = 0; j < 4800; j += 160) {
    for (int i = 62; i < 64; i++) {
      wbm(7, 0, 0, j + i);
    }
  }
  // Parte branca
  for (int j = 80; j < 4800; j += 160) {
    for (int i = 62; i < 64; i++) {
      wbm(7, 7, 7, j + i);
    }
  }

}