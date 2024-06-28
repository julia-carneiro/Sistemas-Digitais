
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> 
#include <linux/input.h>
#include "biblioteca.c"

struct input_event event;

int verificar_limite(int limite[], int valor_atual, int valor_mouse){
  if(valor_atual + valor_mouse >= limite[0] && valor_atual + valor_mouse <= limite[1]){
    return valor_mouse;
  }
  else{
    return limite - valor_atual;
  }
}


int main() { 
  int fd = open("/dev/input/event0", O_RDONLY);
  if(fd == -1){
      printf("Nao foi possivel abrir o arquivo!\n");
  }
  
  // X = 640 Y = 480
  // O pixel de erferencia da sprite é o superior esquerdo
  // Sempre subtrair/adicionar 10 para centralizar, e 20 para limitar os cantos1
  // Ex: Canto inferior direito X = 620 Y = 460

  int mouse_x,mouse_y,pos_x,pos_y, velocidade;
  pos_x = 620;
  pos_y = 460;

  velocidade = 1;

  int limites_x[2] = {20,620};

  wbr02(pos_x, pos_y, 1, 1, 1);
  while (1) {
      read(fd, &event, sizeof(struct input_event));
      // Processar o evento aqui
      wbr02(pos_x, pos_y, 24, 1, 1);

      if(event.type == EV_REL && event.code == REL_X){
        mouse_x = event.value;
        
        pos_x += verificar_limite(limites_x, pos_x, mouse_x);
      }

      // if(event.type == EV_REL && event.code == REL_Y){
      //   mouse_y = event.value;
      //   pos_y += verificar_limite(460, pos_y, mouse_y);
      // }

      }
 
  close(fd);

  return 0;

}