#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> 
#include <linux/input.h>
#include "biblioteca.c"
#include "sete_seg.c"
#include "cenario.c"
#include "sprite.c"


struct input_event event;
int mouse_x,mouse_y;
int pos_x = 300;
int pos_y = 400;
int signed pos_y_tiro;
int click,pos_x_tiro; 
int limites_x[2] = {120,490};
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
      if(event.value == 1 && click == 0) {
        click = 1;
      }
      else{
        click = 0;
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

int randomInt() {
  return 140 + rand() % (470 - 140 + 1);
}

int pontuacao(int pontos){
  int uni = pontos %10;
  int dez = (pontos / 10) % 10;
  int cem = (pontos / 100) % 10;

  ligar_7seg(cem,4);
  ligar_7seg(dez,3);
  ligar_7seg(uni,2);
}

int mostrar_gasosa(int vetor_gasolina[]){
  wbr02(520, 10, 0, vetor_gasolina[4], 11);
  wbr02(540, 10, 0,vetor_gasolina[3] , 12);  
  wbr02(560, 10, 0, vetor_gasolina[2], 13);
  wbr02(580, 10, 0, vetor_gasolina[1],14);
  wbr02(600, 10, 0, vetor_gasolina[0], 15);  
}

// int mostrar_vida(int vetor_vida[]){
//   wbr02(10, 10, 15, vetor_vida[0], 8);
//   wbr02(31, 10, 15, vetor_vida[1], 9);
//   wbr02(52, 10, 15, vetor_vida[2], 10);
// }

int gerar_carros(int tempo, int vetor_criado_y[], int vetor_pos_x[], int vetor_pos_y[]){
  if(tempo == 10 && vetor_criado_y[0] == 0){
    wbr02(vetor_pos_x[0], vetor_pos_y[0], 19, 0, 5);
    vetor_criado_y[0]=1;
  }
  if(tempo == 200 && vetor_criado_y[1] == 0){
    wbr02(vetor_pos_x[1], vetor_pos_y[1], 19, 0, 2);
    vetor_criado_y[1]=1;
  }
  if(tempo == 100 && vetor_criado_y[2] == 0){
    wbr02(vetor_pos_x[2], vetor_pos_y[2], 19, 0, 3);
    vetor_criado_y[2]=1;
      
  }
  if(tempo == 350 && vetor_criado_y[3] == 0){
    wbr02(vetor_pos_x[3], vetor_pos_y[3], 19, 0, 4);
    vetor_criado_y[3]=1;
  }
}

int resetar_sprites(){
  wbr02(1, 1, 19, 0, 2);
  wbr02(1, 1, 19, 0, 3);
  wbr02(1, 1, 19, 0, 4);
  wbr02(1, 1, 19, 0, 5);
}

int main() {
  // X = 640 Y = 480
  // O pixel de erferencia da sprite é o superior esquerdo
  // Sempre subtrair/adicionar 10 para centralizar, e 20 para limitar os cantos1
  // Ex: Canto inferior direito X = 620 Y = 460
  srand(time(NULL));
  int random_x_position;
  
  pthread_t thread_mouse;
  if(pthread_create(&thread_mouse, NULL, movimento_mouse, NULL)){
    printf("Erro ao criar thread.");
  }

  gerar_sprite();
  gerar_cenario();  

  while(1){
    // Variáveis das Sprites dos Carros
    int vetor_pos_x[4];
    int vetor_pos_y[4]= {0,0,0,0};
    int vetor_criado_y[4] = {0,0,0,0};
    int colidiu = 0; 

    //Variáveis Sprite Gasolina
    int pos_x_gasolina = 0;
    int pos_y_gasolina = 0;
    int gasolina_criada = 0;

    // Variavéis dos Jogo
    int pontos = -1;
    int tempo = 0;
    int vida = 3;
    int vetor_vida[] = {1,1,1};
    int gasolina = 5;
    int vetor_gasolina[] = {1,1,1,1,1};
    int velocidade = 1;

    // Posição inicial do Mouse
    pos_x = 300;
    pos_y = 400;

    mostrar_gasosa(vetor_gasolina);
    // mostrar_vida(vetor_vida);
    pontuacao(pontos);
    ligar_7seg(vida, 0);
    resetar_sprites();
    
    while(vida && gasolina){
      // Movimenta Sprite Mouse   
      wbr02(pos_x, pos_y, 18, 1, 1);
  
      // Define quando as Sprite vão ser geradas
      tempo++;
      if(tempo % 1000 == 0){
        velocidade++;
      }
      
      // Atira se houver clique e não houver um tiro já gerado
      if(click && pos_y_tiro <= 0){
        pos_y_tiro = pos_y-19;
        pos_x_tiro = pos_x;
        wbr02(pos_x_tiro, pos_y_tiro, 11, 1, 6);
      }

      //Move o Tiro
      if(pos_y_tiro <= 0){
        wbr02(pos_x_tiro, pos_y_tiro, 11, 0, 6);
      }
      else{
        pos_y_tiro-=5;
        wbr02(pos_x_tiro, pos_y_tiro, 11, 1, 6);
      }

      //Define posição aleatoria do carro
      for (int i = 0;i<4;i++){
          if(vetor_pos_y[i] == 0 || vetor_pos_y[i] >= 460){
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
      if(tempo % 500 == 0){
        vetor_gasolina[gasolina-1] = 0;
        gasolina -=1;
        mostrar_gasosa(vetor_gasolina);
        printf("%d\n", gasolina); 
      }

      // Colisão Carro - Gasolina
      if((pos_x + 20 > pos_x_gasolina && pos_x +20 < pos_x_gasolina+ 20 ) || (pos_x > pos_x_gasolina && pos_x < pos_x_gasolina + 20)){
        if((pos_y >pos_y_gasolina && pos_y < pos_y_gasolina + 20)|| (pos_y +20 > pos_y_gasolina  && pos_y +20 < pos_y_gasolina+20)){
          if(gasolina < 5){
            gasolina +=1;
            vetor_gasolina[gasolina - 1]=1;
            mostrar_gasosa(vetor_gasolina);
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
        gerar_carros(tempo, vetor_criado_y, vetor_pos_x, vetor_pos_y);
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
        if (vetor_criado_y[k]){
          wbr02(vetor_pos_x[k], vetor_pos_y[k], 19, vetor_criado_y[k], 2+k);
        }
      }
      
      //Colisão carro - carro 
      for (int l = 0; l < 4; l++){
        if((pos_x + 20 > vetor_pos_x[l] && pos_x +20 < vetor_pos_x[l]+ 20 ) || (pos_x > vetor_pos_x[l] && pos_x < vetor_pos_x[l] + 20)){
          if((pos_y >vetor_pos_y[l] && pos_y < vetor_pos_y[l] + 20)|| (pos_y +20 > vetor_pos_y[l]  && pos_y +20 < vetor_pos_y[l]+20)){
            wbr01(7, 0, 0);
            vetor_pos_y[l] = 460;
            vetor_vida[vida - 1] = 0;
            vida--;
            wbr01(3, 3, 3);

            //Mostra vida
            ligar_7seg(vida, 0);
            // mostrar_vida(vetor_vida);
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
            colidiu = 1;
          }
        }
      }
      
      if (colidiu == 1){
        pontos += 1;
        colidiu = 0;  
        pontuacao(pontos);
      }
    }
    // Tela Game Over
    printf("Game-Over\n");
  }

  pthread_join(thread_mouse,NULL);
  return 0;
}
