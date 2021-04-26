# RNA---PONG
Jogo Pong  RNA - Jogo onde uma RNA Perceptron aprende a rebater uma bola.
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
