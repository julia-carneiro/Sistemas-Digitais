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
  pthread_t thread_mouse;
  if(pthread_create(&thread_mouse, NULL, movimento_mouse, NULL)){
    printf("Erro ao criar thread.");
  }

  // X = 640 Y = 480
  // O pixel de erferencia da sprite é o superior esquerdo
  // Sempre subtrair/adicionar 10 para centralizar, e 20 para limitar os cantos1
  // Ex: Canto inferior direito X = 620 Y = 460
  
  gerar_cenario();

  srand(time(NULL));
  int random_x_position;
  int vetor_pos_x[4];
  int vetor_pox_y[4]= {0,0,0,0};
  int vetor_criado_y[4] = {0,0,0,0};
  int move = 0;
  int verifica;
 


    for(int i = 0; i <= 4; i++){
        random_x_position = randomInt();
        vetor_pos_x[i] = random_x_position;
    }
  while(1){
    wbr01(3, 3, 3);
    int position = 0;
    // Sprite Mouse
    move++;
    for (int i = 0;i<4;i++){

        if(vetor_pox_y[i] == 460){
          random_x_position = randomInt();
          vetor_pos_x[i] = random_x_position;
          vetor_pox_y[i] = 0;
        }
    }
    if(!vetor_criado_y[0] || !vetor_criado_y[1] || !vetor_criado_y[2] || !vetor_criado_y[3]){

        if(move == 10 && vetor_criado_y[0] == 0){
            
            wbr02(vetor_pos_x[0], vetor_pox_y[0], 19, 1, 5);
            vetor_criado_y[0]=1;
        }
        if(move == 200 && vetor_criado_y[1] == 0){
            wbr02(vetor_pos_x[1], vetor_pox_y[1], 19, 1, 2);
            vetor_criado_y[1]=1;
            
        }
        if(move == 100 && vetor_criado_y[2] == 0){
        
            wbr02(vetor_pos_x[2], vetor_pox_y[2], 19, 1, 3);
            vetor_criado_y[2]=1;
            
        }
        if(move == 350 && vetor_criado_y[3] == 0){
            wbr02(vetor_pos_x[3], vetor_pox_y[3], 19, 1, 4);
            vetor_criado_y[3]=1;
            
        }
    }
    
    

    for(int j = 0; j<4; j++){
        
        if(vetor_criado_y[j]==1){
            vetor_pox_y[j]++;
        }
    }  
    // Sprite Mouse   
    wbr02(pos_x, pos_y, 18, 1, 1);
    // Sprites carros
    wbr02(vetor_pos_x[0], vetor_pox_y[0], 19, 1, 5);//carro 5
    wbr02(vetor_pos_x[1], vetor_pox_y[1], 19, 1, 2);//carro 2
    wbr02(vetor_pos_x[2], vetor_pox_y[2], 19, 1, 3);//carro 3
    wbr02(vetor_pos_x[3], vetor_pox_y[3], 19, 1, 4);//carro 4



//Verificar colisão com carro 5
    if((pos_x + 20 > vetor_pos_x[0] && pos_x +20 < vetor_pos_x[0]+ 20 ) || (pos_x > vetor_pos_x[0] && pos_x < vetor_pos_x[0] + 20)){
      if((pos_y >vetor_pox_y[0] && pos_y < vetor_pox_y[0] + 20)|| (pos_y +20 > vetor_pox_y[0]  && pos_y +20 < vetor_pox_y[0]+20)){
        wbr01(7, 0, 0);
      }
    }
    //Verificar colisão com carro 2
    if((pos_x + 20 > vetor_pos_x[1] && pos_x +20 < vetor_pos_x[1]+ 20 ) || (pos_x > vetor_pos_x[1] && pos_x < vetor_pos_x[1] + 20)){
      if((pos_y >vetor_pox_y[1] && pos_y < vetor_pox_y[1] + 20)|| (pos_y +20 > vetor_pox_y[1]  && pos_y +20 < vetor_pox_y[1]+20)){
        wbr01(7, 0, 0);
      }
    }

     //Verificar colisão com carro 3
    if((pos_x + 20 > vetor_pos_x[2] && pos_x +20 < vetor_pos_x[2]+ 20 ) || (pos_x > vetor_pos_x[2] && pos_x < vetor_pos_x[2] + 20)){
      if((pos_y >vetor_pox_y[2] && pos_y < vetor_pox_y[2] + 20)|| (pos_y +20 > vetor_pox_y[2]  && pos_y +20 < vetor_pox_y[2]+20)){
        wbr01(7, 0, 0);
      }
    }

     //Verificar colisão com carro 4
    if((pos_x + 20 > vetor_pos_x[3] && pos_x +20 < vetor_pos_x[3]+ 20 ) || (pos_x > vetor_pos_x[3] && pos_x < vetor_pos_x[3] + 20)){
      if((pos_y >vetor_pox_y[3] && pos_y < vetor_pox_y[3] + 20)|| (pos_y +20 > vetor_pox_y[3]  && pos_y +20 < vetor_pox_y[3]+20)){
        wbr01(7, 0, 0);
      }
    }



//     if (pos_x < vetor_pos_x[0] + 20 &&
//     pos_x + 18 > vetor_pos_x[0] &&
//     pos_y < vetor_pox_y[0] + 20 &&
//     pos_y + 1 > vetor_pox_y[0]) {
//     // Houve colisão entre a sprite 1 e a sprite 5
//     printf("houve colisao");
// }



  }


  pthread_join(thread_mouse,NULL);
  return 0;
}
