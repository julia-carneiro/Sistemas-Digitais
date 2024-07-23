#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> 
#include <linux/input.h>
#include "biblioteca.c"

struct input_event event;
int mouse_x,mouse_y;
int pos_x = 300;
int pos_y = 400;
int pos_y_tiro,click,pos_x_tiro; 
int limites_x[2] = {80,532};
int limites_y[2] = {0,460};

int verificar_limite(int limite[], int valor_atual, int valor_mouse);

void *movimento_mouse(){
  int fd = open("/dev/input/event0", O_RDONLY);
  if(fd == -1){
      printf("Nao foi possivel abrir o arquivo!\n");
  }

  while (1) {
    read(fd, &event, sizeof(struct input_event));

    if(event.type== EV_REL && event.code == REL_X){
        mouse_x = event.value;
        pos_x = verificar_limite(limites_x, pos_x, mouse_x);
    }

    if(event.type== EV_REL && event.code == REL_Y){
        mouse_y = event.value;
        pos_y = verificar_limite(limites_y, pos_y, mouse_y);
    }

    if (event.type == EV_KEY && event.code == BTN_LEFT)
    {
      if (event.value == 1) {
        click = 1;
      }
      else{
        click=0;
      }
    }
  }
  
  close(fd);
  return NULL;

}


int verificar_limite(int limite[], int valor_atual, int valor_mouse){
  // Verifica se o movimento é maior que o limite inferior e menor que o limite superior
  if(valor_atual + valor_mouse >= limite[0] && valor_atual + valor_mouse <= limite[1]){
    return valor_atual + valor_mouse;
  }
  else if(valor_atual + valor_mouse < limite[0]){
    return limite[0];
  }
  else{
    return limite[1];
  }
}

void gerar_cenario(){
  // Cor do Fundo
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
  
  // Inicia sprite do Carro
  wbr02(pos_x, pos_y, 18, 1, 1);
}

int randomInt() {
  return 100 + rand() % (520 - 100 + 1);
}

  
int main() {
  // X = 640 Y = 480
  // O pixel de erferencia da sprite é o superior esquerdo
  // Sempre subtrair/adicionar 10 para centralizar, e 20 para limitar os cantos1
  // Ex: Canto inferior direito X = 620 Y = 460
  
  pthread_t thread_mouse;
  if(pthread_create(&thread_mouse, NULL, movimento_mouse, NULL)){
    printf("Erro ao criar thread.");
  }


  while(1){
    srand(time(NULL));
    int random_x_position;

    // Variáveis das Sprites dos Carros
    int vetor_pos_x[4];
    int vetor_pos_y[4]= {0,0,0,0};
    int vetor_criado_y[4] = {0,0,0,0};

    //Variáveis Sprite Gasolina
    int pos_x_gasolina = 0;
    int pos_y_gasolina = 0;
    int gasolina_criada = 0;

    //Variáveis Sprite Pontos

    // Variavéis dos Jogo
    int tempo = 0;
    int vida = 3;
    int gasolina = 100;
    int velocidade = 1;

    // Posição inicial do Mouse
    pos_x = 300;
    pos_y = 400;

    gerar_cenario();  
    while(vida && gasolina){
      // Movimenta Sprite Mouse   
      wbr02(pos_x, pos_y, 18, 1, 1);

      // Define quando as Sprite vão ser geradas
      tempo++;
      if(tempo % 1000 == 0){
        velocidade++;
      }
      
      //Move o Tiro
      if(pos_y_tiro == 0){
        wbr02(pos_x_tiro, pos_y_tiro, 11, 0, 6);
      }
      else{
        pos_y_tiro-=5;
        wbr02(pos_x_tiro, pos_y_tiro, 11, 1, 6);
      }

      // Atira se houver clique e não houver um tiro já gerado
      if(click && pos_y_tiro <= 0){
        pos_y_tiro = pos_y-19;
        pos_x_tiro = pos_x;
        wbr02(pos_x_tiro, pos_y_tiro, 11, 1, 6);
      }

      //Define posição aleatoria do carro
      for (int i = 0;i<4;i++){
          if(vetor_pos_y[i] >= 460){
            random_x_position = randomInt();
            vetor_pos_x[i] = random_x_position;
            vetor_pos_y[i] = 0;
          }
      }

      // Define posição aleatória Gasolina
      if(pos_y_gasolina == 0 && gasolina_criada == 0){
        pos_x_gasolina = randomInt();
      }

      //Gera Sprite Gasolina
      if(tempo % 350 == 0 && gasolina_criada == 0){
        gasolina_criada = 1;
        wbr02(pos_x_gasolina, pos_y_gasolina, 0, 1, 7);
      }

      //Movimenta Sprite Gasolina
      if(gasolina_criada){
        if(pos_y_gasolina >= 460){
          gasolina_criada = 0;
          
        }
        else{
          pos_y_gasolina+=velocidade;
          wbr02(pos_x_gasolina, pos_y_gasolina, 0, 1, 7);
        }
      }else{
        pos_y_gasolina = 0; 
        wbr02(pos_x_gasolina, pos_y_gasolina, 0, 0, 7);
      }

      // Consome gasolina
      if(tempo % 300 == 0){
        gasolina-=10;
        printf("%d\n", gasolina); 
      }

      // Colisão Carro - Gasolina
      if((pos_x + 20 > pos_x_gasolina && pos_x +20 < pos_x_gasolina+ 20 ) || (pos_x > pos_x_gasolina && pos_x < pos_x_gasolina + 20)){
        if((pos_y >pos_y_gasolina && pos_y < pos_y_gasolina + 20)|| (pos_y +20 > pos_y_gasolina  && pos_y +20 < pos_y_gasolina+20)){
          if(gasolina < 100){
            gasolina+=10;
          }
          gasolina_criada = 0;
        
          printf("Pegou!: %d\n", gasolina); 
        }
      }

      //Colisão Gasolina - Tiro
      if((pos_x_tiro + 10 >= pos_x_gasolina && pos_x_tiro + 9 <= pos_x_gasolina+20) || 
      (pos_x_tiro+9>=pos_x_gasolina && pos_x_tiro+10<=pos_x_gasolina+20)){
        if (pos_y_tiro <= pos_y_gasolina + 19 && pos_y_tiro >= pos_y_gasolina){
          gasolina_criada = 0;
          pos_y_tiro = 0;
        }
      }

      //Cria os carros
      if(!vetor_criado_y[0] || !vetor_criado_y[1] || !vetor_criado_y[2] || !vetor_criado_y[3]){

          if(tempo == 10 && vetor_criado_y[0] == 0){
              
              wbr02(vetor_pos_x[0], vetor_pos_y[0], 19, 1, 5);
              vetor_criado_y[0]=1;
          }
          if(tempo == 200 && vetor_criado_y[1] == 0){
              wbr02(vetor_pos_x[1], vetor_pos_y[1], 19, 1, 2);
              vetor_criado_y[1]=1;
              
          }
          if(tempo == 100 && vetor_criado_y[2] == 0){
          
              wbr02(vetor_pos_x[2], vetor_pos_y[2], 19, 1, 3);
              vetor_criado_y[2]=1;
              
          }
          if(tempo == 350 && vetor_criado_y[3] == 0){
              wbr02(vetor_pos_x[3], vetor_pos_y[3], 19, 1, 4);
              vetor_criado_y[3]=1;
              
          }
      }
      
      // Define a próxima posição das Sprites dos Carros em Y
      for(int j = 0; j<4; j++){
          //Verifica carro criado 
          if(vetor_criado_y[j]==1){
              vetor_pos_y[j]+= velocidade;
          }
      }  
      
      // Movimenta Sprites dos Carros
      for (int k = 0; k < 4; k++){
        wbr02(vetor_pos_x[k], vetor_pos_y[k], 19, vetor_criado_y[k], 2+k);
      }
      
      //Colisão carro - carro 
      for (int l = 0; l < 4; l++){
        if((pos_x + 20 > vetor_pos_x[l] && pos_x +20 < vetor_pos_x[l]+ 20 ) || (pos_x > vetor_pos_x[l] && pos_x < vetor_pos_x[l] + 20)){
          if((pos_y >vetor_pos_y[l] && pos_y < vetor_pos_y[l] + 20)|| (pos_y +20 > vetor_pos_y[l]  && pos_y +20 < vetor_pos_y[l]+20)){
            wbr01(7, 0, 0);
            vetor_pos_y[l] = 460;
            vida--;
            wbr01(3, 3, 3);
          }
        }
      }

      //Colisão Carro - Tiro
      for (int l = 0; l < 4; l++){
        if((pos_x_tiro + 10 >= vetor_pos_x[l] && pos_x_tiro + 9 <= vetor_pos_x[l]+20) || 
        (pos_x_tiro+9>=vetor_pos_x[l] && pos_x_tiro+10<=vetor_pos_x[l]+20)){
          if (pos_y_tiro <= vetor_pos_y[l] + 19 && pos_y_tiro >= vetor_pos_y[l]){
            vetor_pos_y[l] = 460;
            pos_y_tiro = 0;

          }
        }
      }

    }
  
    // Tela Game Over
    printf("Game-Over\n");

  }

  pthread_join(thread_mouse,NULL);
  return 0;
}
