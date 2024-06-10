#include "biblioteca.c"

int main(){
    long int temp =0;
    for (int i = 0; i < 4799; i++)
    {
      wbm(0b110,0b111,0b111,temp);
      temp+=1;
    }

    for (int i = 1; i <= 200; i++){
        wsm(7,0,0,i);
    }
    for (int j = 200; j <= 400; j++){
        wsm(7,0,0,j);
    }


    // for (int i = 1; i <= 20; i++){
    //   for (int j=1; j <= 20;j++){
    //     if(i==j)
    //     {
    //       wsm(7,0,0,i*j);
    //     }else
    //     {
    //       wsm(7,7,7,i*j);
    //     }
    //   }

    // }

    wbr02(380,40,0,1,14);
    
    wbr01(3,3,3);

      
    // criando a grama
  for(int j= 0; j<4800;j+=80){
    for (int i = 0; i < 10; i++)
    {
        wbm(0,7,0,j+i);
        
    }
    for(int k =69;k <80;k++){
        wbm(0,7,0,j+k);
    }
  }


    //criando faixa quadriculada

    //faixa da esquerda
      for(int j= 0; j<4800;j+=160){    
        for (int i = 10; i <12; i++)
        {
            wbm(7,0,0,j+i);
            
        }
      }
      for(int j= 80; j<4800;j+=160){    
        for (int i = 10; i <12; i++)
        {
            wbm(7,7,7,j+i);
            
        }
      }

      // //faixa da direita
       for(int j= 0; j<4800;j+=160){    
        for (int i = 67; i <69; i++)
        {
            wbm(7,0,0,j+i);
            
        }
      }
      for(int j= 80; j<4800;j+=160){    
        for (int i = 67; i <69; i++)
        {
            wbm(7,7,7,j+i);
            
        }
      }




      wbr02(320,40,24,1,11);


        // CARRO
      dp(1,7,7,7,0b0101,320,240,0);//triangulos brancos
      dp(0,0,0,7,0b0011,320,250,1);//base do meio azul
      dp(0,0,0,7,0b0101,320,300,2);//base inferior
      dp(0,0,0,0,0b0001,280,300,3);//roda traseira esquerda
      dp(0,0,0,0,0b0001,360,300,4);//roda traseira direita
      dp(0,0,0,7,0b0011,320,220,5);//topo azul
      dp(1,0,0,7,0b0010,320,190,6);//ponta azul
      dp(0,0,0,0,0b0001,350,220,7);//roda dianteira direita
      dp(0,0,0,0,0b0001,290,220,8);//roda dianteria esquerda
    //  dp(0,7,0,0,0b0011,320,300,9);//base inferior vermelho

      wbr02(460,160,17,1,5);
      wbr02(150,380,17,1,12);
      wbr02(460,360,17,1,13);
      wbr02(150,180,17,1,4);
      
     while(1){
      for(int j=1;j <=480; j++){
        wbr02(20,j + 160,16,1,2);
        wbr02(50,j + 240,4,1,3);
        wbr02(570,j + 240,4,1,6);
        wbr02(580,j + 120,4,1,7);
        wbr02(585,j + 360,4,1,8);  
        wbr02(585,j + 40,16,1,9);
        wbr02(50,j + 370,4,1,10);
        wbr02(50,j + 40 ,4,1,1);
        }
      }

      // while(1){
      //   for(int i = 480; i >1; i--){
      //     //carros
      //     wbr02(460,i+100,17,1,5);
      //     wbr02(150,i+100,17,1,12);
      //     wbr02(460,i+40,17,1,13);
      //     wbr02(150,i,17,1,4);
      //   }

      // }

    //wbm(3,3,3,81);

    // Desenhar um coração usando a função wbm
    // dp(0,0,7,0,0b0001,280,300,3);//roda esquerda

    // // Preencher espaços em branco
    // for (unsigned long int endereco = 0; endereco < 4800; endereco++) {
    //     wbm(7, 7, 7, endereco);
    // }

    // Desenhar o coração
    // for (int y = 0; y < 64; y++) {
    //     for (int x = 0; x < 64; x++) {
    //         if ((y >= 0 && y <= 5 && x >= 25 - y && x <= 39 + y) ||
    //             (y >= 6 && y <= 15 && x >= 18 && x <= 47) ||
    //             (y >= 16 && y <= 21 && x >= 14 && x <= 51) ||
    //             (y >= 22 && y <= 26 && x >= 10 && x <= 55) ||
    //             (y >= 27 && y <= 31 && x >= 6 && x <= 59) ||
    //             (y >= 32 && y <= 35 && x >= 2 && x <= 63)) {
    //             wbm(7, 0, 0, y * 64 + x);
    //         }
    //     }
    // }


    //limpar bloco de background 
    // g = 0b111
    //r= 0b110
    // b = 0b111

    return 0;
}
