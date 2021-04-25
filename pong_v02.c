/************************************************************************************************************************************************************
 * PONG  - ASC
 * Segundo código do Curso de Desenvolvimento de Jogos sem Framework
 *
 * Este programa não é exatamente um jogo, uma vez que ele serve exclusivamente para demonstrar como uma IA - RNA Perceptron, pode ser utilizada para gerar
 * um comportamento competitivo em jogos do tipo plataforma.
 * O processo é realizado por uma RNA - Perceptron de uma camada com dois neurônios, 3 entradas e duas saídas.
 * Inicialmente a RNA desconhece completamente seus parâmetros de comportamento, pois seus pesos foram gerados aleatóriamente.
 * As entradas recebem o tempo inteiro do jogo, os valores de posições horizontais da "bola", do "bastão" (Jogador), e um valor inteiro setado como 1 sempre
 * que a bola estiver sobre o jogador. (Estas são as 3 entradas da RNA).
 * A saída da RNA fornece dois valores: o primeiro para indicar se o bastão deve ser movido para a direita ou para a esquerda (1 - 0 respectivamente).
 * 				      	o segundo indica se o bastão deve continuar se movendo ou parado (1 - 0 respectivamente).
 * Um vetor "*dese1" é fornecido como referência para os resultados, orientando a RNA com os valores ideais de saída (Quadrado dos erros).
 *
 * Para demonstrar o funcionamento do aprendizado, a rede deve treinar à rebater a bola 50 vezes. Uma vez que obtenha sucesso nesse processo, a fase de
 * treinamento é encerrada e inicia a fase de teste. Caso a RNA não consiga rebater sem erros, novamente a fase de aprendizado é retomada por mais 50 
 * acertos. Este procedimento segue de forma recorrente até que não haja necessidade de novo aprendizado (A fase de testes não comete mais erros!).
 *
 * Tecals para sair: [Q]
 *
 * Regras de compilação: gcc pong_v02.c -o pong_v02 -lncursers -lm
 *
 * Requisitos: gcc, ncurses, kbhit.c, perceptron02_sig_v02.c
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Desenvolvimento: Francisco Alves de Freitas Neto - IFF - Itaperuna
 * Data: 14/04/21
 **********************************************************************/
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include "kbhit.c" //Um pequeno código para auxiliar a capturar eventos
#include "perceptron02_sig_v02.c" //Introduzindo a RNA

//Dimensões da tela do Jogo
#define LINHAS 19
#define COLUNAS 60

//Variáveis Globais do jogo
int erros = 0;
int pontos = 0;
int estadoJogo=0; //Estados do Jogo: 0-Rodando 1-Fim
int rebateu = 0;

//Definições de Estruturas
typedef struct JOGADOR {
	int coluna;
}Jogador;

typedef struct BOLA{
	float linha;
	float coluna;
	float direcao;
}Bola;

//Função responsável por liberar os recursos "ncurses" alocados 
void fechar_jogo(WINDOW *telaJogo, Rna *rna, float *dese){
	free(rna);
	free(dese);
	delwin(telaJogo); //Removendo a tela do Jogo da memória
	endwin();         //Saindo do modo curses
	system("setterm -cursor on"); //Desligando o cursor
}

void preparar_valores_iniciais(Jogador *jogador, Bola *bola){ 
	erros      = 0;
	estadoJogo = 0;
	pontos     = 0;
	rebateu    = 0;
			
	jogador->coluna = (int) COLUNAS/2 - 1;
	//jogador->coluna = 50;

	bola->linha   = LINHAS/2;
	bola->coluna  = COLUNAS/4;
	bola->direcao = 45;
}

int main(){
	/***************************************************
	 * Configurações iniciais do JOGO
	 ***************************************************/

	//Criando a RNA
	//Parâmetros: qtd_camadas, tam_entrada, tam_saida
	Rna *rna1 = criar_rna(1, 3, 2);	

	//Lista dos valores ideias para treinamento da RNA (Comparação para gerar o erro de saída)
	float *dese1 = malloc(2*sizeof(float));
	
	//Semente aleatória do jogo
	srand(time(NULL));

	int evento; //Captura de eventos do teclado
	
	clock_t intervalo_bola = clock(); //Intervalo de tempo para passos do jogo
	clock_t intervalo_jogador = clock();
	int intervalo   = 30000;
	int intervalo_j = 10000;
	int aprendizado = 1;
	float tx_apr = 0.2;
	
	//criando o Jogador
	Jogador jogador;
	//Criando a bola
	Bola bola;
	
	preparar_valores_iniciais(&jogador, &bola);
	
	system("setterm -cursor off"); //Desligando o cursor
	initscr(); // Iniciando o modo curses
	WINDOW *telaJogo; //Definindo a tela do jogo
	telaJogo = newwin(LINHAS, COLUNAS, (int)(LINES/2 - LINHAS/2), 
					(int)(COLS/2 - COLUNAS/2)); //Tela do Jogo centralizada
					
	//Loop do Jogo
	while(1){
		//***************
		//Regras do Jogo
		//**************

		//FIM DE JOGO
		while(estadoJogo == 1){
			char opcao=' ';
			while(kbhit()) opcao=getchar();
			if(opcao == 'q'){
				fechar_jogo(telaJogo, rna1, dese1);
				return(0);
			}
			if(opcao == 'r')
				preparar_valores_iniciais(&jogador, &bola);
		}
	
		//Verificando se a bola bateu nas paredes
		if(estadoJogo == 0){
			if(round(bola.linha) < 1){
				bola.linha++;
				bola.direcao = 360-bola.direcao;
			}
			if(round(bola.linha) > LINHAS -2){
				bola.linha--;
				bola.direcao = 360-bola.direcao;
			}
			if(round(bola.coluna) < 1){
				bola.coluna++;
				bola.direcao = 360-bola.direcao-180;
			}
			if(round(bola.coluna) > COLUNAS-2){
				bola.coluna--;
				bola.direcao =  360-bola.direcao-180;
			}
			if(bola.direcao >= 360) 
				bola.direcao -=360;
			if(bola.direcao < 0)
				bola.direcao +=360;
			mvwprintw(stdscr, 0, 0, "%f", bola.direcao);
		}

		//Verificar se a bola bateu no jogador
		if(!rebateu)
			if(round(bola.linha) > LINHAS-3){
				if(round(bola.coluna) >  round(jogador.coluna-2) &&
			  	   round(bola.coluna) < round(jogador.coluna+4)){
			 	   bola.direcao = rand()%150 + 20 ;
					pontos++;
					rebateu = 1;
				}else{
					erros++;
					pontos = 0;
				}
			}

		//Testando se a RNA aprendeu
		if(pontos > 50){
			aprendizado = 0;
		}
		if(erros > 3){
			aprendizado = 1;
			erros = 0;
		}

		//Fazendo o jogador se movimentar
		if (intervalo_jogador + intervalo_j < clock()){
			intervalo_jogador = clock();
			//Entrando com a distância da bola ao jogador
			rna1->ent[0] = bola.coluna*0.1;
			rna1->ent[1] = jogador.coluna*0.1;
			if(round(bola.coluna) == jogador.coluna   ||
			   round(bola.coluna) == jogador.coluna+1 ||
			   round(bola.coluna) == jogador.coluna+2){
				rna1->ent[2] = 1;
				dese1[1] = 1;
			}
			else{
			    	rna1->ent[2] = 0;
				dese1[1] = 0;
			}
			//Ativando a saida da RNA: camada, tam entrada, tam saída
			ativar_rna(rna1, 1, 3, 2);
			//Movendo o jogador conforme a RNA
			if(round(rna1->sai[0]) == 1 && round(rna1->sai[1]) == 0 && 
				 jogador.coluna < COLUNAS -4) jogador.coluna ++; //Direita
			if(round(rna1->sai[0]) == 0 && round(rna1->sai[1]) == 0 && 
				 jogador.coluna > 2) jogador.coluna --; //Esquerda
		}

		//Fazendo a bola se movimentar
		if (intervalo_bola + intervalo < clock()){
			intervalo_bola = clock();
			if(estadoJogo == 0){
				rebateu = 0;
				bola.linha  -= sin(bola.direcao*M_PI/180);
				bola.coluna += cos(bola.direcao*M_PI/180);
			}
			/*******************
			 * Ajustes Neurais *
			 *******************/
			//Aprendendo com o a saída anterior
			if(round(bola.coluna) >  jogador.coluna+2) dese1[0] = 1;
			if(round(bola.coluna) <  jogador.coluna+2) dese1[0] = 0;
			//Aprendizado neural: camada, tam entrada, tam saída, tx aprendizado
			if(aprendizado){
				backprop_rna(rna1, 1, 3, 2, dese1, tx_apr);
			}
		
		}
			
		//***************
		//Design Gráfico
		//***************

		//Imprimindo informações da RNA
		mvwprintw(stdscr, 2, 0, "* RNA1 * ");
		mvwprintw(stdscr, 3, 0, "Ent: [ %f  %f %f ]  " ,rna1->ent[0], rna1->ent[1], rna1->ent[2]);
		mvwprintw(stdscr, 4, 0, "Sai: [ %f  %f ]   "   ,rna1->sai[0], rna1->sai[1]);
		mvwprintw(stdscr, 5, 0, "Dese:[ %f  %f ]   "   ,dese1[0], dese1[1]);
		mvwprintw(stdscr, 7, 0, "* PESOS *");
		mvwprintw(stdscr, 8, 0, "[ %f %f %f ]   ", rna1->cam[0].neu[0].peso[0], 
							   rna1->cam[0].neu[0].peso[1], 
							   rna1->cam[0].neu[0].peso[2]); 
		mvwprintw(stdscr, 9, 0, "[ %f %f %f ]   ", rna1->cam[0].neu[1].peso[0], 
							   rna1->cam[0].neu[1].peso[1], 
							   rna1->cam[0].neu[1].peso[2]); 
		mvwprintw(stdscr,11, 0, "tx Apr:[%f]  ", tx_apr);
		if(aprendizado)
			mvwprintw(stdscr,6, round(COLS/2)-10, "* RNA em Treinamento! *");
		else
			mvwprintw(stdscr,6, round(COLS/2)-10, "*    RNA em Teste!    *");
				
		//Limpando a Tela
		for(int lin = 0; lin < LINHAS; lin++)
			for(int col = 0; col < COLUNAS; col++)
			mvwprintw(telaJogo, lin, col, " ");
		
		//Imprimindo o Jogador
		mvwprintw(telaJogo, LINHAS-2, jogador.coluna, "%s", "---");
	
		//Imprimindo a Bola
		mvwprintw(telaJogo, round(bola.linha), round(bola.coluna), "%c", 'O');
			
		box(telaJogo, '|', '-'); //Bordas da Janela
		mvwprintw(telaJogo, 0, 14, "| *********  PONG  ********* |- Aprend.= %d", 
					pontos); //Título da Janela
					
		refresh();
		wrefresh(telaJogo);

		//*****************
		//Ações do usuário
		//*****************
		//evento = 0;
		if (kbhit()){ //Capturando a ação do usuário
			evento = getchar();
		}
		if(evento == ('q')){ //Encerrando o Jogo
			fechar_jogo(telaJogo, rna1, dese1);
			return(0);       //Saindo do programa
		}	
		evento = ' ';
	} //Fim do Loop do Jogo
					
}
