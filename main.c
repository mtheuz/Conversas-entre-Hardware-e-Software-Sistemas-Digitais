
/**
 * \file            main.c
 * \brief           Programa que quando compilado será executado para formar um desenho no monitor usando a GPU
 */

/*
 * Copyright (c) 2024 Pedro Henrique Araujo Almeida, Dermeval Neves de Oliveira Filho, Matheus
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
 *                  Dermeval Neves de Oliveira Filho <dermevalneves\gmail.com>
 *                  Matheus Mota Santos<matheuzwork\gmail.com>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "gpu_lib.h"

int main()
{   
    /* Tentar abrir o arquivo do kernel do driver da GPU */
    if (open_gpu_device() == 0)
        return 0;

    set_background_color(0, 0, 0); /* Coloca a cor do background como preto */
    draw_sprites_anfranserai(); /* Desenha na memoria de sprites a palavra anfranerai */
    draw_sprites_PMD(); /* Desenha na memoria de sprites as letras P, M e D*/

    /* DESENHA TODOS OS POLIGONOS NA TELA QUANDO O PROGRAMA RECEBE A LETRA 'N' PELO TERMINAL */
    printf("Pressione 'N' para começar o desenho: ");

    char ch;
    while (1) {
        ch = getchar();

        if (ch == 'N') {
            break;
        }

        while (getchar() != '\n');
        
        printf("Pressione 'N' para começar o desenho: ");
    }

    usleep(300 * 1000); /* PEQUENO DELAY */ 
    set_poligono(6, 400, 300, 9, 0, 0, 5, 0); /* PAREDE DA CASA */
    usleep(300 * 1000); 
    set_poligono(5, 400, 200, 10, 5, 0, 0, 1); /* TELHADO DA CASA */
    usleep(300 * 1000); 
    set_poligono(2, 380, 310, 1, 4, 2, 0, 0); /* JANELA DA CASA */
    usleep(300 * 1000); 
    set_poligono(1, 420, 335, 2, 4, 2, 0, 0); /* PRIMEIRA PARTE DA PORTA DA CASA */
    usleep(300 * 1000); 
    set_poligono(0, 420, 305, 2, 4, 2, 0, 0); /* SEGUNDA PARTE DA PORTA DA CASA */
    usleep(300 * 1000); 
    set_poligono(14, 500, 100, 4, 7, 7, 7, 0); /* LUA */


    /* DESENHA O CHÃO DO DESENHO NA TELA QUANDO O PROGRAMA RECEBE A LETRA 'N' PELO TERMINAL */
    printf("Pressione 'N' para desenhar o chão: ");

    while (1) {
        ch = getchar();

        if (ch == 'N') {
            break;
        }

        while (getchar() != '\n');
        
        printf("Pressione 'N' para desenhar o chão: ");
    }
    
    fill_background_blocks(35); /* Chamada da função que preenche todo os background blocks da linha 35 para baixo*/

    /* DESENHA AS ESTRELAS NO CEU NA TELA QUANDO O PROGRAMA RECEBE A LETRA 'N' PELO TERMINAL*/
    printf("Pressione 'N' para desenhar as estrelas: ");

    while (1) {
        ch = getchar();

        if (ch == 'N') {
            break;
        }

        while (getchar() != '\n');
        
        printf("Pressione 'N' para desenhar as estrelas: ");
    }

    set_background_block(10, 10, 7, 5, 0); /* Primeira estrela */
    usleep(250 * 1000); /* Pequeno Delay */
    set_background_block(25, 13, 7, 5, 0); /* Segunda estrela */
    usleep(250 * 1000);
    set_background_block(34, 11, 7, 5, 0); /* Terceira estrela */
    usleep(250 * 1000);
    set_background_block(15, 15, 7, 5, 0); /* Quarta estrela */
    usleep(250 * 1000);
    set_background_block(46, 12, 7, 5, 0); /* Quinta estrela */
    usleep(250 * 1000);
    set_background_block(40, 10, 7, 5, 0); /* Sexta estrela */
    usleep(250 * 1000);
    set_background_block(33, 16, 7, 5, 0); /* Setima estrela */
    usleep(250 * 1000);
    set_background_block(70, 13, 7, 5, 0); /* Oitava estrela */
    usleep(250 * 1000);
    set_background_block(8, 15, 7, 5, 0); /* Nona estrela */
    usleep(250 * 1000);
    set_background_block(75, 9, 7, 5, 0); /* Decima estrela */ 
    usleep(250 * 1000);
    set_background_block(78, 15, 7, 5, 0); /* Decima primeira estrela */

    /* DESENHA OS SPRITES NA TELA QUANDO O PROGRAMA RECEBE A LETRA 'N' PELO TERMINAL */
    printf("Pressione 'N' para desenhar os sprites: ");

    while (1) {
        ch = getchar();

        if (ch == 'N') {
            break;
        }

        while (getchar() != '\n');
        
        printf("Pressione 'N' para desenhar os sprites: ");
    }

    set_sprite(1, 0, 50,6 , 1); /* NAVE */
    set_sprite(4, 620, 100, 8, 1); /* NAVE */
    set_sprite(2, 200, 330, 4 , 1); /* FLOR */
    set_sprite(3, 250, 330, 4 , 1); /* FLOR */

    /* ANIMAÇÃO DAS NAVES NA TELA QUANDO O PROGRAMA RECEBE A LETRA 'N' PELO TERMINAL */
    printf("Pressione 'N' para começar a animação das naves: ");

    while (1) {
        ch = getchar();

        if (ch == 'N') {
            break;
        }

        while (getchar() != '\n');
        
        printf("Pressione 'N' para começar a animação das naves: ");
    }

    /* Declaração das coordenadas de referencia do X que os sprites vão se mover*/
    uint8_t x = 0;
    uint8_t x2 = 620;

    /* Loop da animação */
    while (1){
        x += 10;
        x2 -= 10;

        set_sprite(1, x, 50, 6, 1); /* Nave superior */
        set_sprite(10, x - 60, 50, 28, 1); /* SPRITE COM A LETRA P */
        set_sprite(11, x - 40, 50, 29, 1); /* SPRITE COM A LETRA M */
        set_sprite(12, x - 20, 50, 30, 1); /* SPRITE COM A LETRA D */
        
        set_sprite(4, x2, 100, 8, 1); /* Nave inferior */
        set_sprite(25, x2 + 20, 100, 25, 1); /* SPRITE COM A PRIMEIRA PARTE DE 'ANFRANSERAI' */
        set_sprite(26, x2 + 40, 100, 26, 1); /* SPRITE COM A SEGUNDA PARTE DE 'ANFRANSERAI' */
        set_sprite(27, x2 + 60, 100, 27, 1); /* SPRITE COM A TERCEIRA PARTE DE 'ANFRANSERAI */

    /* Verifica se os sprites chegaram no fim da tela e reseta suas posições*/
    if (x == 620){
	    x = 0;
        x2 = 620;
       }
       usleep(300 * 1000); 
    }

    close_gpu_devide(); /* Fecha o arquivo do driver da GPU */

    return 0;
}
