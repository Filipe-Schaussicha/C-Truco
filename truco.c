/*
 * Jogo de Truco (Versão Paulista)
 *
 * Autor: Filipe S. de Medeiros
 * Curso: Bacharelado em Ciências da Computação - UNESP
 * Data: Julho de 2025
 *
 * Descrição:
 * Implementação em linguagem C de uma versão simplificada do jogo de Truco,
 * com suporte a:
 *  - Embaralhamento e distribuição de cartas
 *  - Rodadas com comparação de cartas
 *  - Sistema de pontuação
 *  - Mecânica de pedidos de Truco (3, 6, 9, 12)
 * 
 * Observação:
 * Por ser uma versão em terminal, ambos os jogadores compartilham a mesma tela.
 *
 * Objetivo:
 * Desenvolvido com fins educacionais e como desafio pessoal para praticar lógica
 * de programação, manipulação de estruturas e controle de fluxo em C.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define CARTAS 40 // Total de cartas utilizadas no jogo
#define MAX_JOGADORES 8 // Máximo de jogadores permitidos
#define MAO 3 // Quantas cartas cada jogador vai ter

void embaralhar();
void ler_players();
int randomico(int n);
void mostrar_baralho();
int vira();
void dar_cartas();
void rodadas();
void falar_ganhador(int ganhador);

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

carta baralho[CARTAS]; // Array com todas as cartas possíveis
payer payers[MAX_JOGADORES]; // Arrey com as informações dos jogadores
time1 times[2]; // Arrey com as informações dos times
int jogadores; // Quantidade de jogadores do jogo (entrada de usuário)


int main(){

    ler_players();
    rodadas();

}

// Função responsável por popular o array baralho com todas as cartas do jogo
void embaralhar(){ 
    for(int i = 0; i < CARTAS; i++){

        baralho[i].status = 0; // Limpa qualquer lixo de memória

        baralho[i].valor = i / 4; 
        // Seta o "peso" da carta de acordo com a face (já que a cada 4 cartas a face muda pois existem 4 naipes para cada face), 
        // esse peso serve para ver que carta é mais forte

        // Por via de regra se eu divido a posição do baralho por 4 eu consigo descobrir a sua face
        // Agora se eu pego o resto dessa posição divido por 4, eu consigo detectar o naipe

        // Popula as faces
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

        // Popula os naipes
        switch(i % 4){ 
            case 0: strcpy(baralho[i].naipe, "Ouros"); break;
            case 1: strcpy(baralho[i].naipe, "Espadas"); break;
            case 2: strcpy(baralho[i].naipe, "Copas"); break;
            case 3: strcpy(baralho[i].naipe, "Paus"); break;
        }

    }

}

// Apenas pega um número aleatório entre 0 e n (n não está incluso)
int randomico(int n){ 

    srand((int)clock());

    return rand() % n;
}

// Mostra todo o baralho e suas estatísticas (usado para debug)
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

// Escolhe a carta que será a vira e por consequência as manilhas
int vira(){ 

    int vira = randomico(40); // Escolhe a vira

    int primeira_manilha = ((vira/4)*4) + 4; 
    // Seleciona a primeira manilha 
    // (e eu não sou um idiota por dividir por 4 e depois multiplicar por 4, 
    // faz sentido pois a variável vira é int, o que faz que nesse processo possamos selecionar o primeira carta que tem a face da vira)

    // Caso a vira tenha face "3" a manilhas serão as cartas com face "4" que está no começo do array, por isso essa correção é necessária
    (primeira_manilha >= 40) && (primeira_manilha = 0); 
    
    // Seta novos pesos para as maninhas, dependendo de seu naipe
    for(int i = 0; i < 4; i++, primeira_manilha++){ 
        baralho[primeira_manilha].valor = 10 + i;
    }

    baralho[vira].status = 2; // Indica que a vira não está mais disponível no baralho

    printf("A vira é: \n|\t %c %s \t|\n\n", baralho[vira].nome, baralho[vira].naipe); // Diz qual é a vira

    return vira; // Retorna a posição da vira no baralho, que não é utilizado em lugar nenhum :(
}

// Popula as informações de players e times, de acordo com a entrada de usuário
void ler_players(){ 

    // Pergunta quantos payers vão jogar (apenas números pares, positivos e que são menores que o máximo permitido)
    do{ 
        printf("Digite a quantidade de jogadores (Apenas números pares, lembrando que o máximo de jogadores é %d): ", MAX_JOGADORES);
        scanf("%d", &jogadores);
        getchar();
    }while(jogadores % 2 != 0 || jogadores < 1 || jogadores > MAX_JOGADORES);

    // Recebe o nome dos times
    for(int i = 0; i < 2; i++){ 
        printf("Digite o nome do %dº time: ", i+1);
        fgets(times[i].nome, 50, stdin);
        times[i].nome[strlen(times[i].nome)-1] = '\0';
        times[i].pontos = 0;
    }

    // Facilita o trabalho de escrever se um player é controlado por computador ou não (usado mais a frente)
    char s[2][8] = {"não é", "é"}; 

    // Lê informações dos jogadores
    for(int i = 0; i < jogadores; i++){ 

        int opt;

        // Distrubui os jogadores nos times de forma alternada
        (i % 2 == 0) ? (payers[i].time = false) : (payers[i].time = true); 

        // Recebe o nome dos players
        printf("Player %d, digite seu nome: ", i+1);
        fgets(payers[i].nome, 50, stdin);
        payers[i].nome[strlen(payers[i].nome)-1] = '\0';
        

        // Pergunta se o payer vai ser controlado por computador ou não
        do{ 
            printf("Esse player é um pessoa (digite 0) ou computador (digite 1): ");
            scanf("%d", &opt);
            getchar();
        }while(opt != 0 && opt != 1);

        (opt == 0) ? (payers[i].cpu = false) : (payers[i].cpu = true);

        // Escreve as informações recem gravadas
        printf("Player %s, você faz parte do time %s e %s controlado pelo computador\n\n", payers[i].nome, times[(int)payers[i].time].nome, s[payers[i].cpu]);
        
    }
}

// Distribui as cartas de forma aleatória
void dar_cartas(){ 
    
    int cartaAleatoria;
    for(int i = 0; i < jogadores; i++){
        for(int j = 0; j < MAO; j++){
            do{
                cartaAleatoria = randomico(40);
            }while(baralho[cartaAleatoria].status != 0);

            payers[i].carta_payer[j] = cartaAleatoria;
            baralho[cartaAleatoria].status = -1;
        }
    }
}

// Aqui onde o jogo realmente acontece
void rodadas(){

    int opt;

    // Repete por todas as rodadas do jogo, o jogo acaba quanto aguém chegar em 12 pontos
    do{ 
        printf("\n");
        embaralhar();
        vira();
        dar_cartas();

        int Turno = 0, trucos = 0, parcial[] = {0,0}; 
        // Turno = indica em qual turno estamos
        // Trucos = indica a quantidade de trucos pedidos
        // Parcial = indica a pontuação de cada time nessa mão

        int askTruco = -1, trucado = -1, ganhador = -1, fazendo = -1;
        // -1 = Nenhum time, 0 = primeiro time, 1 = segundo time
        // AskTruco = Salva o time que pediu truco na última vez
        // Trucado = Indica qual time está sendo trucado agora, quando ele aceitar o truco, esse valor volta pra -1
        // ganhador = Indica o ganhador dessa mão
        // fazendo = indica o time que jogou a carta mais forte, e logo está ganhando esse turno
        
        // Repete por todos os 2 ou 3 turnos (Depende se o 2 turno acabou empatado ou não)
        do{ 
            // Esse array salva a carta mais forte jogada por cada time nesse turno, o -2 indica que esse time ainda não jogo nenhuma carta
            int cartasJogadas[2] = {-2,-2};

            // Dá a oportunidade de cada jogador jogar uma vez
            for(int i = 0; i < jogadores; i++){

                // Indica quantas cartas esse jogador tem na sua mão pra jogar nesse turno (varia entre 1 e 3)
                int cartasDisponiveis = 0; 
                bool timeAtual = payers[i].time;

                // Escreve todas as cartas desse jogador que ainda não foram jogadas
                (times[(int)timeAtual].pontos == 11 || times[(int)!timeAtual].pontos == 11) ? printf("RODADA AS CEGAS") : printf("Vez do Player %s\nAs suas cartas são:\n",payers[i].nome);

                for(int j = 0; j < MAO; j++){
                    if(baralho[payers[i].carta_payer[j]].status == -1){
                        ++cartasDisponiveis;
                        if(times[(int)timeAtual].pontos < 11 && times[(int)!timeAtual].pontos < 11){
                            printf("Carta %d: %c %s\n", cartasDisponiveis, baralho[payers[i].carta_payer[j]].nome, baralho[payers[i].carta_payer[j]].naipe);
                        }
                    }
                }
                printf("\n");


                // Verifica se o time desse player já foi trucado
                if((int)timeAtual == trucado){ 

                    do{
                        (trucos == 0) ? printf("Pediram truco, aceitar? (1 = sim, 0 = não): ") : printf("Pediram %d, aceitar? (1 = sim, 0 = não): ", (trucos+1)*3);

                        (payers[i].cpu) ? opt = (randomico(3)) % 2 : scanf("%d", &opt);

                    }while(opt != 0 && opt != 1);

                    // Se o player aceitar o truco, o número de "trucos pedidos" aumenta em 1 e esse time não é mais considerado "trucado"
                    if(opt == 1){ 
                        trucos++;
                        trucado = -1;
                    }else{ // Caso ele regeite, já acaba essa rodada/mão, com o time opositor ganhando e já pulando para a contagem de pontos
                        ganhador = (int)!timeAtual;
                        goto fimTurno;
                    }
                }

                // Se o time desse player não foi o último a pedir truco, ele terá a opção de pedir
                if(askTruco != (int)timeAtual && trucos < 6 && times[(int)timeAtual].pontos < 11){ 

                    do{

                        (trucos == 0) ? printf("Você quer pedir truco? (1 = sim, 0 = não): ") : printf("Você quer pedir %d? (1 = sim, 0 = não): ", (trucos+1)*3);

                        (payers[i].cpu) ? opt = (randomico(3)) % 2 : scanf("%d", &opt);

                    }while(opt != 0 && opt != 1);

                    if(opt == 1){
                        askTruco = (int)timeAtual;
                        trucado = (int)!timeAtual;
                    }
                    
                }

                // Pergunta ao payer qual carta ele quer jogar (entre as que ainda não foram)
                do{ 

                    (cartasDisponiveis > 1) ? printf("Que carta você quer jogar? (digite um número entre 1 e %d): ",cartasDisponiveis) : printf("Digite 1 para jogar sua última carta: ");

                    (payers[i].cpu) ? opt = randomico(cartasDisponiveis) + 1 : scanf("%d", &opt);

                }while(opt < 1 || opt > cartasDisponiveis);

                // A variável opt que indica a opção do payer não bate com a posição da carta no array carta_payer, esse loop só serve para ageitar isso
                for(int cont = 0, temp = 0; cont < MAO; cont++){ 

                    (baralho[payers[i].carta_payer[cont]].status == -1) && temp++;

                    (temp == opt) && (opt = cont);

                }

                // Verifica se carta que o player jogo é maior do que as que foram jogas antes por seu time, se for ela será salva no array cartasJogadas do seu time
                (payers[i].carta_payer[opt] > cartasJogadas[(int)timeAtual]) && (cartasJogadas[(int)timeAtual] = payers[i].carta_payer[opt]);  

                // Registra que essa carta foi jogada
                baralho[payers[i].carta_payer[opt]].status = 1; 

                // Verifica qual carta mais forte de cada time é a mais forte, e depois escreve o resultado na tela
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

            // Verifica quem ganhou o turno
            if(fazendo == -1){
                printf("O turno terminou empachada\n\n");
            }else{
                printf("o time %s ganhou esse turno\n\n", times[fazendo].nome);
                parcial[fazendo]++;
            }

            Turno++;
        }while((Turno < 2 || parcial[0] == parcial[1]) && Turno < 3);

        // Verifica quem ganhou a rodada e faz a adição dos pontos baseado na quantidade de trucos pedidos
        if(parcial[0] != parcial[1]){
            // Verifica quem ganhou a rodada
            (parcial[0] > parcial[1]) ? (ganhador = 0) : (ganhador = 1);

            fimTurno:

            // Adiciona a pontuação, baseada na quantidade de trucos
            (trucos == 0) ? (times[ganhador].pontos++) : (times[ganhador].pontos += 3 * trucos);

            printf("\nO time %s ganhou esse turno\nPontos totais: \n%s: %d pontos\n%s: %d pontos\n\n", times[ganhador].nome, times[0].nome, times[0].pontos, times[1].nome, times[1].pontos);

        }else{
            // Se caso houver empate
            printf("\nNenhum time ganhou esse turno\nPontos totais: \n%s: %d pontos\n%s: %d pontos\n\n", times[0].nome, times[0].pontos, times[1].nome, times[1].pontos);
        }

    }while(times[0].pontos < 12 && times[1].pontos < 12);

    (times[0].pontos > times[1].pontos) ? falar_ganhador(0) : falar_ganhador(1);

}

// Apenas escreve o nome do time ganhador de uma forma mais estilosa
void falar_ganhador(int ganhador){

    int len = strlen(times[ganhador].nome);

    printf(" ");
    for(int i = -2; i < len; i++){
        printf("_");
    }
    printf("\n/");
    for(int i = -2; i < len; i++){
        printf(" ");
    }

    printf("\\\n| %s |\n\\", times[ganhador].nome);

    for(int i = -2; i < len; i++){
        printf("_");
    }

    printf("/\n");
}