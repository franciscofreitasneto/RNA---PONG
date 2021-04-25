/*******************************************************************************************************************************************************
 * Estudo de RNA - PERCEPTRON - Clássica
 *
 * Exercício 04 - Sigmóide, Várias conexões de saída e Multiplas Camadas Ocultas
 * Desenvolvimento de uma pequena biblioteca de RNA Perceptron para uso acadêmico.
 *
 * Interface de utilização (Funções):
 *
 * Como utilizar a biblioteca no código fonte:
 * #include "perceptron02_sig_v02.c"
 * 
 * Como Compilar:
 * gcc -o [arquivo_de_saida] [arquivo_fonte] -lm
 *
 * Rna *rna = cria_rna(Quantidade de Camadas, Nº de Conexões de Entrada, Nº de Conexões de Saída);
 * Esta função retorna um ponteiro para a estrutura neural passada como parâmetro
 *
 * int sucesso = ativar_rna(*rna, Quantidade de Camadas, Nº de Conexões de Entrada, Nº de COnexões de Saída);
 * Função ativa a saída da (Rna *rna), passada por parâmetro, ativando suas saídas por uma função sigmóide
 *
 * int sucesso = backprop_rna(*rna, Quantidade de Camadas, Nº de Conexões de Entrada, Nº de Conexões de Saída, 
 * 			      *vetor de aprendizagem, taxa de aprendizagem);
 * Função responsável por corrigir os pesos neurais da *rna, passada por parâmetro, pelo método de Backpropagation.
 * O ponterio "*vetor de aprendizagem", é um vetor do tipo "float", passado como parâmetro do tamanho da saída da RNA
 * para comparar os resultados esperados (Método supervisionado de aprendizagem).
 *
 * Com estas 3 funções é possível criar, treinar e utilizar uma RNA Perceptron com um modelo clássico de Aprendizado 
 * Superviosionado.  :)
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
 * Prof. Francisco Freitas
 ****************************************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Tipos de Camadas
#define SAIDA 1
#define OCULTO 0
#define RANDOMICO 1

//float *desejado;

//Estruturas Básicas de um nurônio
typedef struct neuronio{
	float *ent;
	float *peso;
	float bias;
	float *sai;
}Neuronio;

//Uma camada é um agragador de neurônios
//e pode ser de SAIDA ou OCULTO
typedef struct camada{
	float *ent;
	float *sai;
	Neuronio *neu;
	int tipo; 
}Camada;

//Uma rna é um agregador de camadas
typedef struct rna{
	float *ent;
	float *sai;
	Camada *cam;
}Rna;

//A função de ativação desta RNA será a sigmôide
float sigmoid(float net){
	float lamb = -2;
	return 1/(1 + exp(lamb*net));
}
float sigmoid_derivada(float net){
	return sigmoid(net) * (1-sigmoid(net));
}
//BackPropagation no Bias de cada neurônio
float backprop_bias(Rna *rna, int cam, int neu, int qtd_ent, int qtd_sai, float *desejado){
	if(rna->cam[cam].tipo == SAIDA){
		return (desejado[neu] - rna->sai[neu]) * rna->sai[neu] *
			(1 - rna->sai[neu]);
	}else{
		float soma_delta = 0.0;
		int tam_camada;
		if(rna->cam[cam+1].tipo == SAIDA) tam_camada = qtd_sai;
		else tam_camada = qtd_ent;
		for(int n = 0; n < tam_camada; n++)
			soma_delta += (backprop_bias(rna, cam+1, n, qtd_ent, qtd_sai, desejado) * rna->cam[cam+1].neu[n].bias);

		return *rna->cam[cam].neu[neu].sai * (1 - *rna->cam[cam].neu[neu].sai) * soma_delta;
	}
}
//BackPropagation em cada neurônio
float backprop_neu(Rna *rna, int cam, int neu, int qtd_ent, int qtd_sai ,float *desejado){
	if(rna->cam[cam].tipo == SAIDA){
		return (desejado[neu] - rna->sai[neu]) * rna->sai[neu] *
			(1 - rna->sai[neu]);
	}else{
		float soma_delta = 0.0;
		int tam_camada;
		if(rna->cam[cam+1].tipo == SAIDA) tam_camada = qtd_sai;
		else tam_camada = qtd_ent;
		for(int n = 0; n < tam_camada; n++)
			soma_delta += (backprop_neu(rna, cam+1, n, qtd_ent, qtd_sai, desejado) * rna->cam[cam+1].neu[n].peso[neu]);

		return *rna->cam[cam].neu[neu].sai * (1 - *rna->cam[cam].neu[neu].sai) * soma_delta;
	}
}

//BackPropagation na camada chamando a backprop dos neurônios
int backprop_cam(Rna *rna, int cam, int qtd_ent, int qtd_sai, float *desejado, float tx_apr){
	int tam_cam;
	if (rna->cam[cam].tipo == SAIDA) tam_cam = qtd_sai;
	else tam_cam = qtd_ent;
	for(int neu = 0; neu < tam_cam; neu++){
		rna->cam[cam].neu[neu].bias += tx_apr * backprop_bias(rna, cam, neu, qtd_ent, qtd_sai, desejado);
		for(int peso = 0; peso < qtd_ent; peso++){
			rna->cam[cam].neu[neu].peso[peso] += tx_apr * backprop_neu(rna, cam, neu, qtd_ent, qtd_sai, desejado) * 
								rna->cam[cam].neu[neu].ent[peso];
		}
	}
	return 0;
}

//BackPropagation na RNA chamando a backprop das camadas
int backprop_rna(Rna *rna, int qtd_cam, int qtd_ent, int qtd_sai, float *desejado, float tx_apr){
	for(int cam = 0; cam < qtd_cam; cam++)
		backprop_cam(rna, cam, qtd_ent, qtd_sai, desejado, tx_apr);
	return 0;
}

//Ativação de um neurônio
int ativar_neuronio(Neuronio *neu, int qtd_ent){
	float net = 0;
	for(int ent = 0; ent < qtd_ent; ent++)
		net += neu->ent[ent] * neu->peso[ent];
	net += neu->bias;
	*neu->sai = sigmoid(net);
	//printf("%f ",*neu->sai);
	return 0;
}

//Ativação de uma camada
int ativar_camada(Camada *cam, int qtd_ent, int qtd_sai){
	for(int neu = 0; neu < qtd_sai; neu++)
		ativar_neuronio(&cam->neu[neu], qtd_ent);
	return 0;
}

//Ativação da RNA
int ativar_rna(Rna *rna, int qtd_cam, int qtd_ent, int qtd_sai){
	for(int cam = 0; cam < qtd_cam; cam++){
		if(cam < qtd_cam-1)
			ativar_camada(&rna->cam[cam], qtd_ent, qtd_ent);
		else
			ativar_camada(&rna->cam[cam], qtd_ent, qtd_sai);
	}
	return 0;
}

//Para criar um neurônio devemos decidir se os pesos de
//conexões serão aleatôrio ou não.
int criar_neuronio(Neuronio *neu, int qtd_ent,  int rando){
	neu->peso = (float *) malloc(qtd_ent*sizeof(float));
	if(rando){
		for(int ent = 0; ent < qtd_ent; ent++)
			neu->peso[ent] = (rand()%100)/100.0;
		neu->bias = (rand()%100)/100.0;
	}else{
		for(int ent = 0; ent < qtd_ent; ent++)
			neu->peso[ent] = 0.5;
		neu->bias = 0.5;
	}
	return 0;
}

int criar_camada(Camada *cam, float *sai_ant,int qtd_ent, int qtd_sai, int tipo){
	cam->ent    = sai_ant;
	cam->sai    = (float *) malloc(qtd_ent*sizeof(float));
	cam->tipo   = tipo;
	//Criando o agrupamento de Neurônios
	cam->neu = (Neuronio *) malloc(qtd_sai*sizeof(Neuronio));
	for(int neu = 0; neu < qtd_sai; neu++){
		criar_neuronio(&cam->neu[neu], qtd_ent, RANDOMICO);
		cam->neu[neu].ent = cam->ent; //As entradas do neurônio estão ligadas à camada
		cam->neu[neu].sai = &cam->sai[neu]; //A saída do neurônio está ligada à camada
	}
	return 0;
}

Rna *criar_rna(int qtd_cam, int qtd_ent, int qtd_sai){
	srand(time(NULL));
	Rna *rna = (Rna *)    malloc(sizeof(Rna));
	rna->cam = (Camada *) malloc(qtd_cam*sizeof(Camada));
	rna->ent = (float *)  malloc(qtd_ent*sizeof(float));
	for (int cam = 0; cam < qtd_cam; cam++){
		if(cam < qtd_cam-1){
			if(cam == 0)
				criar_camada(&rna->cam[cam], rna->ent, qtd_ent, qtd_ent, OCULTO);
			else
				criar_camada(&rna->cam[cam], rna->cam[cam-1].sai, qtd_ent, qtd_ent, OCULTO);
		}
		else
			if(cam == 0)
				criar_camada(&rna->cam[cam], rna->ent, qtd_ent, qtd_ent, SAIDA);
			else
				criar_camada(&rna->cam[cam], rna->cam[cam-1].sai, qtd_ent, qtd_ent, SAIDA);
	}
	//rna->cam[0].ent = rna->ent;         //As entradas da RNA estão ligadas à 1ª Camada
	rna->sai = rna->cam[qtd_cam-1].sai; //As saídas da RNA estão ligadas à última camada
	return rna;
}

int imprimir_rna(Rna *rna, int qtd_cam, int qtd_ent, int qtd_sai){
	printf("\n");
	printf("Entrada da RNA:\n");
	for(int ent = 0; ent < qtd_ent; ent++)
		printf("%4.2f ", rna->ent[ent]);
	for(int cam = 0; cam < qtd_cam; cam++){
		printf("\nCAMADA %d:\n",cam);
		printf("Entrada: \n");
		for(int ent = 0; ent < qtd_ent; ent++)
			printf("%4.2f ", rna->cam[cam].ent[ent]);
		printf("\nSaida: \n");
		for(int sai = 0; sai < qtd_ent; sai++)
			printf("%4.2f ", rna->cam[cam].sai[sai]);
			
	}
	printf("\n");
	return 0;	
}
