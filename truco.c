#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define CARTAS 10 // Cartas Utilizadas no Jogo
#define NAIPES 4

void embaralhar();

typedef struct{
    char nome;
    bool manilha;
}carta;

carta cartas[CARTAS];
char naipes[NAIPES][8];

int baralho[CARTAS][NAIPES];

int main(){

    embaralhar();
    printf("%c %s\n", cartas[9].nome, naipes[3]);
    return 0;
}

void embaralhar(){
    for(int i = 0; i < CARTAS; i++){

        cartas[i].manilha = false;
        switch(i){
            case 0: cartas[i].nome = '4'; break;
            case 1: cartas[i].nome = '5'; break;
            case 2: cartas[i].nome = '6'; break;
            case 3: cartas[i].nome = '7'; break;
            case 4: cartas[i].nome = 'Q'; break;
            case 5: cartas[i].nome = 'J'; break;
            case 6: cartas[i].nome = 'K'; break;
            case 7: cartas[i].nome = 'A'; break;
            case 8: cartas[i].nome = '2'; break;
            case 9: cartas[i].nome = '3'; break;
        }

        for(int j = 0; j < NAIPES; j++){
            baralho[i][j] = 0;
        }
    }

    strcpy(naipes[0], "Ouros");
    strcpy(naipes[1], "Espadas");
    strcpy(naipes[2], "Copas");
    strcpy(naipes[3], "Paus");

}