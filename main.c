#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <intelfpgaup/SW.h>
#include <pthread.h> 

#define SENSI 3

int global_pause = 0;

void LerMatriz(int posicao_mouse[][3], int coordenadas[]){
    //Percorrendo matriz e salva a posicao atual
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(posicao_mouse[i][j] == 1){
                coordenadas[0] = i;
                coordenadas[1] = j;
            }
        }         
    }
}

//Adiciona ao vetor os movimentos feito pelo jogador - cada posição do vetor é um movimento no jogo (direita, esquerda, cima, baixo)
int PegarMovimento(char data[3], int vetor[4], int control) {
    signed char x, y;
    int direita = 0, esquerda = 0, cima = 0, baixo = 0;
    
    x = data[1];
    y = data[2];

    if (x > SENSI && control == 0 && direita < 2) {
        direita += 1;
        control +=1;
    }
    else if (x < -SENSI && control == 0 && esquerda < 2) {
        esquerda += 1;
        control +=1;
    } 
    else if (y > SENSI && control == 0 && cima < 2) {
        cima += 1;
        control +=1;
    }
    else if (y < -SENSI && control == 0  && baixo < 2) {
        baixo += 1;
        control +=1;
    }

    vetor[0] = direita;
    vetor[1] = esquerda;
    vetor[2] = cima;
    vetor[3] = baixo;

    return control;
}

 int exibirCliqueMouse(char data){
    if (data & 0x02){
        return 2;
    }else if (data & 0x01){
        return 1;
    }else if (data & 0x04){
        return 3;
    }

    return 0;
 }

int FazerMovimento(char data[3], int movimento[4], int posicao_mouse[3][3], int control){
    char x = data[1];
    char y = data[2];
    int coordenadas[2];
    int direita = movimento[0];
    int esquerda = movimento[1];
    int cima = movimento[2];
    int baixo = movimento[3];
    

    if(x == 0 || y == 0){
        control = 0; 
        LerMatriz(posicao_mouse, coordenadas);// encontra a posicao atual do mouse
        int linha = coordenadas[0]; //Salva as posicoes
        int coluna = coordenadas[1];
        posicao_mouse[linha][coluna] = 0;     

        // muda a posicao caso seja possivel
        if(direita > 0){
            if(coluna < 2){
                coluna +=1;            
            } 
        }else if(esquerda > 0){
            if(coluna > 0){
                coluna -=1;
            }        
        }else if(cima > 0){
            if(linha > 0){
                linha -=1;
            }            
       }else if(baixo > 0){
            if(linha < 2){
                linha += 1;
            }
        }
        posicao_mouse[linha][coluna] = 1;
    }
    return control;
}


int EfetuarJogada(char jogo[3][3], int posicao_mouse[3][3], int jogador_atual){
    char caracter;
    int coordenadas[2];
      
    LerMatriz(posicao_mouse, coordenadas);

    int linha = coordenadas[0];
    int coluna = coordenadas[1];

    //Verifica jogador da vez
    if (jogo[linha][coluna] == ' ')
    {
        if (jogador_atual == 1){
            caracter = 'X';
            jogador_atual = 0;
        }else{
            jogador_atual = 1;
            caracter = 'O';
        }
    }
    
    if(jogo[linha][coluna] == ' '){
        jogo[linha][coluna] = caracter; //faz jogada
    }
             
    return jogador_atual;
}

 

void ImprimirJogo(char jogo[3][3], int posicao_mouse[3][3], int jogador_atual){
   
    int coordenadas[2];
    LerMatriz(posicao_mouse, coordenadas);
    int linha = coordenadas[0];
    int coluna = coordenadas[1];
    printf("\e[1;1H\e[2J");
    
    if (jogador_atual == 1){
        printf("    Jogador X\n");
    }else{
        printf("    Jogador O\n");
    }  
    
    printf("\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n",jogo[0][0],jogo[0][1],jogo[0][2]);
    printf("     |     |     \n");
    printf("-----------------\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n",jogo[1][0],jogo[1][1],jogo[1][2]);
    printf("     |     |     \n");
    printf("-----------------\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n",jogo[2][0],jogo[2][1],jogo[2][2]);
    printf("     |     |     \n");
    printf("\n");
    printf("Linha: %d  Coluna: %d\n", linha, coluna);
}

int VerificarVencedor(char jogo[3][3]) {
    
    //Verfica linhas e colunas
    for (int i = 0; i < 3; i++) {
        //Linhas
        if (jogo[i][0] != ' ' && jogo[i][0] == jogo[i][1] && jogo[i][1] == jogo[i][2]) {
            if (jogo[i][0] == 'X'){
                return 'X'; // X venceu
            }else{
                return 'O'; // O venceu
            }
        }
        
        //Colunas
        if (jogo[0][i] != ' ' && jogo[0][i] == jogo[1][i] && jogo[1][i] == jogo[2][i]) {
            if (jogo[0][i] == 'X'){
                return 'X'; 
            }else{
                return 'O'; 
            }
        }
    }

    //Diagonal principal
    if (jogo[0][0] != ' ' && jogo[0][0] == jogo[1][1] && jogo[1][1] == jogo[2][2]) {
        if (jogo[0][0] == 'X')
            return 'X'; 
        else
            return 'O'; 
    }

    //Diagonal Secundária
    if (jogo[0][2] != ' ' && jogo[0][2] == jogo[1][1] && jogo[1][1] == jogo[2][0]) {
        if (jogo[0][2] == 'X'){
            return 'X'; 
        }else{
            return 'O'; 
        }
    }

     // Verificar empate
    int empate = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (jogo[i][j] == ' ') {
                empate = 0;
                break;
            }
        }
    }
    if (empate == 1)
        return 'E'; // Empate

    return ' '; // Jogo ainda em andamento

}

void *pausarJogo(){
    SW_open();
    while(1){
        int sw;
        SW_read(&sw);
        if(sw == 1) 
        {  
            global_pause = 1;
            printf("  Jogo Pausado!\n");
 
            while(global_pause){
                SW_read(&sw);
                if(sw == 0){ 
                    printf("\x1b[1F"); // Move to beginning of previous line
                    printf("\x1b[2K"); // Clear entire line
                    printf("Em jogo\n");
                    printf("\x1b[1F"); // Move to beginning of previous line
            
                    global_pause = 0;
                }
            }

        }
        
    }
    SW_close();
    return NULL;
}

void telaInicial(){
    system("clear");
    printf("   _                             _                         _  _   \n");
    printf("  (_)                           | |                       | || |  \n");
    printf("   _   ___    ____   ___      _ | |  ____    _   _   ____ | || | _    ____\n");
    printf("  | | / _ \\  / _  | / _ \\    / || | / _  |  | | | | / _  )| || || \\  / _  |\n");
    printf("  | || |_| |( ( | || |_| |  ( (_| |( ( | |   \\ V / ( (/ / | || | | |( ( | |\n");
    printf(" _| | \\___/  \\_|| | \\___/    \\____| \\_||_|    \\_/   \\____)|_||_| |_| \\_||_|\n");
    printf("(__/        (_____|                                                       \n");
    printf("                      Mexa o mouse para iniciar o jogo                               \n");
    sleep(2);
}


int main(){
    pthread_t thread_pause;
    char data[3]; //data para pegar clicks do mouse
    int movimento[4];

    int posicao_mouse[3][3] = {{0,0,0},{0,1,0},{0,0,0}}; //posicao atual do jogador
    char jogo[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}}; //Jogo atual 
    signed char x,y;
    char vencedor = ' ';
    int clique;
  
    
    int jogador_atual = 0; //inicia com o jogador X

    int control = 0; //Variavel global
    int byte;
    int fd = open("/dev/input/mice", O_RDONLY);
    if(fd == -1){
        printf("Nao foi possivel abrir o arquivo!\n");
    }

    if(pthread_create(&thread_pause, NULL, pausarJogo, NULL)){
        printf("Erro ao criar thread.");
    } 
    
    telaInicial();

    while(1){
    
        byte = read(fd,data,sizeof(data)); 
       
        if(global_pause == 0){
            control = PegarMovimento(data, movimento, control); //percorrer esse vetor para ver qual foi a jogada do jogador atual
            control = FazerMovimento(data, movimento, posicao_mouse, control);
            clique = exibirCliqueMouse(data[0]);
        
            if(clique == 1) 
            {   
                jogador_atual = EfetuarJogada(jogo, posicao_mouse, jogador_atual);
                clique = 0;     
            }

            
            //VERIFICAR VENCEDOR
            vencedor = VerificarVencedor(jogo); 
            
            ImprimirJogo(jogo, posicao_mouse, jogador_atual);  
            
            if (vencedor == 'X' || vencedor == 'O'){
                printf("\n\n\nJogador %c venceu!\n\n\n", vencedor);
            }
            else if (vencedor == 'E'){
                printf("\n\n\nEmpatou :p!\n\n\n");
            }

            if(vencedor != ' '){
                exit(0);
            }
        }
    }

    pthread_join(thread_pause,NULL);
    return 0;
}
