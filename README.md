# Sistemas-Digitais
Conteúdo desenvolvido durante a matéria de PBL de Sistemas Digitais.

> Grupo: 
> - Humberto Bandeira Costa
> - João Victor Alves Cerqueira
> - Júlia Carneiro Gonçalves de Souza
> - Thiago Souza Rodrigues.

# Introdução
As GPUs (Unidades de Processamento Gráfico) desempenham um papel fundamental em sistemas de computação gráfica, sendo projetadas para lidar com operações de computação gráfica e processamento de imagens de maneira eficiente. 

O ex-aluno do curso de Engenharia de Computação na UEFS (Universidade Estadual de Feira de Santana) Gabriel Sá Barreto Alves, durante suas atividades de iniciação científica e em seu trabalho de conclusão de curso, projetou e implementou uma GPU capaz de mover e controlar elementos em um monitor VGA com resolução de 640x480 pixels. Esse processador foi projetado para desenhar dois tipos de polígonos convexos (quadrado e triângulo) e manipular uma quantidade determinada de sprites. Para realizar essas tarefas, Gabriel utilizou o NIOS II como unidade de processamento principal, que foi embarcada na mesma FPGA que o Processador Gráfico.

No entanto, a plataforma DE1-SoC, utilizada nesse projeto, possui um processador ARM (HPS) que elimina a necessidade do NIOS II. Essa mudança trouxe um novo desafio: Como o HPS irá se comunicar com o processador gráfico que está na FPGA? A solução para esse problema envolve a criação de um módulo de Kernel no HPS para permitir a comunicação eficiente com o processador gráfico. 

Neste projeto, os estudantes de Engenharia de Computação da matéria Sistemas Digitais estão encarregados de desenvolver tanto o módulo de Kernel garantindo que o HPS possa gerenciar o Processador Gráfico de maneira eficaz. Além disso, será necessário desenvolver uma biblioteca em C que contenha funções para cada instrução existente na GPU.


# Metodologia

### Ferramentas utilizadas:
* Kit FPGA DE1-SoC
* Visual Studio Code para desenvolvimento do código em C
* Processador Gráfico (GPU) desenvolvido por Gabriel Sá Barreto Alves
* Monitor
* Cabo VGA para conexão



### 1\. Início do projeto

* Tabela das instruções da GPU.
* Código em C para escrever uma instrução.


### 2\. Análise do projeto a nível de hardware:

* O que é o Kernel?
* Como fazer um módulo do Kernel?
* Como fazer a comunicação do espaço do usuário com o Kernel?
* Como ocorre a passagem de dados do Kernel para a GPU?


### 3\. Desenvolvimento do código e integração com placa

* Desenvolvimento do módulo Kernel
* Criação da biblioteca de funções com cada Instrução do Processador Gráfico
* Desenvolvimento do código responsável pela geração do Driver do Processador Gráfico


### 4\. Ajustes

* Testes e correções no funcionamento do código


# Descrição do Projeto

## Processador Gráfico
- imagem 

## Módulo Kernel 
Módulos Kernel surgem da necessidade de estender a funcionalidade do kernel do sistema operacional para suportar novos dispositivos ou fornecer novas funcionalidades. 
Assim, no contexto retratado, para realizar a comunicação entre o processador ARM (HPS) e a GPU embarcada na mesma FPGA é necessário o desenvolvimento desse módulo para o Linux. Além disso, para que essa comunicação seja realizada de maneira eficaz e segura entre o hardware e o sistema operacional, surge a necessidade de um driver de dispositivo, também essencial para a criação da biblioteca solicitada, abstraindo a complexidade na comunicação e apresentando uma interface de leitura/escrita padrão do Linux utilizada em qualquer comunicação hardware-software.

Utilizando como um documento de endereçamento da GPU fornecido pelo professor, foi desenvolvido um código em C responsável por ambas as funções, gerar o módulo Kernel e o Driver.

## Biblioteca 

Para que usuários consigam utilizar o código de maneira simples, uma biblioteca foi criada - ela é responsável por enviar uma cadeia de bits referente as instruções que o usuário utilizar para o kernel. Sendo assim, um arquivo nomeado "GPU" foi criado para que a biblioteca escreva as informações 

# Como utilizar?
1\. Faça download dos arquivos no Kit FPGA DE1-SoC que já tenha a GPU previamente configurada. 

2\. Abra um terminal remoto na pasta que você colocou os arquivos. 

3\. Digite o comando 'make'
* Atenção! É possível que na primeira vez que você rode o código apareça esse aviso:
  - imagem do erro de clock

# Resultados e Conclusão:

--

# Possíveis melhorias:

--

# Referências:

--

