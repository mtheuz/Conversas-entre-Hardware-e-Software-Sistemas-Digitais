
<div align="center">
<h1>Conversas entre Hardware e Software</h1>
</div>


  ### Passo a passo
 ```py 
1. Clone esse repositório para sua máquina [git clone URL]
2. Faça conexão com a placa [ssh aluno@IP]
3. Faça uma cópia do arquivo para a placa por meio do scp:
    scp [ opcoes ] < usuario_fonte@endereco_IP >:/ < diretorio_do_arquivo_fonte/ arquivo_fonte > < usuario_destino@endereco_IP >:
    /<diretorio_do_arquivo_destino / arquivo_destino >
5. Navegue até o arquivo
6. Execute make 
7. carregue o múdulo: sudo insmod gpu_driver.ko
6.  execute: gcc -o exec main.c gpu_lib.c

 ```

# 📖 Introdução

### Introdução

Este documento apresenta a solução para o Problema #2 da discplina de Sistemas Digitais da Universidade Estadual de Feira de Santana, centrado na interação entre hardware e software. O objetivo principal é desenvolver módulos de Kernel no HPS (Hard Processor System) da plataforma DE1-SoC para um Processador Gráfico implementado previamente em uma FPGA. Esse desafio abrange a compreensão e implementação da comunicação entre hardware e software, mapeamento de memória, programação em C e gerenciamento de sistemas operacionais Linux na arquitetura ARM.

O problema se baseia no trabalho de conclusão de curso do discente Gabriel Sá Barreto Alves, que projetou um Processador Gráfico capaz de manipular elementos visuais em um monitor VGA com resolução de 640x480 pixels. A plataforma DE1-SoC, equipada com um processador ARM, deve comunicar-se de forma eficiente com o Processador Gráfico integrado na FPGA. Este documento detalha o desenvolvimento de uma biblioteca em C e dos módulos de Kernel necessários para esta comunicação, assegurando a plena funcionalidade do Processador Gráfico.

# 🎯 Requisitos do sistema
- [x] O código deve ser escrito em linguagem C;
- [x] A biblioteca deve conter no mínimo uma função para cada Instrução do Processador Gráfico;
- [x] A biblioteca deve seguir as recomendações descritas em: https://github.com/MaJerle/c-code-style;

# 📝 Descrição das ferramentas utilizadas 

- ### Nano:
    O GNU Nano é um editor de texto minimalista, integrado de forma nativa em sistemas operacionais Linux e operado diretamente no terminal do sistema. Este software foi um dos utilitários de edição de texto empregados durante a codificação do projeto.
    
- ### Linguagem C:
    Foi utilizado linguagem C para codificar o projeto devido à sua capacidade de ser executada em computadores com recursos limitados, que não suportam softwares mais robustos. Além disso, sua proximidade com a arquitetura de máquina e a disponibilidade de bibliotecas-padrão facilitam a interação com o sistema operacional, tornando mais eficiente a manipulação dos drivers dos dispositivos utilizados no sistema. 
    
- ### GNU
    O GNU Compiler Collection (GCC) é uma ferramenta de compilação de código aberto amplamente utilizada, oferecida gratuitamente para diversas linguagens de programação. Foi instalado e empregado no sistema operacional da plataforma De1-SoC para compilar os códigos escritos em C pelo grupo de desenvolvimento.

- ### Makefile 

    Um arquivo Makefile é um script especial usado em ambientes de desenvolvimento de software para automatizar o processo de compilação, vinculação e execução de programas. Ele define as regras e dependências entre os arquivos do seu projeto, facilitando a construção e a manutenção do software.

- ### SSH

    O SSH (Secure Shell) é um protocolo de rede usado para comunicação segura e controle remoto de dispositivos através de uma conexão criptografada. Ele é amplamente utilizado para acessar servidores e dispositivos remotos pela internet, garantindo a proteção das informações transmitidas por meio de criptografia forte.




#  Arquitetura do processador gráfico
A arquitetura apresentada na imagem abaixo mostra um sistema de processamento gráfico integrado, composto por várias unidades funcionais conectadas.

![image](https://github.com/mtheuz/Problema-2---Sistemas-Digitais/assets/77650601/313ebf3a-a4ba-4506-a048-b9c2a85a5fd0)

 O Processador de Propósito Geral (GPP) executa instruções gerais. O GPP possui uma Unidade de Memória que inclui Memória de Instruções para armazenar as instruções a serem executadas e Memória de Dados para os dados necessários à execução das instruções. As interfaces do GPP incluem periféricos para comunicação com outras partes do sistema e um barramento (bus) para facilitar a comunicação de dados. Além disso, o GPP utiliza um contador de pulsos e um gerador de pulsos para sincronização e controle de tempo.

A Unidade de Processamento Gráfico (GPU) inclui um buffer de instruções com duas filas FIFO (A e B) para armazenar temporariamente as instruções gráficas antes de serem processadas. O campo de opcode e endereçamento no buffer de instruções armazena os códigos de operação e endereços das instruções. O núcleo da GPU (GPU core) processa as instruções gráficas, acessa as memórias de dados (memória de sprites e memória de background) e gera a saída de vídeo.

A memória de sprites armazena bitmaps de sprites, enquanto a memória de background armazena bitmaps do fundo da tela. A memória de sprites possui 12.800 palavras de 9 bits (3 bits por componente RGB), permitindo o armazenamento de 32 sprites de 20x20 pixels cada, totalizando 400 posições de memória por sprite. A memória de background possui 4.800 palavras de 9 bits para modificar pequenas partes do fundo da tela.

O fluxo de dados começa com o carregamento do código pelo usuário na memória de instruções do GPP. O GPP processa o código, gerencia a comunicação entre as diferentes unidades e gera instruções gráficas que são passadas para o buffer de instruções da GPU. A GPU processa essas instruções, acessa as memórias de dados e gera a saída de vídeo. Finalmente, a saída de vídeo é enviada ao monitor VGA para exibição.

Essa arquitetura permite que um usuário interaja com jogos e aplicativos gráficos, proporcionando uma plataforma eficiente para a execução e exibição de gráficos complexos em tempo real. O sistema é capaz de administrar 31 sprites em um mesmo frame, renderizando imagens em uma taxa de aproximadamente 60 frames por segundo, o que resulta em uma tela impressa a cada 16,768 ms. Além disso, um co-processador gerencia a construção de polígonos convexos, como quadrados e triângulos, que são renderizados na tela junto com os sprites e o background.

# Instruções do processador gráfico

O processador gráfico possui quatro instruções, cada instrução tem o tamanho de 64 bits:

- Escrita no Banco de Registradores (WBR):
Para esta instrução, o valor é configurado em 0000. Responsável por definir os registradores que armazenam as informações dos sprites e a cor de fundo.
- Escrita na Memória de Sprites (WSM): 
Para esta instrução, o valor é configurado em 0001.Essa instrução armazena ou modifica o conteúdo presente na Memória de Sprites
- Escrita na Memória de Background (WBM): 
Para esta instrução, o valor é configurado em 0010. Essa instrução armazena ou modifica o conteúdo presente na Memória de Background.
- Definição de um Polígono (DP): 
Para esta instrução, o valor é configurado em 0011.Essa instrução é utilizadapara modificar o conteúdo da Memória de instrução do Coprocessador

# Fluxo de Comunicação
A imagem abaixo ilustra a comunicação entre uma aplicação de usuário (main.c), a biblioteca gpu_lib, o módulo de kernel gpu_driver e o hardware.

![image](https://github.com/mtheuz/Problema-2---Sistemas-Digitais/assets/77650601/b2eda83b-a94e-4301-aae4-df3d4be7ddc1)

## Módulo de kernel

## Funções

O código contém a implementação inicial das funções básicas das seguintes bibliotecas:

- set_background_color (Instrução WBR)
- set_sprite (Instrução WBR)
- set_poligono (Instrução DP)
- set_background_block (Instrução WBM)
- set_sprite_pixel (Instrução WSM)

Essas funções são imprecidivéis para a criação de formas e desenhos na tela.

# Criação de Polígonos
set_poligono(address, ref_x, ref_y, size, r, g, b, shape)
![TB 1 PLOLIGONOS](https://github.com/mtheuz/Problema-2---Sistemas-Digitais/assets/8845392/e6433bca-5a9f-47b6-a898-eee4f0c55b0e)

<img src="[imagefile.jpg](https://github.com/mtheuz/Problema-2---Sistemas-Digitais/assets/8845392/e6433bca-5a9f-47b6-a898-eee4f0c55b0e)" alt="Image" height="20" width="20">


# Referências
- 

- 

# Autores

- Matheus Mota Santos 
- Pedro Henrique Araujo
- Dermeval Neves de Oliveira Filho
