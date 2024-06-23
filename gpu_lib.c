/**
 * \file            gpu_lib.c
 * \brief           Biblioteca que implementa as funções do header gpu_lib.h para uso da GPU
 */

/*
 * Copyright (c) 2024 Pedro Henrique Araujo Almeida, Demerval, Matheus
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of library_name.
 *
 * Author:          Pedro Henrique ARAUJO ALMEIDA <phaalmeida1\gmail.com>
 *                  Demerval <optional_email\example.com>
 *                  Matheus <optional_email\example.com>
 */


#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include "gpu_lib.h"

int fd = 0;

/**
 * \brief           Usada para abrir o arquivo do driver da GPU
 *  \return         Retorna 1 caso o arquivo foi aberto ou retorna 0 caso não seja possivel abrir o arquivo
 */
int open_gpu_device () {
    fd = open(DEVICE_PATH, O_WRONLY);

    if (fd < 0) {
        perror("Failed to open the device");
        return 0;
    }
    return 1;
}

/**
 * \brief           Usada para fechar o arquivo do driver da GPU
 */
void close_gpu_devide () {
    close(fd);
}

/**
 * \brief           Usada para configurar a cor base do background a partir dos valores de Red, Green e Blue.
 * 
 * \param[in]       R: Valor para a cor vermelha.
 * \param[in]       G: Valor para a cor verde.
 * \param[in]       B: Valor para a cor azul.
 * \return          Retorna 0 quando a operação não foi realizada, e 1 quando foi bem sucedida
*/
int set_background_color (uint8_t R, uint8_t B, uint8_t G) {
    unsigned char command[4];

    command[0] = 0;
    command[1] = R;
    command[2] = G;
    command[3] = B;

    if (write(fd, command, sizeof(command)) < 0) {
        perror("Failed to write to the device");
        return 0;
    }

    return 1;
}

/**
 * \brief           Usada para setar um sprite na tela
 * 
 * \param[in]       reg: Registrador ao qual o sprite será armazenado
 * \param[in]       x: Coordenada x do sprite na tela
 * \param[in]       y: Coordenada y do sprite na tela
 * \param[in]       offset: Deslocamento na memória para selecionar o bitmap
 * \param[in]       sp: Ativação do sprite (0 - desativado, 1 - ativado)
 * \return          Retorna 0 quando a operação não foi realizada, e 1 quando foi bem sucedida
*/
int set_sprite(uint8_t reg, uint16_t x, uint16_t y, uint8_t offset, uint8_t sp){
    unsigned char command[7];

    command[0] = 1;
    command[1] = reg;
    command[2] = (offset >> 1) & 0xFF;
    command[3] = ((offset & 0x01) << 7) | ((x >> 3) & 0x7F);
    command[4] = ((x & 0x07) << 5) | ((y >> 5) & 0x1F);
    command[5] = (y & 0x1F) << 3;
    command[6] = sp;

    if (write(fd, command, sizeof(command)) < 0) {
        perror("Failed to write to the device");
        return 0;
    }

    return 1;
}

/**
 * \brief           Usada para setar um poligono na tela
 * 
 * \param[in]       address: Endereço onde o poligono estará armazenado
 * \param[in]       ref_x: Coordenada x na tela referente ao centro do poligono        
 * \param[in]       ref_y: Coordenada y na tela referente ao centro do poligono                      
 * \param[in]       size: Tamanho do poligono        
 * \param[in]       r: Valor para a cor vermelha 
 * \param[in]       g: Valor para a cor verde 
 * \param[in]       b: Valor para a cor azul    
 * \param[in]       shape: Formato do poligono (0 = quadrado, 1 = triangulo)
 * \return          Retorna 0 quando a operação não foi realizada, e 1 quando foi bem sucedida
*/
int set_poligono(uint16_t address, uint16_t ref_x, uint16_t ref_y, uint8_t size, uint8_t r, uint8_t g, uint8_t b, uint8_t shape){
    unsigned char command[7];

    command[0] = 4; 
    command[1] = address; 
    command[2] = ref_x >> 1; 
    command[3] = ((ref_x & 0b01) << 7) | (ref_y >> 2); 
    command[4] = ((ref_y & 0b11) << 6) | (size & 0b1111);
    command[5] = ((r & 0b111)<< 5) | (g & 0b111) << 2; 
    command[6] = ((b &0b111) << 5) | shape & 0b1;

    if (write(fd, command, sizeof(command)) < 0) {
        perror("Failed to write to the device");
        return 0;
    }

    return 1;
}

/**
 * \brief Usada para modelar o background atraves do preenchimento dos blocos de 8x8 pixels
 * 
 * \param[in]       column: Valor da coluna do bloco.
 * \param[in]       line: Valor da linha do bloco.
 * \param[in]       R: Valor para a cor vermelha.
 * \param[in]       G: Valor para a cor verde.
 * \param[in]       B: Valor para a cor azul.
 * \return          Retorna 0 quando a operação não foi realizada, e 1 quando foi bem sucedida
*/
int set_background_block(uint8_t column, uint8_t line, uint8_t R, uint8_t G, uint8_t B){
    unsigned char command[5];
    // 0001 1111 1111 1111 000
    int i = 0;
    int address = column + (line*80);


    command[0] = 2;
    command[1] = (address >> 5);
    command[2] = ((address) << 3) | R;
    command[3] = G;
    command[4] = B;

    //printf("address[1]: %d\n", (R & 0b111));


    if (write(fd, command, sizeof(command)) < 0) {
        perror("Failed to write to the device");
        return 0;
    }

    return 1;
}

/**
 * \brief           Usada para mudar o valor RGB de um determinado pixel de um sprite com base no endereço de memoria.
 * 
 * \param[in]       address: Endereço de memoria do local que deve ter o valor alterado.
 * \param[in]       R: Valor para a cor vermelha.
 * \param[in]       G: Valor para a cor verde.
 * \param[in]       B: Valor para a cor azul.
 * \return          Retorna 1 quando colisão foi detectada e 0 quando não.
*/
int set_sprite_pixel_color( uint16_t address, uint8_t R, uint8_t G, uint8_t B){
    unsigned char command[6];

    command[0] = 3; // Command for instrucao_wsm
    command[1] = (address >> 6); // Higher 8 bits of 14-bit address
    command[2] = (address & 0b111111); // Lower 6 bits of address and r
    command[3] = R & 0b111;
    command[4] = G & 0b111; // g value
    command[5] = B & 0b111; // b value

    if (write(fd, command, sizeof(command)) < 0) {
        perror("Failed to write to the device");
        return 0;
    }

    return 1;
}

/**
 * \brief           Usada para atualizar as coordenadas x e y de um sprit móvel de acordo ao seu ângulo de movimento e valor de deslocamento.
 * 
 * 
 * \param[in]       sp: Ponteiro do sprite que deve ter suas coordenadas atualizadas
 * \param[in]       mirror: Define se as coordendas do sprite enviado devem ser espelhadas
*/
void increase_coordinate(Sprite *sp, uint8_t mirror){
    if ((*sp).direction == LEFT){
        (*sp).pos_x -= (*sp).step_x;
    } else if ((*sp).direction == RIGHT){
        (*sp).pos_x += (*sp).step_x;
    } else if ((*sp).direction == UP){
        (*sp).pos_y -= (*sp).step_y;
    } else if ((*sp).direction == DOWN){
        (*sp).pos_y += (*sp).step_y;
    } else if ((*sp).direction == UPPER_RIGHT){
        (*sp).pos_y -= (*sp).step_y;
        (*sp).pos_x += (*sp).step_x;
    } else if ((*sp).direction == UPPER_LEFT){
        (*sp).pos_y -= (*sp).step_y;
        (*sp).pos_x -= (*sp).step_x;
    } else if ((*sp).direction == BOTTOM_LEFT){
        (*sp).pos_y += (*sp).step_y;
        (*sp).pos_x -= (*sp).step_x;
    } else if ((*sp).direction == BOTTOM_LEFT){
        (*sp).pos_y += (*sp).step_y;
        (*sp).pos_x += (*sp).step_x;
    } 
    int i = set_sprite((*sp).data_register, (*sp).pos_x, (*sp).pos_y, (*sp).offset, (*sp).enable);
}

/**
 * \brief           Usada para verificar se ocorreu uma colisão entre dois sprites quaisquer a partir da técnica de sobreposição de retângulos.
 * 
 * \param[in]       sp1: Ponteiro para o sprite 1.
 * \param[in]       sp2: Ponteiro para o sprite 2.
 * \return          Retorna 1 quando colisão foi detectada e 0 quando não.
*/
int collision(Sprite *sp1, Sprite *sp2){

    int square1_right = (*sp1).pos_x + 20;
    int square1_bottom = (*sp1).pos_y - 20;
    int square2_right = (*sp2).pos_x + 20;
    int square2_bottom = (*sp2).pos_y - 20;

    if ((*sp1).pos_x >= square2_right || (*sp2).pos_x >= square1_right) {
        return 0;
    }
    if ((*sp1).pos_y <= square2_bottom || (*sp2).pos_y <= square1_bottom) {
        return 0;
    }

    return 1;

}

/**
 * \brief           Usada para setar o valor "510" no RGB de todos os background blocks, assim fazendo eles copiar a cor padrão do background
 */
void clear_background_blocks() {
    int i = 0;
    int j = 0;
    for (i; i <60; i++){
        for (j; j < 80; j++){
            set_background_block(j, i, 6, 7, 7);
        }
        j = 0;
    }
}

/**
 * \brief           Usada para a partir de uma determinada linha, preencher todos os background blocks que estão na linha e todos os abaixo da linha
 * \param[in]       line: Valor da linha da tela 60x80
 */
void fill_background_blocks (uint8_t line) {
    int i = line;
    int j = 0;
    for (i; i <60; i++){
        for (j; j < 80; j++){
            set_background_block(j, i, 2, 5, 0);
        }
        j = 0;
    }
}

/**
 * \brief           Usada para colocar o valor 0 como o tamanho de todo os poligonos que estão na memoria, assim desativando ele
 */
void clear_poligonos(){
    int i = 0;
    for (i; i < 15; i++){
        set_poligono(i, 0, 0, 0, 0, 0, 0, 0);
    }
}

/**
 * \brief           Usada para desativar todos os sprite que estão nos registradores 1 até 31
 */
void clear_sprites(){
    int i = 1;
    for (i; i< 32; i++){
        set_sprite(i, 0, 0, 0, 0);
    }
}

/**
 * \brief           Usada para desenhar nos sprites de endereço 25, 26 e 27 a palavra ANF, RANS e ERAI rescpectivamente
 */
void draw_sprites_anfranserai() {
    /* A N F parte */
    set_sprite_pixel_color(10124, 7, 0, 0);
    set_sprite_pixel_color(10128, 7, 0, 0);
    set_sprite_pixel_color(10132, 7, 0, 0);
    set_sprite_pixel_color(10134, 7, 0, 0);
    set_sprite_pixel_color(10135, 7, 0, 0);
    set_sprite_pixel_color(10136, 7, 0, 0);
    set_sprite_pixel_color(10137, 7, 0, 0);
    set_sprite_pixel_color(10139, 7, 0, 0);
    set_sprite_pixel_color(10140, 7, 0, 0);
    set_sprite_pixel_color(10143, 7, 0, 0);
    set_sprite_pixel_color(10145, 7, 0, 0);
    set_sprite_pixel_color(10148, 7, 0, 0);
    set_sprite_pixel_color(10149, 7, 0, 0);
    set_sprite_pixel_color(10152, 7, 0, 0);
    set_sprite_pixel_color(10154, 7, 0, 0);
    set_sprite_pixel_color(10159, 7, 0, 0);
    set_sprite_pixel_color(10163, 7, 0, 0);
    set_sprite_pixel_color(10165, 7, 0, 0);
    set_sprite_pixel_color(10168, 7, 0, 0);
    set_sprite_pixel_color(10170, 7, 0, 0);
    set_sprite_pixel_color(10172, 7, 0, 0);
    set_sprite_pixel_color(10174, 7, 0, 0);
    set_sprite_pixel_color(10179, 7, 0, 0);
    set_sprite_pixel_color(10183, 7, 0, 0);
    set_sprite_pixel_color(10185, 7, 0, 0);
    set_sprite_pixel_color(10188, 7, 0, 0);
    set_sprite_pixel_color(10190, 7, 0, 0);
    set_sprite_pixel_color(10192, 7, 0, 0);
    set_sprite_pixel_color(10194, 7, 0, 0);
    set_sprite_pixel_color(10195, 7, 0, 0);
    set_sprite_pixel_color(10196, 7, 0, 0);
    set_sprite_pixel_color(10199, 7, 0, 0);
    set_sprite_pixel_color(10200, 7, 0, 0);
    set_sprite_pixel_color(10202, 7, 0, 0);
    set_sprite_pixel_color(10203, 7, 0, 0);
    set_sprite_pixel_color(10204, 7, 0, 0);
    set_sprite_pixel_color(10205, 7, 0, 0);
    set_sprite_pixel_color(10206, 7, 0, 0);
    set_sprite_pixel_color(10208, 7, 0, 0);
    set_sprite_pixel_color(10211, 7, 0, 0);
    set_sprite_pixel_color(10212, 7, 0, 0);
    set_sprite_pixel_color(10214, 7, 0, 0);
    set_sprite_pixel_color(10219, 7, 0, 0);
    set_sprite_pixel_color(10222, 7, 0, 0);
    set_sprite_pixel_color(10226, 7, 0, 0);
    set_sprite_pixel_color(10228, 7, 0, 0);
    set_sprite_pixel_color(10231, 7, 0, 0);
    set_sprite_pixel_color(10232, 7, 0, 0);
    set_sprite_pixel_color(10234, 7, 0, 0);
    set_sprite_pixel_color(10239, 7, 0, 0);
    set_sprite_pixel_color(10242, 7, 0, 0);
    set_sprite_pixel_color(10246, 7, 0, 0);
    set_sprite_pixel_color(10248, 7, 0, 0);
    set_sprite_pixel_color(10252, 7, 0, 0);
    set_sprite_pixel_color(10254, 7, 0, 0);
    set_sprite_pixel_color(10259, 7, 0, 0);

    /* R A N S parte */
    set_sprite_pixel_color(10521, 7, 0, 0);
    set_sprite_pixel_color(10522, 7, 0, 0);
    set_sprite_pixel_color(10523, 7, 0, 0);
    set_sprite_pixel_color(10527, 7, 0, 0);
    set_sprite_pixel_color(10531, 7, 0, 0);
    set_sprite_pixel_color(10535, 7, 0, 0);
    set_sprite_pixel_color(10537, 7, 0, 0);
    set_sprite_pixel_color(10538, 7, 0, 0);
    set_sprite_pixel_color(10539, 7, 0, 0);
    set_sprite_pixel_color(10540, 7, 0, 0);
    set_sprite_pixel_color(10543, 7, 0, 0);
    set_sprite_pixel_color(10546, 7, 0, 0);
    set_sprite_pixel_color(10548, 7, 0, 0);
    set_sprite_pixel_color(10551, 7, 0, 0);
    set_sprite_pixel_color(10552, 7, 0, 0);
    set_sprite_pixel_color(10555, 7, 0, 0);
    set_sprite_pixel_color(10557, 7, 0, 0);
    set_sprite_pixel_color(10563, 7, 0, 0);
    set_sprite_pixel_color(10566, 7, 0, 0);
    set_sprite_pixel_color(10568, 7, 0, 0);
    set_sprite_pixel_color(10571, 7, 0, 0);
    set_sprite_pixel_color(10573, 7, 0, 0);
    set_sprite_pixel_color(10575, 7, 0, 0);
    set_sprite_pixel_color(10577, 7, 0, 0);
    set_sprite_pixel_color(10581, 7, 0, 0);
    set_sprite_pixel_color(10582, 7, 0, 0);
    set_sprite_pixel_color(10583, 7, 0, 0);
    set_sprite_pixel_color(10586, 7, 0, 0);
    set_sprite_pixel_color(10588, 7, 0, 0);
    set_sprite_pixel_color(10591, 7, 0, 0);
    set_sprite_pixel_color(10593, 7, 0, 0);
    set_sprite_pixel_color(10595, 7, 0, 0);
    set_sprite_pixel_color(10597, 7, 0, 0);
    set_sprite_pixel_color(10598, 7, 0, 0);
    set_sprite_pixel_color(10599, 7, 0, 0);
    set_sprite_pixel_color(10600, 7, 0, 0);
    set_sprite_pixel_color(10601, 7, 0, 0);
    set_sprite_pixel_color(10605, 7, 0, 0);
    set_sprite_pixel_color(10606, 7, 0, 0);
    set_sprite_pixel_color(10607, 7, 0, 0);
    set_sprite_pixel_color(10608, 7, 0, 0);
    set_sprite_pixel_color(10609, 7, 0, 0);
    set_sprite_pixel_color(10611, 7, 0, 0);
    set_sprite_pixel_color(10614, 7, 0, 0);
    set_sprite_pixel_color(10615, 7, 0, 0);
    set_sprite_pixel_color(10620, 7, 0, 0);
    set_sprite_pixel_color(10622, 7, 0, 0);
    set_sprite_pixel_color(10625, 7, 0, 0);
    set_sprite_pixel_color(10629, 7, 0, 0);
    set_sprite_pixel_color(10631, 7, 0, 0);
    set_sprite_pixel_color(10634, 7, 0, 0);
    set_sprite_pixel_color(10635, 7, 0, 0);
    set_sprite_pixel_color(10640, 7, 0, 0);
    set_sprite_pixel_color(10643, 7, 0, 0);
    set_sprite_pixel_color(10645, 7, 0, 0);
    set_sprite_pixel_color(10649, 7, 0, 0);
    set_sprite_pixel_color(10651, 7, 0, 0);
    set_sprite_pixel_color(10655, 7, 0, 0);
    set_sprite_pixel_color(10657, 7, 0, 0);
    set_sprite_pixel_color(10658, 7, 0, 0);
    set_sprite_pixel_color(10659, 7, 0, 0);
    set_sprite_pixel_color(10660, 7, 0, 0);

    /* E R A I parte */
    set_sprite_pixel_color(10922, 7, 0, 0);
    set_sprite_pixel_color(10923, 7, 0, 0);
    set_sprite_pixel_color(10924, 7, 0, 0);
    set_sprite_pixel_color(10925, 7, 0, 0);
    set_sprite_pixel_color(10927, 7, 0, 0);
    set_sprite_pixel_color(10928, 7, 0, 0);
    set_sprite_pixel_color(10929, 7, 0, 0);
    set_sprite_pixel_color(10930, 7, 0, 0);
    set_sprite_pixel_color(10931, 7, 0, 0);
    set_sprite_pixel_color(10935, 7, 0, 0);
    set_sprite_pixel_color(10939, 7, 0, 0);
    set_sprite_pixel_color(10942, 7, 0, 0);
    set_sprite_pixel_color(10947, 7, 0, 0);
    set_sprite_pixel_color(10951, 7, 0, 0);
    set_sprite_pixel_color(10954, 7, 0, 0);
    set_sprite_pixel_color(10956, 7, 0, 0);
    set_sprite_pixel_color(10959, 7, 0, 0);
    set_sprite_pixel_color(10962, 7, 0, 0);
    set_sprite_pixel_color(10967, 7, 0, 0);
    set_sprite_pixel_color(10971, 7, 0, 0);
    set_sprite_pixel_color(10974, 7, 0, 0);
    set_sprite_pixel_color(10976, 7, 0, 0);
    set_sprite_pixel_color(10979, 7, 0, 0);
    set_sprite_pixel_color(10982, 7, 0, 0);
    set_sprite_pixel_color(10983, 7, 0, 0);
    set_sprite_pixel_color(10984, 7, 0, 0);
    set_sprite_pixel_color(10985, 7, 0, 0);
    set_sprite_pixel_color(10987, 7, 0, 0);
    set_sprite_pixel_color(10988, 7, 0, 0);
    set_sprite_pixel_color(10989, 7, 0, 0);
    set_sprite_pixel_color(10990, 7, 0, 0);
    set_sprite_pixel_color(10991, 7, 0, 0);
    set_sprite_pixel_color(10994, 7, 0, 0);
    set_sprite_pixel_color(10996, 7, 0, 0);
    set_sprite_pixel_color(10999, 7, 0, 0);
    set_sprite_pixel_color(11002, 7, 0, 0);
    set_sprite_pixel_color(11007, 7, 0, 0);
    set_sprite_pixel_color(11009, 7, 0, 0);
    set_sprite_pixel_color(11013, 7, 0, 0);
    set_sprite_pixel_color(11014, 7, 0, 0);
    set_sprite_pixel_color(11015, 7, 0, 0);
    set_sprite_pixel_color(11016, 7, 0, 0);
    set_sprite_pixel_color(11017, 7, 0, 0);
    set_sprite_pixel_color(11019, 7, 0, 0);
    set_sprite_pixel_color(11022, 7, 0, 0);
    set_sprite_pixel_color(11027, 7, 0, 0);
    set_sprite_pixel_color(11030, 7, 0, 0);
    set_sprite_pixel_color(11033, 7, 0, 0);
    set_sprite_pixel_color(11037, 7, 0, 0);
    set_sprite_pixel_color(11039, 7, 0, 0);
    set_sprite_pixel_color(11042, 7, 0, 0);
    set_sprite_pixel_color(11043, 7, 0, 0);
    set_sprite_pixel_color(11044, 7, 0, 0);
    set_sprite_pixel_color(11045, 7, 0, 0);
    set_sprite_pixel_color(11047, 7, 0, 0);
    set_sprite_pixel_color(11051, 7, 0, 0);
    set_sprite_pixel_color(11053, 7, 0, 0);
    set_sprite_pixel_color(11057, 7, 0, 0);
    set_sprite_pixel_color(11059, 7, 0, 0);
}

/**
 * \brief           Usada para desenhar nos sprites de endereço 28, 29 e 30 as letras P, M e D respectivamente
 */
void draw_sprites_PMD() {
    /* LETRA P EM VERDE */
    set_sprite_pixel_color(11265, 0, 7, 0);
    set_sprite_pixel_color(11266, 0, 7, 0);
    set_sprite_pixel_color(11267, 0, 7, 0);
    set_sprite_pixel_color(11268, 0, 7, 0);
    set_sprite_pixel_color(11269, 0, 7, 0);
    set_sprite_pixel_color(11270, 0, 7, 0);
    set_sprite_pixel_color(11271, 0, 7, 0);
    set_sprite_pixel_color(11272, 0, 7, 0);
    set_sprite_pixel_color(11273, 0, 7, 0);
    set_sprite_pixel_color(11274, 0, 7, 0);
    set_sprite_pixel_color(11275, 0, 7, 0);
    set_sprite_pixel_color(11276, 0, 7, 0);
    set_sprite_pixel_color(11285, 0, 7, 0);
    set_sprite_pixel_color(11286, 0, 7, 0);
    set_sprite_pixel_color(11287, 0, 7, 0);
    set_sprite_pixel_color(11288, 0, 7, 0);
    set_sprite_pixel_color(11289, 0, 7, 0);
    set_sprite_pixel_color(11290, 0, 7, 0);
    set_sprite_pixel_color(11291, 0, 7, 0);
    set_sprite_pixel_color(11292, 0, 7, 0);
    set_sprite_pixel_color(11293, 0, 7, 0);
    set_sprite_pixel_color(11294, 0, 7, 0);
    set_sprite_pixel_color(11295, 0, 7, 0);
    set_sprite_pixel_color(11296, 0, 7, 0);
    set_sprite_pixel_color(11305, 0, 7, 0);
    set_sprite_pixel_color(11306, 0, 7, 0);
    set_sprite_pixel_color(11315, 0, 7, 0);
    set_sprite_pixel_color(11316, 0, 7, 0);
    set_sprite_pixel_color(11325, 0, 7, 0);
    set_sprite_pixel_color(11326, 0, 7, 0);
    set_sprite_pixel_color(11335, 0, 7, 0);
    set_sprite_pixel_color(11336, 0, 7, 0);
    set_sprite_pixel_color(11345, 0, 7, 0);
    set_sprite_pixel_color(11346, 0, 7, 0);
    set_sprite_pixel_color(11355, 0, 7, 0);
    set_sprite_pixel_color(11356, 0, 7, 0);
    set_sprite_pixel_color(11365, 0, 7, 0);
    set_sprite_pixel_color(11366, 0, 7, 0);
    set_sprite_pixel_color(11375, 0, 7, 0);
    set_sprite_pixel_color(11376, 0, 7, 0);
    set_sprite_pixel_color(11385, 0, 7, 0);
    set_sprite_pixel_color(11386, 0, 7, 0);
    set_sprite_pixel_color(11387, 0, 7, 0);
    set_sprite_pixel_color(11388, 0, 7, 0);
    set_sprite_pixel_color(11389, 0, 7, 0);
    set_sprite_pixel_color(11390, 0, 7, 0);
    set_sprite_pixel_color(11391, 0, 7, 0);
    set_sprite_pixel_color(11392, 0, 7, 0);
    set_sprite_pixel_color(11393, 0, 7, 0);
    set_sprite_pixel_color(11394, 0, 7, 0);
    set_sprite_pixel_color(11395, 0, 7, 0);
    set_sprite_pixel_color(11396, 0, 7, 0);
    set_sprite_pixel_color(11405, 0, 7, 0);
    set_sprite_pixel_color(11406, 0, 7, 0);
    set_sprite_pixel_color(11407, 0, 7, 0);
    set_sprite_pixel_color(11408, 0, 7, 0);
    set_sprite_pixel_color(11409, 0, 7, 0);
    set_sprite_pixel_color(11410, 0, 7, 0);
    set_sprite_pixel_color(11411, 0, 7, 0);
    set_sprite_pixel_color(11412, 0, 7, 0);
    set_sprite_pixel_color(11413, 0, 7, 0);
    set_sprite_pixel_color(11414, 0, 7, 0);
    set_sprite_pixel_color(11415, 0, 7, 0);
    set_sprite_pixel_color(11416, 0, 7, 0);
    set_sprite_pixel_color(11425, 0, 7, 0);
    set_sprite_pixel_color(11426, 0, 7, 0);
    set_sprite_pixel_color(11445, 0, 7, 0);
    set_sprite_pixel_color(11446, 0, 7, 0);
    set_sprite_pixel_color(11465, 0, 7, 0);
    set_sprite_pixel_color(11466, 0, 7, 0);
    set_sprite_pixel_color(11485, 0, 7, 0);
    set_sprite_pixel_color(11486, 0, 7, 0);
    set_sprite_pixel_color(11505, 0, 7, 0);
    set_sprite_pixel_color(11506, 0, 7, 0);
    set_sprite_pixel_color(11525, 0, 7, 0);
    set_sprite_pixel_color(11526, 0, 7, 0);
    set_sprite_pixel_color(11545, 0, 7, 0);
    set_sprite_pixel_color(11546, 0, 7, 0);

    /* LETRA M EM AZUL */
    set_sprite_pixel_color(11665, 0, 0, 7);
    set_sprite_pixel_color(11676, 0, 0, 7);
    set_sprite_pixel_color(11685, 0, 0, 7);
    set_sprite_pixel_color(11686, 0, 0, 7);
    set_sprite_pixel_color(11695, 0, 0, 7);
    set_sprite_pixel_color(11696, 0, 0, 7);
    set_sprite_pixel_color(11705, 0, 0, 7);
    set_sprite_pixel_color(11706, 0, 0, 7);
    set_sprite_pixel_color(11707, 0, 0, 7);
    set_sprite_pixel_color(11714, 0, 0, 7);
    set_sprite_pixel_color(11715, 0, 0, 7);
    set_sprite_pixel_color(11716, 0, 0, 7);
    set_sprite_pixel_color(11725, 0, 0, 7);
    set_sprite_pixel_color(11726, 0, 0, 7);
    set_sprite_pixel_color(11727, 0, 0, 7);
    set_sprite_pixel_color(11728, 0, 0, 7);
    set_sprite_pixel_color(11733, 0, 0, 7);
    set_sprite_pixel_color(11734, 0, 0, 7);
    set_sprite_pixel_color(11735, 0, 0, 7);
    set_sprite_pixel_color(11736, 0, 0, 7);
    set_sprite_pixel_color(11745, 0, 0, 7);
    set_sprite_pixel_color(11746, 0, 0, 7);
    set_sprite_pixel_color(11747, 0, 0, 7);
    set_sprite_pixel_color(11748, 0, 0, 7);
    set_sprite_pixel_color(11749, 0, 0, 7);
    set_sprite_pixel_color(11752, 0, 0, 7);
    set_sprite_pixel_color(11753, 0, 0, 7);
    set_sprite_pixel_color(11754, 0, 0, 7);
    set_sprite_pixel_color(11755, 0, 0, 7);
    set_sprite_pixel_color(11756, 0, 0, 7);
    set_sprite_pixel_color(11765, 0, 0, 7);
    set_sprite_pixel_color(11766, 0, 0, 7);
    set_sprite_pixel_color(11768, 0, 0, 7);
    set_sprite_pixel_color(11769, 0, 0, 7);
    set_sprite_pixel_color(11770, 0, 0, 7);
    set_sprite_pixel_color(11771, 0, 0, 7);
    set_sprite_pixel_color(11772, 0, 0, 7);
    set_sprite_pixel_color(11773, 0, 0, 7);
    set_sprite_pixel_color(11775, 0, 0, 7);
    set_sprite_pixel_color(11776, 0, 0, 7);
    set_sprite_pixel_color(11785, 0, 0, 7);
    set_sprite_pixel_color(11786, 0, 0, 7);
    set_sprite_pixel_color(11789, 0, 0, 7);
    set_sprite_pixel_color(11790, 0, 0, 7);
    set_sprite_pixel_color(11791, 0, 0, 7);
    set_sprite_pixel_color(11792, 0, 0, 7);
    set_sprite_pixel_color(11795, 0, 0, 7);
    set_sprite_pixel_color(11796, 0, 0, 7);
    set_sprite_pixel_color(11805, 0, 0, 7);
    set_sprite_pixel_color(11806, 0, 0, 7);
    set_sprite_pixel_color(11810, 0, 0, 7);
    set_sprite_pixel_color(11811, 0, 0, 7);
    set_sprite_pixel_color(11815, 0, 0, 7);
    set_sprite_pixel_color(11816, 0, 0, 7);
    set_sprite_pixel_color(11825, 0, 0, 7);
    set_sprite_pixel_color(11826, 0, 0, 7);
    set_sprite_pixel_color(11835, 0, 0, 7);
    set_sprite_pixel_color(11836, 0, 0, 7);
    set_sprite_pixel_color(11845, 0, 0, 7);
    set_sprite_pixel_color(11846, 0, 0, 7);
    set_sprite_pixel_color(11855, 0, 0, 7);
    set_sprite_pixel_color(11856, 0, 0, 7);
    set_sprite_pixel_color(11865, 0, 0, 7);
    set_sprite_pixel_color(11866, 0, 0, 7);
    set_sprite_pixel_color(11875, 0, 0, 7);
    set_sprite_pixel_color(11876, 0, 0, 7);
    set_sprite_pixel_color(11885, 0, 0, 7);
    set_sprite_pixel_color(11886, 0, 0, 7);
    set_sprite_pixel_color(11895, 0, 0, 7);
    set_sprite_pixel_color(11896, 0, 0, 7);
    set_sprite_pixel_color(11905, 0, 0, 7);
    set_sprite_pixel_color(11906, 0, 0, 7);
    set_sprite_pixel_color(11915, 0, 0, 7);
    set_sprite_pixel_color(11916, 0, 0, 7);
    set_sprite_pixel_color(11925, 0, 0, 7);
    set_sprite_pixel_color(11926, 0, 0, 7);
    set_sprite_pixel_color(11935, 0, 0, 7);
    set_sprite_pixel_color(11936, 0, 0, 7);
    set_sprite_pixel_color(11945, 0, 0, 7);
    set_sprite_pixel_color(11946, 0, 0, 7);
    set_sprite_pixel_color(11955, 0, 0, 7);
    set_sprite_pixel_color(11956, 0, 0, 7);

    /* LETRA D EM BRANCO */
    set_sprite_pixel_color(12065, 7, 7, 7);
    set_sprite_pixel_color(12066, 7, 7, 7);
    set_sprite_pixel_color(12067, 7, 7, 7);
    set_sprite_pixel_color(12068, 7, 7, 7);
    set_sprite_pixel_color(12069, 7, 7, 7);
    set_sprite_pixel_color(12070, 7, 7, 7);
    set_sprite_pixel_color(12071, 7, 7, 7);
    set_sprite_pixel_color(12085, 7, 7, 7);
    set_sprite_pixel_color(12086, 7, 7, 7);
    set_sprite_pixel_color(12087, 7, 7, 7);
    set_sprite_pixel_color(12088, 7, 7, 7);
    set_sprite_pixel_color(12089, 7, 7, 7);
    set_sprite_pixel_color(12090, 7, 7, 7);
    set_sprite_pixel_color(12091, 7, 7, 7);
    set_sprite_pixel_color(12092, 7, 7, 7);
    set_sprite_pixel_color(12093, 7, 7, 7);
    set_sprite_pixel_color(12105, 7, 7, 7);
    set_sprite_pixel_color(12106, 7, 7, 7);
    set_sprite_pixel_color(12112, 7, 7, 7);
    set_sprite_pixel_color(12113, 7, 7, 7);
    set_sprite_pixel_color(12114, 7, 7, 7);
    set_sprite_pixel_color(12115, 7, 7, 7);
    set_sprite_pixel_color(12125, 7, 7, 7);
    set_sprite_pixel_color(12126, 7, 7, 7);
    set_sprite_pixel_color(12134, 7, 7, 7);
    set_sprite_pixel_color(12135, 7, 7, 7);
    set_sprite_pixel_color(12145, 7, 7, 7);
    set_sprite_pixel_color(12146, 7, 7, 7);
    set_sprite_pixel_color(12155, 7, 7, 7);
    set_sprite_pixel_color(12156, 7, 7, 7);
    set_sprite_pixel_color(12165, 7, 7, 7);
    set_sprite_pixel_color(12166, 7, 7, 7);
    set_sprite_pixel_color(12175, 7, 7, 7);
    set_sprite_pixel_color(12176, 7, 7, 7);
    set_sprite_pixel_color(12185, 7, 7, 7);
    set_sprite_pixel_color(12186, 7, 7, 7);
    set_sprite_pixel_color(12195, 7, 7, 7);
    set_sprite_pixel_color(12196, 7, 7, 7);
    set_sprite_pixel_color(12205, 7, 7, 7);
    set_sprite_pixel_color(12206, 7, 7, 7);
    set_sprite_pixel_color(12215, 7, 7, 7);
    set_sprite_pixel_color(12216, 7, 7, 7);
    set_sprite_pixel_color(12225, 7, 7, 7);
    set_sprite_pixel_color(12226, 7, 7, 7);
    set_sprite_pixel_color(12235, 7, 7, 7);
    set_sprite_pixel_color(12236, 7, 7, 7);
    set_sprite_pixel_color(12245, 7, 7, 7);
    set_sprite_pixel_color(12246, 7, 7, 7);
    set_sprite_pixel_color(12255, 7, 7, 7);
    set_sprite_pixel_color(12256, 7, 7, 7);
    set_sprite_pixel_color(12265, 7, 7, 7);
    set_sprite_pixel_color(12266, 7, 7, 7);
    set_sprite_pixel_color(12275, 7, 7, 7);
    set_sprite_pixel_color(12276, 7, 7, 7);
    set_sprite_pixel_color(12285, 7, 7, 7);
    set_sprite_pixel_color(12286, 7, 7, 7);
    set_sprite_pixel_color(12294, 7, 7, 7);
    set_sprite_pixel_color(12295, 7, 7, 7);
    set_sprite_pixel_color(12296, 7, 7, 7);
    set_sprite_pixel_color(12305, 7, 7, 7);
    set_sprite_pixel_color(12306, 7, 7, 7);
    set_sprite_pixel_color(12312, 7, 7, 7);
    set_sprite_pixel_color(12313, 7, 7, 7);
    set_sprite_pixel_color(12314, 7, 7, 7);
    set_sprite_pixel_color(12315, 7, 7, 7);
    set_sprite_pixel_color(12325, 7, 7, 7);
    set_sprite_pixel_color(12326, 7, 7, 7);
    set_sprite_pixel_color(12327, 7, 7, 7);
    set_sprite_pixel_color(12328, 7, 7, 7);
    set_sprite_pixel_color(12329, 7, 7, 7);
    set_sprite_pixel_color(12330, 7, 7, 7);
    set_sprite_pixel_color(12331, 7, 7, 7);
    set_sprite_pixel_color(12332, 7, 7, 7);
    set_sprite_pixel_color(12333, 7, 7, 7);
    set_sprite_pixel_color(12345, 7, 7, 7);
    set_sprite_pixel_color(12346, 7, 7, 7);
    set_sprite_pixel_color(12347, 7, 7, 7);
    set_sprite_pixel_color(12348, 7, 7, 7);
    set_sprite_pixel_color(12349, 7, 7, 7);
    set_sprite_pixel_color(12350, 7, 7, 7);
    set_sprite_pixel_color(12351, 7, 7, 7);
}