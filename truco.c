#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define CARTAS 40 // Total de cartas utilizadas no jogo
#define MAX_JOGADORES 8
#define MAO 3 // Quantas cartas cada jogador vai ter

void embaralhar();
void ler_players();
int randomico(int n);
void mostrar_baralho();
int vira();
void dar_cartas();
void rodadas();
void sort_carta(int *array, int size);

typedef struct{
    char nome;
    char naipe[10];
    int status; // 0 = disponível, 1 = i, -1 = já foi pega
    int valor;
}carta;

typedef struct{
    char nome[50];
    bool time;
    bool cpu;
    int carta_payer[MAO];
}payer;

typedef struct{
    char nome[50];
    int pontos;
}time1;

carta baralho[CARTAS];
payer payers[MAX_JOGADORES];
time1 times[2];
int jogadores;


int main(){

    ler_players();
    rodadas();
    //mostrar_baralho();
    
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

    srand((int)clock());

    return rand() % n;
}

void mostrar_baralho(){

    for(int i = 0; i < CARTAS; i++){
        printf("Nome: %c, Naipe: %s, Valor: %d, Status %d\n", baralho[i].nome, baralho[i].naipe, baralho[i].valor, baralho[i].status);
    }
    printf("\n");

    for(int i = 0; i < jogadores; i++){
        printf("Nome: %s, Time: %s \nCartas: | ", payers[i].nome, times[(int)payers[i].time].nome);
        for(int j = 0; j < MAO; j++){
            printf("%c %s | ", baralho[payers[i].carta_payer[j]].nome, baralho[payers[i].carta_payer[j]].naipe);
        }
        printf("\n");
    }
}

int vira(){

    int k = randomico(40);
    int j = ((k/4)*4) + 4;

    (j >= 40) && (j = 0);

    for(int i = 0; i < 4; i++, j++){
        baralho[j].valor = 10 + i;
    }

    baralho[k].status = 2;

    printf("A vira é: \n|\t %c %s \t|\n\n", baralho[k].nome, baralho[k].naipe);

    return k;
}

void ler_players(){

    do{
        printf("Digite a quantidade de jogadores (Apenas números pares, lembrando que o máximo de jogadores é %d): ", MAX_JOGADORES);
        scanf("%d", &jogadores);
        getchar();
    }while(jogadores % 2 != 0 || jogadores < 1 || jogadores > MAX_JOGADORES);

    for(int i = 0; i < 2; i++){
        printf("Digite o nome do %dº time: ", i+1);
        fgets(times[i].nome, 50, stdin);
        times[i].nome[strlen(times[i].nome)-1] = '\0';
        times[i].pontos = 0;
    }

    char s[2][8] = {"não é", "é"};

    for(int i = 0; i < jogadores; i++){

        int opt;

        (i % 2 == 0) ? (payers[i].time = false) : (payers[i].time = true);

        printf("Player %d, digite seu nome: ", i+1);
        fgets(payers[i].nome, 50, stdin);
        payers[i].nome[strlen(payers[i].nome)-1] = '\0';

        do{
            printf("Esse player é um pessoa (digite 0) ou computador (digite 1): ");
            scanf("%d", &opt);
            getchar();
        }while(opt != 0 && opt != 1);

        (opt == 0) ? (payers[i].cpu = false) : (payers[i].cpu = true);

        printf("Player %s, você faz parte do time %s e %s controlado pelo computador\n\n", payers[i].nome, times[(int)payers[i].time].nome, s[payers[i].cpu]);
    }
}

void dar_cartas(){
    
    int k;
    for(int i = 0; i < jogadores; i++){
        for(int j = 0; j < MAO; j++){
            do{
                k = randomico(40);
            }while(baralho[k].status != 0);

            payers[i].carta_payer[j] = k;
            baralho[k].status = -1;
        }
    }
}

void rodadas(){

    int opt;

    do{
        printf("\n");
        embaralhar();
        vira();
        dar_cartas();

        int rodada = 0, trucos = 0, parcial[] = {0,0};
        int askTruco = -1, trucado = -1, ganhador = -1, fazendo = -1;
        //int cartasJogadas[2] = {-2,-2};
        
        do{
            int cartasJogadas[2] = {-2,-2};

            for(int i = 0; i < jogadores; i++){

                int k = 0;
                printf("Vez do Player %s\nAs suas cartas são:\n",payers[i].nome);
                for(int j = 0; j < MAO; j++){

                    (baralho[payers[i].carta_payer[j]].status == -1) && printf("Carta %d: %c %s\n", ++k, baralho[payers[i].carta_payer[j]].nome, baralho[payers[i].carta_payer[j]].naipe);

                }
                printf("\n");

                if((int)payers[i].time == trucado){

                    do{
                        (trucos == 0) ? printf("Pediram truco, aceitar? (1 = sim, 0 = não): ") : printf("Pediram %d, aceitar? (1 = sim, 0 = não): ", (trucos+1)*3);

                        (payers[i].cpu) ? opt = (randomico(3)) % 2 : scanf("%d", &opt);

                    }while(opt != 0 && opt != 1);

                    if(opt == 1){
                        trucos++;
                        trucado = -1;
                    }else{
                        ganhador = (int)!payers[i].time;
                        goto fimRodada;
                    }
                }

                if(askTruco != (int)payers[i].time){

                    do{

                        (trucos == 0) ? printf("Você quer pedir truco? (1 = sim, 0 = não): ") : printf("Você quer pedir %d? (1 = sim, 0 = não): ", (trucos+1)*3);

                        (payers[i].cpu) ? opt = (randomico(3)) % 2 : scanf("%d", &opt);

                    }while(opt != 0 && opt != 1);

                    if(opt == 1){
                        askTruco = (int)payers[i].time;
                        trucado = (int)!payers[i].time;
                    }
                    
                }

                do{

                    (k > 1) ? printf("Que carta você quer jogar? (digite um número entre 1 e %d): ",k) : printf("Digite 1 para jogar sua última carta: ");

                    (payers[i].cpu) ? opt = randomico(k) + 1 : scanf("%d", &opt);

                }while(opt < 1 || opt > k);

                for(int cont = 0, temp = 0; cont < MAO; cont++){

                    (baralho[payers[i].carta_payer[cont]].status == -1) && temp++;

                    (temp == opt) && (opt = cont);

                }

                (payers[i].carta_payer[opt] > cartasJogadas[(int)payers[i].time]) && (cartasJogadas[(int)payers[i].time] = payers[i].carta_payer[opt]);

                baralho[payers[i].carta_payer[opt]].status = 1;


                if(baralho[cartasJogadas[0]].valor > baralho[cartasJogadas[1]].valor){
                    printf("\nA carta | %c %s | do time \"%s\" está fazendo\n\n", baralho[cartasJogadas[0]].nome, baralho[cartasJogadas[0]].naipe, times[0].nome);
                    fazendo = 0;
                }else if(baralho[cartasJogadas[0]].valor < baralho[cartasJogadas[1]].valor){
                    printf("\nA carta | %c %s | do time \"%s\" está fazendo\n\n", baralho[cartasJogadas[1]].nome, baralho[cartasJogadas[1]].naipe, times[1].nome);
                    fazendo = 1;
                }else{
                    printf("Empachado, entre as cartas | %c %s | e | %c %s |\n\n", baralho[cartasJogadas[0]].nome, baralho[cartasJogadas[0]].naipe, baralho[cartasJogadas[1]].nome, baralho[cartasJogadas[1]].naipe);
                    fazendo = -1;
                }

            }

            if(fazendo == -1){
                printf("A rodada terminou empachada\n\n");
            }else{
                printf("o time %s ganhou essa rodada\n\n", times[fazendo].nome);
                parcial[fazendo]++;
            }

            rodada++;
        }while((rodada < 2 || parcial[0] == parcial[1]) && rodada < 3);

        fimRodada:

        

    }while(times[0].pontos < 12 && times[1].pontos < 12);

}

void sort_carta(int *array, int size){

    int buffer;

    for(int i = 0; i < size; i++){
        for(int j = i+1; j < size; j++){

            if(*(array+i) != -2 && *(array+j) != -2){
                if(baralho[*(array+i)].valor <= baralho[*(array+j)].valor){

                    buffer = *(array+i);
                    *(array+i) = *(array+j);
                    *(array+j) = buffer;

                }
            }
        }
    }

}