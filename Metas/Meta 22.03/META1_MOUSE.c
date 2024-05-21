#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>

#define SENSI 12


void exibirMovimentoMouse(char data[]){ 
    signed char x,y;
    x = data[1]; //O data[1] (capturado no arquivo "/dev/input/mice") coorresponde as coordenadas do mouse no "eixo x"
    y = data[2]; //O data[2] coorresponde as coordenadas do mouse no "eixo y"
    if(x > SENSI){
        printf("Movendo pra direita!\n");
    }
    else if(x < -SENSI){
        printf("Movendo pra esquerda!\n");
    } else if(y > SENSI){
        printf("Movendo pra Cima!\n");
    }
    else if(y < -SENSI){
        printf("Movendo pra Baixo!\n");
    }
}

void exibirCliqueMouse(char data){
    int esquerda,direita,meio;

    //Caso tenha input do mouse, faz uma comparação através de uma porta AND
    esquerda = data & 0x1; //0x1 = 001
    direita = data & 0x2; //0x2 = 010
    meio = data & 0x4; //0x4 = 100

    if(esquerda){
        printf("Clique Esquerdo\n");
    }else if(direita){
        printf("Clique Direito!\n");
    } else if (meio){
        printf("Clique Meio\n");
    }
}

int main(){
    char data[3]; //Vetor "data", captura input do mouse - movimentos e clique
    
    signed char x,y; //Coordenadas do mouse
    
    int byte; //Recebe resposta de sucesso do da função "read"
    
    int fd = open("/dev/input/mice", O_RDONLY); //Abre arquivo onde as informações do mouse estão armazenadas 
    
    if(fd == -1){
        printf("Nao foi possivel abrir o arquivo!\n"); //Exibe erro caso não consiga abrir arquivo
    }

    while(1){
        byte = read(fd,data,sizeof(data)); //Lê o arquivo e armazena dados no vetor de char "data"
        if(byte > 0) //Caso tenha sucesso na leitura
        {
            exibirMovimentoMouse(data);
            exibirCliqueMouse(data[0]);
        }   
    }

    return 0;
}
