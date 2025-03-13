#include <stdio.h>
#include <conio.c>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

typedef struct {
	char name[30];
	int game_mode=0;
	int number=0;					//dados dos jogadores
	int placar=0;
}player;

void posicao(int horizontal, int vertical) {
    COORD coord;
    coord.X = horizontal;														//posicao das maos
    coord.Y = vertical;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void texto(int color_text, int color_background) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (color_text + (color_background * 16)));	//ajuste da cor do fundo
}

void logo()
{
	int largura = 99;
    int altura = 10;

    // Desenho de uma mão em ASCII
    char *draw[10] = {
        "        0000                00000     000000         0000         0000       000000     00000000         ", 
        "     0001111000           00     00   00    00    000    00     00    00     00   00    00              ", 
        "   00111100111100        00       00  00    00  00            00        00   00    00   00             ", 
        "  0111110000111110       00       00  00    00 00             00        00   00     00  00           ", 
        "  0111100000011110       00000000000  000000   00             000000000000   00     00  00000000        ", 
        "  0111100000011110       00       00  00   00  00             00        00   00     00  00000000    ", 
        "  0111110000111110       00       00  00   00  00             00        00   00     00  00           ",
        "   00111100111100        00       00  00   00   00            00        00   00     00  00             ", 
        "     0001111000          00       00  00   00     000    00   00        00   00    00   00          ", 
        "        0000             00       00  00   00        0000     00        00   000000     00000000                       "  
    };

    // Definindo o fundo da tela como verde
    system("cls"); // Limpa a tela
    texto(0, 2); // Texto preto com fundo verde
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++) {
            printf(" ");
        }
    }

    for (int vertical = 0; vertical < altura; vertical++) {
        for (int horizontal = 0; horizontal < largura; horizontal++) {
            posicao(horizontal + 10, vertical + 5); 

            if (draw[vertical][horizontal] == '1') {
                texto(0, 6); // Cor preta no fundo e tom de pele claro para a mão
                printf(" ");
            } else if (draw[vertical][horizontal] == '0') {
                texto(0, 0); // Cor preta no fundo para '0'
                printf(" ");
            } else {
                texto(0, 2); // Fundo verde para espaços vazios
                printf(" ");
            }
        }
    }

    texto(7, 2);
    posicao(0, altura + 10); 	
    textcolor(BLACK);
}

void trofeu()
{
	int largura = 29;
    int altura = 17;

    // Desenho de uma mão em ASCII
    char *draw[17] = { 
        "       0000000000000000", 
        "       0011111111111100               ", 
        " 0000000011111001111100000000                ", 
        " 0011110011100001111100111100       ", 
        " 0011110011111001111100111100        ", 
        "   001100111110011111001100        ",
		"     00001111100111110000              ", 
        "       0011111111111100             ", 
        "        00111111111100        ", 
		"         00011111000       ",
		"           0011100           ",
		"           0011100             ",
		"           0011100         ",
		"           0011100         ",
		"           0011100           ",
		"          001111100          ",
		"          000000000            " 
    };

    // Definindo o fundo da tela como verde
    system("cls"); // Limpa a tela
    texto(0, 2); // Texto preto com fundo verde
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++) {
            printf(" ");
        }
    }

    for (int vertical = 0; vertical < altura; vertical++) {
        for (int horizontal = 0; horizontal < largura; horizontal++) {
            posicao(horizontal + 10, vertical + 5); 

            if (draw[vertical][horizontal] == '1') {
                texto(0, 6); // Cor preta no fundo e tom de pele claro para a mão
                printf(" ");
            } else if (draw[vertical][horizontal] == '0') {
                texto(0, 0); // Cor preta no fundo para '0'
                printf(" ");
            } else {
                texto(0, 2); // Fundo verde para espaços vazios
                printf(" ");
            }
        }
    }

    texto(7, 2);
    posicao(0, altura + 10); 	
    textcolor(BLACK);
}

void handzero()
{
	 int largura = 21;
    int altura = 10;

    // Desenho de uma mão em ASCII
    char *draw[10] = {
        "   0000000000000  ", 
        "   0110110110110   ", 
        "   0110110110110   ", 
        "   0110110110110            ", 
        "   0110000000000   ", 
        "   0101111111110   ", 
        "   0110000000000   ",
        "    01111111110    ", 
        "    01111111110    ", 
        "    0000000000     "  
    };

    // Definindo o fundo da tela como verde
    system("cls"); // Limpa a tela
    texto(0, 2); // Texto preto com fundo verde
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++) {
            printf(" ");
        }
    }

    // Desenho da mão com fundo verde e '0' preto
    for (int vertical = 0; vertical < altura; vertical++) {
        for (int horizontal = 0; horizontal < largura; horizontal++) {
            posicao(horizontal + 10, vertical + 5); 

            if (draw[vertical][horizontal] == '1') {
                texto(0, 6); // Cor preta no fundo e tom de pele claro para a mão
                printf(" ");
            } else if (draw[vertical][horizontal] == '0') {
                texto(0, 0); // Cor preta no fundo para '0'
                printf(" ");
            } else {
                texto(0, 2); // Fundo verde para espaços vazios
                printf(" ");
            }
        }
    }

    texto(7, 2);
    posicao(0, altura + 10); 	
    textcolor(BLACK);
}
void handone()
{
	 int largura = 21;
    int altura = 10;

    // Desenho de uma mão em ASCII
    char *draw[10] = {
        "   0000           ", 
        "   0110            ", 
        "   0110            ", 
        "   0110            ", 
        "   011000000000    ", 
        "   0101111111110   ", 
        "   0110000000000   ", 
        "    01111111110    ", 
        "    01111111110    ", 
        "    0000000000     "  
    };

    // Definindo o fundo da tela como verde
    system("cls"); // Limpa a tela
    texto(0, 2); // Texto preto com fundo verde
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++) {
            printf(" ");
        }
    }

    // Desenho da mão com fundo verde e '0' preto
    for (int vertical = 0; vertical < altura; vertical++) {
        for (int horizontal = 0; horizontal < largura; horizontal++) {
            posicao(horizontal + 10, vertical + 5); 

            if (draw[vertical][horizontal] == '1') {
                texto(0, 6); // Cor preta no fundo e tom de pele claro para a mão
                printf(" ");
            } else if (draw[vertical][horizontal] == '0') {
                texto(0, 0); // Cor preta no fundo para '0'
                printf(" ");
            } else {
                texto(0, 2); // Fundo verde para espaços vazios
                printf(" ");
            }
        }
    }

    texto(7, 2);
    posicao(0, altura + 10); 
	textcolor(BLACK);	 
}
void handtwo()
{
	 int largura = 21;
    int altura = 10;

    // Desenho de uma mão em ASCII
    char *draw[10] = {
        "   0000000        ", 
        "   0110110         ", 
        "   0110110         ", 
        "   0110110         ", 
        "   011011000000    ", 
        "   0111101111110   ", 
        "   0111110000000   ", 
        "    01111111110    ", 
        "    01111111110    ", 
        "    0000000000     "  
    };

    // Definindo o fundo da tela como verde
    system("cls"); // Limpa a tela
    texto(0, 2); // Texto preto com fundo verde
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++) {
            printf(" ");
        }
    }

    // Desenho da mão com fundo verde e '0' preto
    for (int vertical = 0; vertical < altura; vertical++) {
        for (int horizontal = 0; horizontal < largura; horizontal++) {
            posicao(horizontal + 10, vertical + 5); 

            if (draw[vertical][horizontal] == '1') {
                texto(0, 6); // Cor preta no fundo e tom de pele claro para a mão
                printf(" ");
            } else if (draw[vertical][horizontal] == '0') {
                texto(0, 0); // Cor preta no fundo para '0'
                printf(" ");
            } else {
                texto(0, 2); // Fundo verde para espaços vazios
                printf(" ");
            }
        }
    }

    texto(7, 2);
    posicao(0, altura + 10); 
    textcolor(BLACK);
}
void handthree()
{
	 int largura = 21;
    int altura = 10;

    // Desenho de uma mão em ASCII
    char *draw[10] = {
        "   0000000000      ", 
        "   0110110110      ", 
        "   0110110110     ", 
        "   0110110110     ", 
        "   011011000000    ", 
        "   0111101111110   ",
        "   0111110000000   ", 
        "    01111111110    ", 
        "    01111111110    ", 
        "    0000000000     "  
    };

    // Definindo o fundo da tela como verde
    system("cls"); // Limpa a tela
    texto(0, 2); // Texto preto com fundo verde
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++) {
            printf(" ");
        }
    }

    // Desenho da mão com fundo verde e '0' preto
    for (int vertical = 0; vertical < altura; vertical++) {
        for (int horizontal = 0; horizontal < largura; horizontal++) {
            posicao(horizontal + 10, vertical + 5); 

            if (draw[vertical][horizontal] == '1') {
                texto(0, 6); // Cor preta no fundo e tom de pele claro para a mão
                printf(" ");
            } else if (draw[vertical][horizontal] == '0') {
                texto(0, 0); // Cor preta no fundo para '0'
                printf(" ");
            } else {
                texto(0, 2); // Fundo verde para espaços vazios
                printf(" ");
            }
        }
    }

    texto(7, 2);
    posicao(0, altura + 10); 
    textcolor(BLACK);
}
void handfour()
{
	 int largura = 21;
    int altura = 10;

    // Desenho de uma mão em ASCII
    char *draw[10] = {
        "    00000000000   ", 
        "   0110110110110   ", 
        "   0110110110110   ", 
        "   0110110110110   ", 
        "   0110111000000   ", 
        "   0111110111110   ", 
        "   0111111000000   ", 
        "    01111111110    ", 
        "    01111111110    ", 
        "    0000000000     "  
    };

    // Definindo o fundo da tela como verde
    system("cls"); // Limpa a tela
    texto(0, 2); // Texto preto com fundo verde
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++) {
            printf(" ");
        }
    }

    // Desenho da mão com fundo verde e '0' preto
    for (int vertical = 0; vertical < altura; vertical++) {
        for (int horizontal = 0; horizontal < largura; horizontal++) {
            posicao(horizontal + 10, vertical + 5); 

            if (draw[vertical][horizontal] == '1') {
                texto(0, 6); // Cor preta no fundo e tom de pele claro para a mão
                printf(" ");
            } else if (draw[vertical][horizontal] == '0') {
                texto(0, 0); // Cor preta no fundo para '0'
                printf(" ");
            } else {
                texto(0, 2); // Fundo verde para espaços vazios
                printf(" ");
            }
        }
    }

    texto(7, 2);
    posicao(0, altura + 10); 
	textcolor(BLACK);	
}
void handfive()
{
	  int largura = 22;
    int altura = 10;

    // Desenho de uma mão em ASCII
     char *draw[10] = {
        "    00000000000   ", 
        "   0110110110110   ", 
        "   0110110110110   ", 
        "   011011011011000 ", 
        "   0110110110110110", 
        "   0111111111110110", 
        "   0111111111111110", 
        "    01111111111100  ", 
        "    011111111110    ", 
        "    00000000000    "  
    };

    // Definindo o fundo da tela como verde
    system("cls"); // Limpa a tela
    texto(0, 2); // Texto preto com fundo verde
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++) {
            printf(" ");
        }
    }

    // Desenho da mão com fundo verde e '0' preto
    for (int vertical = 0; vertical < altura; vertical++) {
        for (int horizontal = 0; horizontal < largura; horizontal++) {
            posicao(horizontal + 10, vertical + 5); 

            if (draw[vertical][horizontal] == '1') {
                texto(0, 6); // Cor preta no fundo e tom de pele claro para a mão
                printf(" ");
            } else if (draw[vertical][horizontal] == '0') {
                texto(0, 0); // Cor preta no fundo para '0'
                printf(" ");
            } else {
                texto(0, 2); // Fundo verde para espaços vazios
                printf(" ");
            }
        }
    }

    texto(7, 2);
    posicao(0, altura + 10); 
    textcolor(BLACK);
}
void multplayer()
{
	player p1,p2;
	p1.placar=0;
	p2.placar=0;
	int conta=0,winer=0,limite=0,onemoretime=0;
	textcolor(BLACK);
do{
	fflush(stdin);
	printf("informe o nome do jogador 1: ");
	gets(p1.name);
	fflush(stdin);
	printf("\ninforme o nome do jogador 2: ");		//nome dos jogadores
	gets(p2.name);
	fflush(stdin);
	
	do{
		printf("\n%s escolha 0 para jogar como par ou 1 para jogar como impar: ",p1.name);
		scanf("%d",&p1.game_mode);
		do{
			if(p1.game_mode==1){
				printf("\nMuito bem, voce escolheu impar, e %s esta como par",p2.name);			//modo de jogo
				p2.game_mode=0;
			}
			else if(p1.game_mode==0){
				printf("\nMuito bem, voce escolheu par, e %s esta como impar",p2.name);
				p2.game_mode=1;
			}
			else{
				printf("\nvalor invalido, por favor redigite: ");
				scanf("%d",&p1.game_mode);
			}
			
		}while(p1.game_mode>1 || p1.game_mode<0);
		
		printf("\n\n%s, escolha um numero entre 0 e 5: ",p1.name);
		textcolor(GREEN);
		scanf("%d",&p1.number);
		do{
			if(p1.number>5){
				textcolor(BLACK);
				printf("\nvalor invalido, por favor redigite: ");
				textcolor(GREEN);
				scanf("%d",&p1.number);
			}
			else if(p1.number<0){
				textcolor(BLACK);
				printf("\nvalor invalido, por favor redigite: ");		//numero do jogador 1
				textcolor(GREEN);
				scanf("%d",&p1.number);
			}
		}while(p1.number>5 || p1.number<0);
		
		textcolor(BLACK);
		printf("\n%s, escolha um numero entre 0 e 5: ",p2.name);
		textcolor(GREEN);
		scanf("%d",&p2.number);
		do{
			if(p2.number>5){
				textcolor(BLACK);
				printf("\nvalor invalido, por favor redigite: ");		//numero do jogador 2
				textcolor(GREEN);
				scanf("%d",&p2.number);
				textcolor(BLACK);
			}
			else if(p2.number<0){
				textcolor(BLACK);
				printf("\nvalor invalido, por favor redigite: ");
				textcolor(GREEN);
				scanf("%d",&p2.number);
				textcolor(BLACK);
			}
		}while(p2.number>5 || p2.number<0);
		
		switch(p1.number){
			case 0:
				handzero();
				break;
			case 1:
				handone();
				break;
			case 2:
				handtwo();
				break;
			case 3:
				handthree();		//mao do jogador 1
				break;
			case 4:
				handfour();
				break;
			case 5:
				handfive();
				break;
			default:
				break;
		}
			Sleep(1000);
			
			switch(p2.number){
			case 0:
				handzero();
				break;
			case 1:
				handone();
				break;
			case 2:
				handtwo();
				break;
			case 3:
				handthree();			//mao do jogador 2
				break;
			case 4:
				handfour();
				break;
			case 5:
				handfive();
				break;
			default:
				break;
		}
		textbackground(GREEN);
		textcolor(BLACK);
		conta=p1.number+p2.number;
		winer=conta%2;
		Sleep(1000);
		trofeu();
		switch (winer){
			case 0:
				if(p1.game_mode==0){
					textcolor(BLACK);
					printf("\n\n\nparabens, %s venceu",p1.name);
					p1.placar+=1;
				}
				else if(p2.game_mode==0){
					textcolor(BLACK);
					printf("\n\n\nparabens, %s venceu",p2.name);
					p2.placar+=1;
				}
				break;
			case 1:																//verifica o vencedor
				if(p1.game_mode==1){
					textcolor(BLACK);
					printf("\n\n\nparabens, %s venceu",p1.name);
					p1.placar+=1;
				}
				else if(p2.game_mode==1){
					textcolor(BLACK);
					printf("\n\n\nparabens, %s venceu",p2.name);
					p2.placar+=1;
				}
				break;
				default:
					break;
		}
		
		printf("\n\ngostaria de jogar de novo?");
		printf("\n\n digite 1 para sim ou 0 para nao: ");
		scanf("%d",&limite);
		onemoretime+=1;
		do{																//verifica se o usuario quer jogar dnv
			if(limite>1||limite<0){
				printf("\n\nnumero invalido, redigite: ");
				scanf("%d",&limite);
				if(limite==0){
					onemoretime=3;
				}
			}
			else if(limite==0){
				onemoretime=3;
			}
		}while(limite>1||limite<0);
	}while(onemoretime<3);
	textbackground(WHITE);	
	printf("\n\nPLACAR:      ");
	printf("\n%s venceu: %d",p1.name,p1.placar);
	printf("\n%s venceu: %d",p2.name,p2.placar);
	textbackground(GREEN);
	printf("\n");
	p1.placar=0;
	p2.placar=0;
	onemoretime=0;
	}while (limite==1);
}

void singleplayer()
{
	textcolor(BLACK);
	player p1,pm;
	p1.placar=0;
	pm.placar=0;
	int conta=0,winer=0,limite=0,playermachine=0,onemoretime=0,playermachine_game=0;
do{
	fflush(stdin);
	printf("informe o nome do jogador: ");	//nome do jogador
	gets(p1.name);
	fflush(stdin);
	
	do{
		printf("\n%s escolha 0 para jogar como par ou 1 para jogar como impar: ",p1.name);
		scanf("%d",&p1.game_mode);
		do{
			if(p1.game_mode==1){
				printf("\nMuito bem, voce escolheu impar");			//modo de jogo
				playermachine_game=0;
			}
			else if(p1.game_mode==0){
				printf("\nMuito bem, voce escolheu par");
				playermachine_game=1;
			}
			else{
				printf("\nvalor invalido, por favor redigite: ");
				scanf("%d",&p1.game_mode);
				if (p1.game_mode==0){
					playermachine_game=1;
				}
				else if(p1.game_mode==1){
					playermachine_game=0;
				}
			}
			
		}while(p1.game_mode>1 || p1.game_mode<0);
		
		printf("\n\n%s, escolha um numero entre 0 e 5: ",p1.name);
		scanf("%d",&p1.number);
		do{
			if(p1.number>5){
				printf("\nvalor invalido, por favor redigite: ");
				scanf("%d",&p1.number);
			}
			else if(p1.number<0){
				printf("\nvalor invalido, por favor redigite: ");		//numero do jogador 1
				scanf("%d",&p1.number);
			}
		}while(p1.number>5 || p1.number<0);
		
		srand(time(NULL));
		playermachine= rand()%6;
		
		switch(p1.number){
			case 0:
				handzero();
				break;
			case 1:
				handone();
				break;
			case 2:
				handtwo();
				break;
			case 3:
				handthree();		//mao do jogador 1
				break;
			case 4:
				handfour();
				break;
			case 5:
				handfive();
				break;
			default:
				break;
		}
			Sleep(1000);
			switch(playermachine){
			case 0:
				handzero();
				break;
			case 1:
				handone();
				break;
			case 2:
				handtwo();
				break;
			case 3:
				handthree();			//mao da maquina
				break;
			case 4:
				handfour();
				break;
			case 5:
				handfive();
				break;
			default:
				break;
		}
		textcolor(BLACK);
		conta=p1.number+playermachine;
		winer=conta%2;
		Sleep(600);
		textcolor(BLACK);
		Sleep(1000);
		trofeu();
		switch (winer){
			case 0:
				if(p1.game_mode==0){
					textcolor(BLACK);
					printf("\n\n\nparabens, %s venceu",p1.name);
					p1.placar+=1;
				}
				else if(playermachine_game==0){
					textcolor(BLACK);
					printf("\n\n\nVoce perdeu");
					pm.placar+=1;
				}
				break;
			case 1:																//verifica o vencedor
				if(p1.game_mode==1){
					textcolor(BLACK);
					printf("\n\n\n\nparabens, %s venceu",p1.name);
					p1.placar+=1;
				}
				else if(playermachine_game==1){
					textcolor(BLACK);
					printf("\n\n\n\nvoce perdeu");
					pm.placar+=1;
				}
				break;
				default:
					break;
		}
		textcolor(BLACK);
		printf("\n\ngostaria de jogar de novo?");
		printf("\n\n digite 1 para sim ou 0 para nao: ");
		scanf("%d",&limite);
		onemoretime+=1;
		do{																//verifica se o usuario quer jogar dnv
			if(limite>1||limite<0){
				printf("\n\nnumero invalido, redigite: ");
				scanf("%d",&limite);
				if (limite==0){
					onemoretime=3;				}
			}
			else if(limite==0){
				onemoretime=3;
			}
		}while(limite>1||limite<0);
	}while (onemoretime<3);
	textbackground(WHITE);
	printf("\n\nPLACAR:      ");
	printf("\n%s venceu: %d",p1.name,p1.placar);
	printf("\n%s perdeu: %d",p1.name,pm.placar);
	textbackground(GREEN);
	printf("\n");
	p1.placar=0;
	pm.placar=0;
	onemoretime=0;
	}while (limite==1);	
}

void jokenpo()
{
	player p1,p2;
	p1.placar=0;
	p2.placar=0;
	int conta=0,winer=0,limite=0,onemoretime=0,empate=0;
	textcolor(BLACK);
do{
	fflush(stdin);
	printf("informe o nome do jogador 1: ");
	gets(p1.name);
	fflush(stdin);
	printf("\ninforme o nome do jogador 2: ");		//nome dos jogadores
	gets(p2.name);
	fflush(stdin);
	
	do{
		
		printf("\n\n%s, escolha um numero entre 1 e 3: ",p1.name);
		printf("\n1-Pedra; 2-Papel; 3-Tesoura: ");
		textcolor(GREEN);
		scanf("%d",&p1.number);
		do{
			if(p1.number>3){
				textcolor(BLACK);
				printf("\nvalor invalido, por favor redigite: ");
				textcolor(GREEN);
				scanf("%d",&p1.number);
			}
			else if(p1.number<1){
				textcolor(BLACK);
				printf("\nvalor invalido, por favor redigite: ");		//numero do jogador 1
				textcolor(GREEN);
				scanf("%d",&p1.number);
			}
		}while(p1.number>3 || p1.number<1);
		
		textcolor(BLACK);
		printf("\n%s, escolha um numero entre 1 e 3: ",p2.name);
		textcolor(GREEN);
		scanf("%d",&p2.number);
		do{
			if(p2.number>3){
				textcolor(BLACK);
				printf("\nvalor invalido, por favor redigite: ");		//numero do jogador 2
				textcolor(GREEN);
				scanf("%d",&p2.number);
				textcolor(BLACK);
			}
			else if(p2.number<1){
				textcolor(BLACK);
				printf("\nvalor invalido, por favor redigite: ");
				textcolor(GREEN);
				scanf("%d",&p2.number);
				textcolor(BLACK);
			}
		}while(p2.number>3 || p2.number<1);
		
		switch(p1.number){
			case 1:
				handzero();
				break;
			case 2:
				handfive();
				break;
			case 3:
				handtwo();		//mao do jogador 1
				break;
			default:
				break;
		}
			Sleep(1000);
			
		switch(p2.number){
			case 1:
				handzero();
				break;
			case 2:
				handfive();
				break;
			case 3:
				handtwo();			//mao do jogador 2
				break;
			default:
				break;
		}
		textcolor(BLACK);
		if (p1.number==1 && p2.number==1){
			printf("Deu empate");
			empate+=1;
		}
		else if(p1.number==1 && p2.number==2){
			Sleep(1000);
			trofeu();
			printf("%s Venceu",p2.name);
			p2.placar+=1;
		}
		else if(p1.number==1 && p2.number==3){
			Sleep(1000);
			trofeu();
			printf("%s Venceu",p1.name);
			p1.placar+=1;
		}
		else if(p1.number==2 && p2.number==1){
			Sleep(1000);
			trofeu();
			printf("%s Venceu",p1.name);
			p1.placar+=1;
		}
		else if(p1.number==2 && p2.number==2){				//verifica vencedor
			printf("Deu empate");
			empate+=1;
		}
		else if(p1.number==2 && p2.number==3){
			Sleep(1000);
			trofeu();
			printf("%s Venceu",p2.name);
			p2.placar+=1;
		}
		else if(p1.number==3 && p2.number==1){
			Sleep(1000);
			trofeu();
			printf("%s Venceu",p2.name);
			p2.placar+=1;
		}
		else if(p1.number==3 && p2.number==2){
			Sleep(1000);
			trofeu();
			printf("%s Venceu",p1.name);
			p1.placar+=1;
		}
		else if(p1.number==3 && p2.number==3){
			printf("Deu empate");
			empate+=1;
		}
		
		printf("\n\ngostaria de jogar de novo?");
		printf("\n\n digite 1 para sim ou 0 para nao: ");
		scanf("%d",&limite);
		onemoretime+=1;
		do{																//verifica se o usuario quer jogar dnv
			if(limite>1||limite<0){
				printf("\n\nnumero invalido, redigite: ");
				scanf("%d",&limite);
				if(limite==0){
					onemoretime=3;
				}
			}
			else if(limite==0){
				onemoretime=3;
			}
		}while(limite>1||limite<0);
	}while(onemoretime<3);
	textbackground(WHITE);	
	printf("\n\n\n\n\nPLACAR:      ");
	printf("\n%s venceu: %d",p1.name,p1.placar);
	printf("\nEmpates: %d  ",empate);
	printf("\n%s venceu: %d",p2.name,p2.placar);
	textbackground(GREEN);
	printf("\n");
	p1.placar=0;
	p2.placar=0;
	empate=0;
	onemoretime=0;
	}while (limite==1);
}

void singjokenpo()
{
	player p1,pm;
	pm.placar=0;
	p1.placar=0;
	int conta=0,winer=0,limite=0,onemoretime=0,playermachine=0,empate=0;
	textcolor(BLACK);
do{
	fflush(stdin);
	printf("informe o nome do jogador: ");  //nome do jogador
	gets(p1.name);
	fflush(stdin);
	
	do{
		
		printf("\n\n%s, escolha um numero entre 1 e 3: ",p1.name);
		printf("\n1-Pedra; 2-Papel; 3-Tesoura: ");
		scanf("%d",&p1.number);
		do{
			if(p1.number>3){
				printf("\nvalor invalido, por favor redigite: ");
				scanf("%d",&p1.number);
			}
			else if(p1.number<1){
				printf("\nvalor invalido, por favor redigite: ");		//numero do jogador 1
				scanf("%d",&p1.number);
			}
		}while(p1.number>3 || p1.number<1);
		
		textcolor(BLACK);
		srand(time(NULL));
		playermachine= (rand()%3)+1; 
		
		switch(p1.number){
			case 1:
				handzero();
				break;
			case 2:
				handfive();
				break;
			case 3:
				handtwo();		//mao do jogador 1
				break;
			default:
				break;
		}
			Sleep(1000);
			
		switch(playermachine){
			case 1:
				handzero();
				break;
			case 2:
				handfive();
				break;
			case 3:
				handtwo();			//mao da maquina
				break;
			default:
				break;
		}
		textcolor(BLACK);
		if (p1.number==1 && playermachine==1){
			printf("\n\n\nDeu empate");
			empate+=1;
		}
		else if(p1.number==1 && playermachine==2){
			printf("\n\n\nVoce perdeu");
			pm.placar+=1;
		}
		else if(p1.number==1 && playermachine==3){
			Sleep(1000);
			trofeu();
			printf("\n\n\n%s Venceu",p1.name);
			p1.placar+=1;
		}
		else if(p1.number==2 && playermachine==1){
			Sleep(1000);
			trofeu();
			printf("\n\n\n%s Venceu",p1.name);
			p1.placar+=1;
		}
		else if(p1.number==2 && playermachine==2){				//verifica vencedor
			printf("\n\n\nDeu empate");
			empate+=1;
		}
		else if(p1.number==2 && playermachine==3){
			printf("\n\n\nVoce perdeu");
			pm.placar+=1;
		}
		else if(p1.number==3 && playermachine==1){
			printf("\n\n\nVoce perdeu");
			pm.placar+=1;
		}
		else if(p1.number==3 && playermachine==2){
			Sleep(1000);
			trofeu();
			printf("\n\n\n%s Venceu",p1.name);
			p1.placar+=1;
		}
		else if(p1.number==3 && playermachine==3){
			printf("\n\n\nDeu empate");
			empate+=1;
		}
		
		printf("\n\n\ngostaria de jogar de novo?");
		printf("\n\n digite 1 para sim ou 0 para nao: ");
		scanf("%d",&limite);
		onemoretime+=1;
		do{																//verifica se o usuario quer jogar dnv
			if(limite>1||limite<0){
				printf("\n\nnumero invalido, redigite: ");
				scanf("%d",&limite);
				if(limite==0){
					onemoretime=3;
				}
			}
			else if(limite==0){
				onemoretime=3;
			}
		}while(limite>1||limite<0);
	}while(onemoretime<3);
	textbackground(WHITE);	
	printf("\nPLACAR:      ");
	printf("\n%s venceu: %d",p1.name,p1.placar);
	printf("\nEmpates: %d  ",empate);								//placar
	printf("\n%s Perdeu: %d",p1.name,pm.placar);
	textbackground(GREEN);
	printf("\n");
	p1.placar=0;
	pm.placar=0;
	empate=0;
	onemoretime=0;
	}while (limite==1);
}

main(){
	
	int qtdplayers=0,game=0,endcode=0;
	// Definindo o fundo da tela como verde
    system("cls"); // Limpa a tela
    texto(0, 2); // Texto preto com fundo verde
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 160; j++) {
            printf(" ");
        }
    }
    do{
		
		gotoxy(1,1);
  	  	logo();
		textcolor(BLACK);
		textcolor(RED);
		printf("Starlight Company");
		textcolor(BLACK);
		printf("\no Jogo funciona com um sistema de melhor de 3");							//menu principal
		printf("\nescolha o modo de jogo");
		printf("\ndigite 0 para jogar <Par ou Impar>; 1 para jogar <Jokenpo> ou 2 para sair do jogo: ");
		scanf("%d",&game);
		
	
		do{
			if(game<0){
				printf("valor invalido,redigite: ");
				scanf("%d",&game);
			}
			else if(game>2){
				printf("valor invalido,redigite: ");
				scanf("%d",&game);
			}
		}while(game<0|| game>2);
	
		if (game==0){
			printf("Digite 0 para Singleplayer ou 1 para Multplayer: ");
			scanf("%d",&qtdplayers);
			do{
				if(qtdplayers<0){
				printf("valor invalido,redigite: ");
				scanf("%d",&game);
				}
			else if(qtdplayers>1){
				printf("valor invalido,redigite: ");
				scanf("%d",&game);
				}
			}while(qtdplayers>1 || qtdplayers<0);
			switch (qtdplayers){
				case 0:
					singleplayer();
					Sleep(2000);
					endcode=1;									//chama o modo de jogo
					break;
				case 1:
					multplayer();
					Sleep(2000);
					endcode=1;
					break;
				default:
					break;
			}
		}
		else if(game==1){
			printf("Digite 0 para Singleplayer ou 1 para Multplayer: ");
			scanf("%d",&qtdplayers);
			do{
				if(qtdplayers<0){
				printf("valor invalido,redigite: ");
				scanf("%d",&game);
				}
			else if(qtdplayers>1){
				printf("valor invalido,redigite: ");
				scanf("%d",&game);
				}
			}while(qtdplayers>1 || qtdplayers<0);
			switch (qtdplayers){
				case 0:
					singjokenpo();
					Sleep(2000);
					endcode=1;
					break;
				case 1:					//chama o modo de jogo
					jokenpo();
					Sleep(2000);
					endcode=1;
					break;
				default:
					break;
			}
		}
		else if (game==2){
			printf("\nJogo Feito Por: Arthur Moreira Felicio N:5; Gabriel Sulzbacher Coimbra de Souza N:10");
			printf("\nSerie:INI-1A");
			printf("\n\nObrigado Por Jogar :)");
			endcode=0;
		}
	}while(endcode!=0);
}




