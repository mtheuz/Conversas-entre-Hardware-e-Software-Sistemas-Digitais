
#define LEFT 0
#define RIGHT 4
#define UP 2
#define DOWN 6
#define UPPER_RIGHT 1
#define UPPER_LEFT 3
#define BOTTOM_LEFT 5
#define BOTTOM_RIGHT 7

#define DEVICE_PATH "/dev/gpu_driver"

/**
 * @brief Struct usada para sprits moveis.
 */
typedef struct{
int pos_X; /*Armazena a coordenada X do sprite. */
int pos_Y; /*Armazena a coordenada Y do sprite. */
int direction; /*Armazena um numero inteiro indicando o ́angulo de movimento. */
int offset; /*Indica o offset de memoria ́utilizado para a escolha do bitmap armazenado no Processa-dor Grafico. */
int data_register; /*Indica o registrador do Banco de Registradores no qual as informaçoes do ̃sprite serao armazenadas. */
int step_X; /*Armazena o numero de pixels que o sprite ira se deslocar no eixo X quando o comando de movimentação for utilizado. */
int step_Y; /*Armazena o numero de pixels que o sprite ira se deslocar no eixo Y quando o comando de movimentação for utilizado. */
int enable; /*Habilita/Desabilita a impressao do ̃sprite em um determinado momento. */
int collision; /*Informa se o sprite sofreu alguma colisao. */
} Sprite; 

/**
 * @brief Struct usada para para os sprites fixos, ou seja, nao possuem movimentação durante o jogo.
 */
typedef struct{
int coord_x, coord_y, offset;
int data_register, ativo;
} Sprite_Fixed;

/**
 * @brief Usada para setar um sprite na tela
 * 
 * @param reg      
 * @param x                Coordenada x do sprite na tela
 * @param y                Coordenada y do sprite na tela
 * @param offset           Deslocamento na memória para selecionar o bitmap
 * @param sp   Ativação do sprite (0 - desativado, 1 - ativado)
 * @return                 0 quando a operação não foi realizada, e 1 quando foi bem sucedida
*/
int set_sprite( int reg, int x, int y, int offset, int sp);

/**
 * @brief Usada para setar um sprite na tela
 * 
 * @param address      
 * @param ref_x                
 * @param ref_y                
 * @param size           
 * @param r   
 * @param g   
 * @param b   
 * @param shape   
 * @return                 0 quando a operação não foi realizada, e 1 quando foi bem sucedida
*/
int set_poligono( int address, int ref_x, int ref_y, int size, int r, int g, int b, int shape);

/**
 * @brief Usada para modelar o background atraves do preenchimento dos blocos de 8x8 pixels
 * 
 * @param column    Valor da coluna do bloco.
 * @param line      Valor da linha do bloco.
 * @param R         Valor para a cor vermelha.
 * @param G         Valor para a cor verde.
 * @param B         Valor para a cor azul.
 * @return          0 quando a operação não foi realizada, e 1 quando foi bem sucedida
*/
int set_background_block( int column, int line, int R, int G, int B);

/**
 * @brief Usada para configurar a cor base do background a partir dos valores de Red, Green e Blue.
 * 
 * @param R   Valor para a cor vermelha.
 * @param G   Valor para a cor verde.
 * @param B   Valor para a cor azul.
 * @return    0 quando a operação não foi realizada, e 1 quando foi bem sucedida
*/
int set_background_color(int R, int G, int B);

/**
 * @brief Usada para atualizar as coordenadas x e y de um sprit móvel de acordo ao seu ângulo de movimento e valor de deslocamento.
 * 
 * 
 * @param sp       Ponteiro do sprite que deve ter suas coordenadas atualizadas
 * @param mirror   Define se as coordendas do sprite enviado devem ser espelhadas
*/
void increase_coordinate(Sprite *sp, int mirror);

/**
 * @brief Usada para verificar se ocorreu uma colisão entre dois sprites quaisquer a partir da técnica de sobreposição de retângulos.
 * 
 * @param sp1   Ponteiro para o sprite 1.
 * @param sp2   Ponteiro para o sprite 2.
 * @return      1 quando colisão foi detectada e 0 quando não.
*/
int collision(Sprite *sp1, Sprite *sp2);

int set_sprite_pixel_color( int address, int R, int G, int B);

int open_gpu_device ();

int close__gpu_devide ();

void clear_screen();

void clear_poligonos();

void teste () ;

void fill_background_blocks (int line);

void clear_sprites();
