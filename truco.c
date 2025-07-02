#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define CARTAS 40 // Total de cartas utilizadas no jogo
#define JOGADORES 4
#define MAO 3 // Quantas cartas cada jogador vai ter

void embaralhar();
void ler_players();
int randomico(int n);
void mostrar_baralho();
int vira();
void dar_cartas();

typedef struct{
    char nome;
    char naipe[10];
    int status; // 0 = disponível, 1 = jogada, -1 = já foi pega
    int valor;
}carta;

typedef struct{
    char nome[50];
    bool time;
    int carta_payer[MAO];
}payer;

carta baralho[CARTAS];
payer payers[JOGADORES];
char times[2][50];


int main(){

    embaralhar();
    ler_players();
    vira();
    dar_cartas();
    mostrar_baralho();
    
    return 0;
}


void embaralhar(){
    for(int i = 0; i < CARTAS; i++){

        baralho[i].status = 0;
        baralho[i].valor = i / 4;

        switch(i / 4){
            case 0: baralho[i].nome = '4'; break;
            case 1: baralho[i].nome = '5'; break;
            case 2: baralho[i].nome = '6'; break;
            case 3: baralho[i].nome = '7'; break;
            case 4: baralho[i].nome = 'Q'; break;
            case 5: baralho[i].nome = 'J'; break;
            case 6: baralho[i].nome = 'K'; break;
            case 7: baralho[i].nome = 'A'; break;
            case 8: baralho[i].nome = '2'; break;
            case 9: baralho[i].nome = '3'; break;
        }

        switch(i % 4){
            case 0: strcpy(baralho[i].naipe, "Ouros"); break;
            case 1: strcpy(baralho[i].naipe, "Espadas"); break;
            case 2: strcpy(baralho[i].naipe, "Copas"); break;
            case 3: strcpy(baralho[i].naipe, "Paus"); break;
        }

    }

}

int randomico(int n){

    clock_t tempo;
    tempo = clock();
    srand((int)tempo);

    return rand() % n;
}

void mostrar_baralho(){

    for(int i = 0; i < CARTAS; i++){
        printf("Nome: %c, Naipe: %s, Valor: %d, Status %d\n", baralho[i].nome, baralho[i].naipe, baralho[i].valor, baralho[i].status);
    }
    printf("\n");

    for(int i = 0; i < JOGADORES; i++){
        printf("Nome: %s, Time: %s \nCartas: | ", payers[i].nome, times[(int)payers[i].time]);
        for(int j = 0; j < MAO; j++){
            printf("%c %s | ", baralho[payers[i].carta_payer[j]].nome, baralho[payers[i].carta_payer[j]].naipe);
        }
        printf("\n");
    }
}

int vira(){
    int k = randomico(40);
    int j = ((k/4)*4) + 4;

    if(j >= 40){
        j = 0;
    }

    for(int i = 0; i < 4; i++, j++){
        baralho[j].valor = 10 + i;
    }

    baralho[k].status = 1;

    printf("A vira é %c %s\n", baralho[k].nome, baralho[k].naipe);

    return k;
}

void ler_players(){

    for(int i = 0; i < 2; i++){
        printf("Digite o nome do %dº time: ", i+1);
        fgets(times[i], 50, stdin);
        times[i][strlen(times[i])-1] = '\0';
    }

    for(int i = 0; i < JOGADORES; i++){

        if(i % 2 == 0){
            payers[i].time = false;
        }else{
            payers[i].time = true;
        }

        printf("Player %d, digite seu nome: ", i+1);
        fgets(payers[i].nome, 50, stdin);
        payers[i].nome[strlen(payers[i].nome)-1] = '\0';
        printf("Player %s, você faz parte do time %s\n", payers[i].nome, times[(int)payers[i].time]);
    }
}

void dar_cartas(){
    int k;
    for(int i = 0; i < JOGADORES; i++){
        for(int j = 0; j < MAO; j++){
            do{
                k = randomico(40);
            }while(baralho[k].status != 0);

            payers[i].carta_payer[j] = k;
            baralho[k].status = -1;
        }
    }
}