/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

 /******************************************************************************
  * This project provides two demo applications.  A simple blinky style project,
  * and a more comprehensive test and demo application.  The
  * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is used to select between the two.
  * The simply blinky demo is implemented and described in main_blinky.c.  The
  * more comprehensive test and demo application is implemented and described in
  * main_full.c.
  *
  * This file implements the code that is not demo specific, including the
  * hardware setup and FreeRTOS hook functions.
  *
  *******************************************************************************
  * NOTE: Windows will not be running the FreeRTOS demo threads continuously, so
  * do not expect to get real time behaviour from the FreeRTOS Windows port, or
  * this demo application.  Also, the timing information in the FreeRTOS+Trace
  * logs have no meaningful units.  See the documentation page for the Windows
  * port for further information:
  * http://www.freertos.org/FreeRTOS-Windows-Simulator-Emulator-for-Visual-Studio-and-Eclipse-MingW.html
  *

  *
  *******************************************************************************
  */

  /* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* This project provides two demo applications.  A simple blinky style demo
application, and a more comprehensive test and demo application.  The
mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is used to select between the two.

If mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is 1 then the blinky demo will be built.
The blinky demo is implemented and described in main_blinky.c.

If mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is not 1 then the comprehensive test and
demo application will be built.  The comprehensive test and demo application is
implemented and described in main_full.c. */
#define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY	1

/* This demo uses heap_5.c, and these constants define the sizes of the regions
that make up the total heap.  heap_5 is only used for test and example purposes
as this demo could easily create one large heap region instead of multiple
smaller heap regions - in which case heap_4.c would be the more appropriate
choice.  See http://www.freertos.org/a00111.html for an explanation. */
#define mainREGION_1_SIZE	7201
#define mainREGION_2_SIZE	29905
#define mainREGION_3_SIZE	6407

/*-----------------------------------------------------------*/

/*
 * main_blinky() is used when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 1.
 * main_full() is used when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 0.
 */
 //extern void main_blinky( void );
 //extern void main_full( void );

 /*
  * Only the comprehensive demo uses application hook (callback) functions.  See
  * http://www.freertos.org/a00016.html for more information.
  */
  //void vFullDemoTickHookFunction( void );
  //void vFullDemoIdleFunction( void );

  /*
   * This demo uses heap_5.c, so start by defining some heap regions.  It is not
   * necessary for this demo to use heap_5, as it could define one large heap
   * region.  Heap_5 is only used for test and example purposes.  See
   * http://www.freertos.org/a00111.html for an explanation.
   */
static void  prvInitialiseHeap(void);

/*
 * Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.  See http://www.freertos.org/a00016.html .
 */
void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char* pcTaskName);
void vApplicationTickHook(void);
void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer, uint32_t* pulIdleTaskStackSize);
void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer, StackType_t** ppxTimerTaskStackBuffer, uint32_t* pulTimerTaskStackSize);

/*
 * Writes trace data to a disk file when the trace recording is stopped.
 * This function will simply overwrite any trace files that already exist.
 */
static void prvSaveTraceFile(void);

/*-----------------------------------------------------------*/

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
use a callback function to optionally provide the memory required by the idle
and timer tasks.  This is the stack that will be used by the timer task.  It is
declared here, as a global, so it can be checked by a test that is implemented
in a different file. */
StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

/* Notes if the trace is running or not. */
static BaseType_t xTraceRunning = pdTRUE;

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*                                                              PROJETO 3 COMEÇA AQUI !!!!                                                                             */
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include <time.h>
#include "semphr.h"

//Pode mexer
#define T 15000             //Tempo de uma fase semafórica
#define CHEGADA_MIN 4000    // Tempo Mínimo de chaga dos carros no circuito
#define CHEGADA_MAX 8000    // Tempo Máximo de chaga dos carros no circuito

SemaphoreHandle_t xAcessoEstado;  // Mutex para acessar os estados dos semáforos
SemaphoreHandle_t xAcessoCruz[4]; // Mutexes para acessar os estados do cruzamento A-0, B-1, C-2 e D-3

int xEstadoSemaforos[4][4] = { {0} }; // 0 vermelho, 1 verde, 2 amarelo

SemaphoreHandle_t xSemaforos[4][4]; // Não mexer, pois é o que os carros irão pedir permissão ao longo do projeto
/*
			  Estrutura da matriz semáforos
		0       ,        1      ,     2     ,      3
0 xNS_Straight_A, xEW_Straight_A, xNS_Left_A, xEW_Left_A
1 xNS_Straight_B, xEW_Straight_B, xNS_Left_B, xEW_Left_B
2 xNS_Straight_C, xEW_Straight_C, xNS_Left_C, xEW_Left_C
3 xNS_Straight_D, xEW_Straight_D, xNS_Left_D, xEW_Left_D

*/

//Mede o quantidade de carros por vias
int FluxoVia[8] = { 0 };

/*---------------------- Funções Estéticas ----------------------*/
// Mostra mensagem de boas vindas
void BoasVindas(void)
{
	printf("\n .(___).	      Prezado professor Kyller,	   	        .(___).\n(o\\_!_/o)   Antes de iniciar, tenha conhecimento da legenda:   (o\\_!_/o)\n\n");

	printf("\n-------------------------------- LEGENDA --------------------------------\n");
	printf("|                             [>] Verde                                 |\n");
	printf("|                             [X] Vermelho                              |\n");
	printf("|                             [!] Amarelo                               |\n");
	printf("-------------------------------------------------------------------------\n");

	printf("\n\n .(___).	          STR 2024.1 PROJETO 3	   	        .(___).\n(o\\_!_/o)           Autores: Matheus, Tamara e Tulio           (o\\_!_/o)\n\n\n");
	system("pause");
	system("cls");
}

// Mostrar a Direção do carro em string
char* mostraDirecao(int sentido) {
	static char direcaoString[3]; // Variável estática pois permanece após o fim da função

	switch (sentido) {
	case 0:
		direcaoString[0] = 'N';
		direcaoString[1] = 'S';
		direcaoString[2] = '\0';
		break;
	case 1:
		direcaoString[0] = 'E';
		direcaoString[1] = 'W';
		direcaoString[2] = '\0';
		break;
	case 2:
		direcaoString[0] = 'S';
		direcaoString[1] = 'N';
		direcaoString[2] = '\0';
		break;
	case 3:
		direcaoString[0] = 'W';
		direcaoString[1] = 'E';
		direcaoString[2] = '\0';
		break;
	default:
		direcaoString[0] = 'X';
		direcaoString[1] = 'X';
		direcaoString[2] = '\0';
		break;
	}

	return direcaoString;
}

// Mostrar o Estado dos semáforos em string
char mostraEstado(int estado) {
	char estadoChar;

	switch (estado) {
	case 0:
		estadoChar = 'X';
		break;
	case 1:
		estadoChar = '>';
		break;
	case 2:
		estadoChar = '!';
		break;
	default:
		estadoChar = 'E';
		break;
	}

	return estadoChar;
}

// Mostrar o Cruzamento em string
char mostraCruzamento(int cruzamento) {
	char cruzamentoChar;
	switch (cruzamento) {
	case 0:
		cruzamentoChar = 'A';
		break;
	case 1:
		cruzamentoChar = 'B';
		break;
	case 2:
		cruzamentoChar = 'C';
		break;
	case 3:
		cruzamentoChar = 'D';
		break;
	default:
		cruzamentoChar = 'E';
		break;
	}

	return cruzamentoChar;
}

/*---------------------- Funções Auxiliares ----------------------*/
// Gerar número aleatório com base em variáveis iniciais e finais
int geraAleatorio(int inicial, int final) {
	srand(time(NULL)); // Seed peseudo aleatória
	return rand() % (final + 1) + inicial;
}

// Definir a rota com base em um ponto inicial aleatório de surgimento do carro
int* definirRota() {
	srand(time(NULL)); // Seed peseudo aleatória

	int pont_nascimento = geraAleatorio(0, 7); // Ponto de surgimento do carro [0 - 7] de acordo com a ilustração
	int i, N = 20;

	int direcoes[20]; // [0, 1, 2] -> [Direita, Esquerda, Frente]

	//Primeiro elemento é a posição de surgimento
	direcoes[0] = pont_nascimento;
	direcoes[1] = geraAleatorio(0, 2);

	//PAR
	if (pont_nascimento % 2 == 0) {
		if (direcoes[1] == 0) {
			direcoes[2] = -1;
		}
		else if (direcoes[1] == 1) {
			for (i = 2; i <= N; i++)
			{
				direcoes[i] = geraAleatorio(0, 2);
				if (direcoes[i] != 0) {
					direcoes[i + 1] = -1;
					break;
				}
			}
		}
		else {
			for (i = 2; i <= N; i++)
			{
				direcoes[i] = geraAleatorio(0, 2);
				if (direcoes[i] != 1) {
					direcoes[i + 1] = -1;
					break;
				}
			}
		}
	}

	//IMPAR
	else {
		if (direcoes[1] == 1) {
			direcoes[2] = -1;
		}
		else if (direcoes[1] == 0) {
			for (i = 2; i <= N; i++)
			{
				direcoes[i] = geraAleatorio(0, 2);
				if (direcoes[i] != 1) {
					direcoes[i + 1] = -1;
					break;
				}
			}
		}
		else {
			for (i = 2; i <= N; i++)
			{
				direcoes[i] = geraAleatorio(0, 2);
				if (direcoes[i] != 0) {
					direcoes[i + 1] = -1;
					break;
				}
			}
		}
	}

	return direcoes;
}
/*---------------------- As Tasks começam aqui ----------------------*/

// Exibir os estados de todos os semáforos 
void MonitoramentoGeral() {
	while (1) {
		// Acessando os estados
		xSemaphoreTake(xAcessoEstado, portMAX_DELAY);

		printf("\n ---- CRUZAMENTO A --------- CRUZAMENTO B --------- CRUZAMENTO C --------- CRUZAMENTO D ------\n");
		printf(" NS-frente_direita [%c] | NS-frente_direita [%c] | NS-frente_direita [%c] | NS-frente_direita [%c]\n", mostraEstado(xEstadoSemaforos[0][0]), mostraEstado(xEstadoSemaforos[1][0]), mostraEstado(xEstadoSemaforos[2][0]), mostraEstado(xEstadoSemaforos[3][0]));
		printf(" EW-frente_direita [%c] | EW-frente_direita [%c] | EW-frente_direita [%c] | EW-frente_direita [%c]\n", mostraEstado(xEstadoSemaforos[0][1]), mostraEstado(xEstadoSemaforos[1][1]), mostraEstado(xEstadoSemaforos[2][1]), mostraEstado(xEstadoSemaforos[3][1]));
		printf(" NS-esquerda       [%c] | NS-esquerda       [%c] | NS-esquerda       [%c] | NS-esquerda       [%c]\n", mostraEstado(xEstadoSemaforos[0][2]), mostraEstado(xEstadoSemaforos[1][2]), mostraEstado(xEstadoSemaforos[2][2]), mostraEstado(xEstadoSemaforos[3][2]));
		printf(" EW-esquerda       [%c] | EW-esquerda       [%c] | EW-esquerda       [%c] | EW-esquerda       [%c]\n", mostraEstado(xEstadoSemaforos[0][3]), mostraEstado(xEstadoSemaforos[1][3]), mostraEstado(xEstadoSemaforos[2][3]), mostraEstado(xEstadoSemaforos[3][3]));
		printf("-----------------------|-----------------------|-----------------------|----------------------\n");
		printf("   Via AB ||  Via BA   |   Via BD ||  Via DB   |   Via CA ||  Via AC   |   Via DC ||  Via CD   \n");
		printf("    [%d]   ||   [%d]     |    [%d]   ||   [%d]     |     [%d]  ||   [%d]     |     [%d]  ||   [%d]     \n", FluxoVia[4], FluxoVia[0], FluxoVia[5], FluxoVia[1], FluxoVia[7], FluxoVia[3], FluxoVia[6], FluxoVia[2]);
		printf("----------------------------------------------------------------------------------------------\n\n");

		// Liberando os estados
		xSemaphoreGive(xAcessoEstado);

		vTaskDelay(pdMS_TO_TICKS(T));
		//clear_terminal();
	}
}

// Controlar os cruzamentos (A, B, C e D)
void Cruzamento(void* pvParameters) {

	//ID do cruzamento
	int id = (int)pvParameters;

	// Pedindo acessando para mudar os estados
	xSemaphoreTake(xAcessoEstado, portMAX_DELAY);

	// Acessando o estado dos semáforos com base no id
	xSemaphoreGive(xSemaforos[id][1]); xEstadoSemaforos[id][1] = 1;
	xSemaphoreGive(xSemaforos[id][2]); xEstadoSemaforos[id][2] = 1;

	// Liberando os estados após acessar e alterar o estado
	xSemaphoreGive(xAcessoEstado);

	// Não mexer
	while (1) {

		/*------------------------------------ FASE 1 ------------------------------------*/

		// Pedindo acessando para mudar os estados
		xSemaphoreTake(xAcessoEstado, portMAX_DELAY);

		// Fechando o estado dos dois semáforos com base no id
		xSemaphoreTake(xSemaforos[id][1], portMAX_DELAY); xEstadoSemaforos[id][1] = 0; // Fecha Via Leste-Oeste frente-direita
		xSemaphoreTake(xSemaforos[id][2], portMAX_DELAY); xEstadoSemaforos[id][2] = 0; // Fecha Via Norte-Sul esquerda

		// Abrindo o estado dos dois semáforos com base no id
		xSemaphoreGive(xSemaforos[id][0]); xEstadoSemaforos[id][0] = 1; //Abre Via Norte-Sul frente-direita
		xSemaphoreGive(xSemaforos[id][3]); xEstadoSemaforos[id][3] = 1; //Abre Via Leste-Oeste esquerda

		// Liberando os estados após acessar e alterar o estado
		xSemaphoreGive(xAcessoEstado);

		vTaskDelay(pdMS_TO_TICKS(T));

		/*------------------------------------ FASE 2 ------------------------------------*/

		// Pedindo acessando para mudar os estados
		xSemaphoreTake(xAcessoEstado, portMAX_DELAY);

		// Alterando o estado para ter a situação do semáforo em amarelo
		xEstadoSemaforos[id][0] = 2;
		xEstadoSemaforos[id][3] = 2;

		// Liberando os estados após acessar e alterar o estado
		xSemaphoreGive(xAcessoEstado);

		vTaskDelay(pdMS_TO_TICKS(T));

		/*------------------------------------ FASE 3 ------------------------------------*/

		// Pedindo acessando para mudar os estados
		xSemaphoreTake(xAcessoEstado, portMAX_DELAY);

		// Fechando o estado dos dois semáforos com base no id
		xSemaphoreTake(xSemaforos[id][0], portMAX_DELAY); xEstadoSemaforos[id][0] = 0; // Fecha Via Norte-Sul frente-direita
		xSemaphoreTake(xSemaforos[id][3], portMAX_DELAY); xEstadoSemaforos[id][3] = 0; // Fecha Via Leste-Oeste esquerda

		// Abrindo o estado dos dois semáforos com base no id
		xSemaphoreGive(xSemaforos[id][1]); xEstadoSemaforos[id][1] = 1;//Abre Via Leste-Oeste frente-direita
		xSemaphoreGive(xSemaforos[id][2]); xEstadoSemaforos[id][2] = 1;//Abre Via Norte-Sul esquerda

		// Liberando os estados após acessar e alterar o estado
		xSemaphoreGive(xAcessoEstado);

		vTaskDelay(pdMS_TO_TICKS(T));

		/*------------------------------------ FASE 4 ------------------------------------*/

		// Pedindo acessando para mudar os estados
		xSemaphoreTake(xAcessoEstado, portMAX_DELAY);

		// Alterando o estado para ter a situação do semáforo em amarelo
		xEstadoSemaforos[id][1] = 2;
		xEstadoSemaforos[id][2] = 2;

		// Liberando os estados após acessar e alterar o estado
		xSemaphoreGive(xAcessoEstado);

		vTaskDelay(pdMS_TO_TICKS(T));
	}
}

// Controla os carros
void Carro(void* pvParameters) {

	//ID do cruzamento
	int id = 1 + (int)pvParameters;

	int i = 0, N = 20;

	int sentidoAtual;    // 0 = NS,  1= EW, 2 = SN, 3 = WE;
	int cruzamentoAtual; // 0 = A,  1= B, 2 = C, 3 = D;
	int velocidade = 50;   //km/h
	float distancia = 0.5; //km

	int* rota = definirRota();

	//Define o sentido e cruzamento inicial
	switch (rota[0])
	{
	case 0:
		sentidoAtual = 0;    // NS
		cruzamentoAtual = 0; // A
		break;
	case 1:
		sentidoAtual = 0;    // NS
		cruzamentoAtual = 1; // B
		break;
	case 2:
		sentidoAtual = 1;    // EW
		cruzamentoAtual = 1; // B
		break;
	case 3:
		sentidoAtual = 1;    // EW
		cruzamentoAtual = 3; // D
		break;
	case 4:
		sentidoAtual = 2;    // SN
		cruzamentoAtual = 3; // D
		break;
	case 5:
		sentidoAtual = 2;    // SN
		cruzamentoAtual = 2; // C
		break;
	case 6:
		sentidoAtual = 3;    // WE
		cruzamentoAtual = 2; // C
		break;
	case 7:
		sentidoAtual = 3;    // WE
		cruzamentoAtual = 0; // A
		break;
	default:
		sentidoAtual = 0;    // NS
		cruzamentoAtual = 0; // A
		break;
	}

	printf("O carro %d surgiu na posicao %d\n", id, rota[0]);

	while (1) {
		i++;
		if (rota[i] == -1) { break; } //Condição de parada

		//Define velociade
		if (sentidoAtual == 0 || sentidoAtual == 2) {      //NS ou SN
			velocidade = 60 + geraAleatorio(0, 10) - 5;
		}
		else if (sentidoAtual == 1 || sentidoAtual == 3) { // EW ou WE
			velocidade = 50 + geraAleatorio(0, 10) - 5;;
		}

		printf("O carro %d esta no sentido %s a %d km/h\n", id, mostraDirecao(sentidoAtual), velocidade);

		//Espera chegar no cruzamento
		vTaskDelay(pdMS_TO_TICKS((distancia / velocidade) * 3600 * 500));
		printf("O carro %d chegou no cruzamento %c!\n", id, mostraCruzamento(cruzamentoAtual));

		//Esperar semáforo com base no sentido
		if (sentidoAtual == 0 || sentidoAtual == 2) {//Direção NS e SN
			switch (rota[i]) {
			case 0://Direita
				printf("O carro %d quer virar a direita\n", id);
				xSemaphoreTake(xSemaforos[cruzamentoAtual][0], portMAX_DELAY);

				xSemaphoreTake(xAcessoCruz[cruzamentoAtual], portMAX_DELAY);
				xSemaphoreGive(xAcessoCruz[cruzamentoAtual]);

				xSemaphoreGive(xSemaforos[cruzamentoAtual][0]);

				break;
			case 1://Esquerda
				printf("O carro %d quer virar a esquerda\n", id);
				xSemaphoreTake(xSemaforos[cruzamentoAtual][2], portMAX_DELAY);

				xSemaphoreTake(xAcessoCruz[cruzamentoAtual], portMAX_DELAY);
				xSemaphoreGive(xAcessoCruz[cruzamentoAtual]);

				xSemaphoreGive(xSemaforos[cruzamentoAtual][2]);
				break;
			case 2://Frente
				printf("O carro %d quer seguir em frente\n", id);
				xSemaphoreTake(xSemaforos[cruzamentoAtual][0], portMAX_DELAY);

				xSemaphoreTake(xAcessoCruz[cruzamentoAtual], portMAX_DELAY);
				xSemaphoreGive(xAcessoCruz[cruzamentoAtual]);

				xSemaphoreGive(xSemaforos[cruzamentoAtual][0]);
				break;
			default:
				printf("ERRO!!!!\n\n");
				break;
			}
		}
		else if (sentidoAtual == 1 || sentidoAtual == 3) { //Direção EW e WE
			switch (rota[i]) {
			case 0://Direita
				printf("O carro %d quer virar a direita\n", id);
				xSemaphoreTake(xSemaforos[cruzamentoAtual][1], portMAX_DELAY);

				xSemaphoreTake(xAcessoCruz[cruzamentoAtual], portMAX_DELAY);
				xSemaphoreGive(xAcessoCruz[cruzamentoAtual]);

				xSemaphoreGive(xSemaforos[cruzamentoAtual][1]);
				break;
			case 1://Esquerda
				printf("O carro %d quer virar a esquerda\n", id);
				xSemaphoreTake(xSemaforos[cruzamentoAtual][3], portMAX_DELAY);

				xSemaphoreTake(xAcessoCruz[cruzamentoAtual], portMAX_DELAY);
				xSemaphoreGive(xAcessoCruz[cruzamentoAtual]);

				xSemaphoreGive(xSemaforos[cruzamentoAtual][3]);
				break;
			case 2://Frente
				printf("O carro %d quer seguir em frente\n", id);
				xSemaphoreTake(xSemaforos[cruzamentoAtual][1], portMAX_DELAY);

				xSemaphoreTake(xAcessoCruz[cruzamentoAtual], portMAX_DELAY);
				xSemaphoreGive(xAcessoCruz[cruzamentoAtual]);

				xSemaphoreGive(xSemaforos[cruzamentoAtual][1]);
				break;
			default:
				printf("ERRO!!!!\n\n");
				break;
			}
		}
		else { printf("ERRO!!!!"); }

		printf("O carro %d seguiu\n", id);

		//Muda sentido
		if (rota[i] == 0) { // Direita
			switch (sentidoAtual) {
			case 0:
				sentidoAtual = 1; // EW
				break;
			case 1:
				sentidoAtual = 2; // SN
				break;
			case 2:
				sentidoAtual = 3; // WE
				break;
			case 3:
				sentidoAtual = 0; // NS
				break;
			default:
				sentidoAtual = sentidoAtual;
				break;
			}
		}
		else if (rota[i] == 1) { // Esquerda
			switch (sentidoAtual) {
			case 0:
				sentidoAtual = 3; // WE
				break;
			case 1:
				sentidoAtual = 0; // NS
				break;
			case 2:
				sentidoAtual = 1; // EW
				break;
			case 3:
				sentidoAtual = 2; // SN
				break;
			default:
				sentidoAtual = sentidoAtual;
				break;
			}
		}
		else { sentidoAtual = sentidoAtual; }

		//Muda cruzamento
		if (sentidoAtual == 0) { // NS
			switch (cruzamentoAtual) {
			case 0:
				cruzamentoAtual = 2; // C
				FluxoVia[3]++;
				break;
			case 1:
				cruzamentoAtual = 3; // D
				FluxoVia[5]++;
				break;
			default:
				cruzamentoAtual = cruzamentoAtual;
				break;
			}
		}
		else if (sentidoAtual == 1) { // EW
			switch (cruzamentoAtual) {
			case 1:
				cruzamentoAtual = 0; // A
				FluxoVia[0]++;
				break;
			case 3:
				cruzamentoAtual = 2; // C
				FluxoVia[6]++;
				break;
			default:
				cruzamentoAtual = cruzamentoAtual;
				break;
			}
		}
		else if (sentidoAtual == 2) { // SN
			switch (cruzamentoAtual) {
			case 2:
				cruzamentoAtual = 0; // A
				FluxoVia[7]++;
				break;
			case 3:
				cruzamentoAtual = 1; // B
				FluxoVia[1]++;
				break;
			default:
				cruzamentoAtual = cruzamentoAtual;
				break;
			}
		}
		else { // sentidoAtual = 3 -> WE
			switch (cruzamentoAtual) {
			case 0:
				cruzamentoAtual = 1; // B
				FluxoVia[4]++;
				break;
			case 2:
				cruzamentoAtual = 3; // D
				FluxoVia[2]++;
				break;
			default:
				cruzamentoAtual = cruzamentoAtual;
				break;
			}
		}
	}

	printf("O carro %d saiu do circuito!\n", id);

	// A tarefa se autodeleta
	vTaskDelete(NULL);
}

// Cria carros indefinidamente
void criarCarros() {
	xTaskHandle Car = NULL;
	int i = 0;

	while (1) {
		vTaskDelay(pdMS_TO_TICKS(geraAleatorio(CHEGADA_MIN, CHEGADA_MAX)));
		xTaskCreate(Carro, (signed char*)"criarCarros", configMINIMAL_STACK_SIZE, (void*)i, 1, &Car);
		i++;
	}
}

int main(void)
{
	/* This demo uses heap_5.c, so start by defining some heap regions.  heap_5
	is only used for test and example reasons.  Heap_4 is more appropriate.  See
	http://www.freertos.org/a00111.html for an explanation. */
	prvInitialiseHeap();
	/* Initialise the trace recorder.  Use of the trace recorder is optional.
	See http://www.FreeRTOS.org/trace for more information. */
	vTraceEnable(TRC_START);

	int i, j;

	//Comente se não quizer apreciar nosso trabalho
	//BoasVindas();

	//Inicializa o mutex dos estados
	xAcessoEstado = xSemaphoreCreateMutex();
	xSemaphoreGive(xAcessoEstado);

	//Inicializa o mutex dos cruzamentos
	for (int j = 0; j < 4; j++) {
		xAcessoCruz[j] = xSemaphoreCreateMutex();
		xSemaphoreGive(xAcessoCruz[j]);
	}

	//Inicializa os semaforos binários de cada cruzamento
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			xSemaforos[i][j] = xSemaphoreCreateBinary();
		}
	}

	//Crias os Handles das tasks
	xTaskHandle Cruz_A, Cruz_B, Cruz_C, Cruz_D;
	xTaskHandle MonGeral, CriaCar;

	// Cria as Tasks
	xTaskCreate(MonitoramentoGeral, (signed char*)"Monitoramento_Geral", configMINIMAL_STACK_SIZE, (void*)NULL, 4, &MonGeral);
	xTaskCreate(Cruzamento, (signed char*)"Cruzamento_A", configMINIMAL_STACK_SIZE, (void*)0, 5, &Cruz_A);
	xTaskCreate(Cruzamento, (signed char*)"Cruzamento_B", configMINIMAL_STACK_SIZE, (void*)1, 5, &Cruz_B);
	xTaskCreate(Cruzamento, (signed char*)"Cruzamento_C", configMINIMAL_STACK_SIZE, (void*)2, 5, &Cruz_C);
	xTaskCreate(Cruzamento, (signed char*)"Cruzamento_D", configMINIMAL_STACK_SIZE, (void*)3, 5, &Cruz_D);
	xTaskCreate(criarCarros, (signed char*)"Criar_Carros", configMINIMAL_STACK_SIZE, (void*)NULL, 1, &CriaCar);

	vTaskStartScheduler();
	for (;;);
	return 0;
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook(void)
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c, heap_2.c or heap_4.c is being used, then the
	size of the	heap available to pvPortMalloc() is defined by
	configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the xPortGetFreeHeapSize()
	API function can be used to query the size of free heap space that remains
	(although it does not provide information on how the remaining heap might be
	fragmented).  See http://www.freertos.org/a00111.html for more
	information. */
	vAssertCalled(__LINE__, __FILE__);
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook(void)
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If application tasks make use of the
	vTaskDelete() API function to delete themselves then it is also important
	that vApplicationIdleHook() is permitted to return to its calling function,
	because it is the responsibility of the idle task to clean up memory
	allocated by the kernel to any task that has since deleted itself. */

	/* Uncomment the following code to allow the trace to be stopped with any
	key press.  The code is commented out by default as the kbhit() function
	interferes with the run time behaviour. */
	/*
		if( _kbhit() != pdFALSE )
		{
			if( xTraceRunning == pdTRUE )
			{
				vTraceStop();
				prvSaveTraceFile();
				xTraceRunning = pdFALSE;
			}
		}
	*/

#if ( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1 )
	{
		/* Call the idle task processing used by the full demo.  The simple
		blinky demo does not use the idle task hook. */
		vFullDemoIdleFunction();
	}
#endif
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char* pcTaskName)
{
	(void)pcTaskName;
	(void)pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.  This function is
	provided as an example only as stack overflow checking does not function
	when running the FreeRTOS Windows port. */
	vAssertCalled(__LINE__, __FILE__);
}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void)
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
#if ( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1 )
	{
		vFullDemoTickHookFunction();
	}
#endif /* mainCREATE_SIMPLE_BLINKY_DEMO_ONLY */
}
/*-----------------------------------------------------------*/

void vApplicationDaemonTaskStartupHook(void)
{
	/* This function will be called once only, when the daemon task starts to
	execute	(sometimes called the timer task).  This is useful if the
	application includes initialisation code that would benefit from executing
	after the scheduler has been started. */
}
/*-----------------------------------------------------------*/

void vAssertCalled(unsigned long ulLine, const char* const pcFileName)
{
	static BaseType_t xPrinted = pdFALSE;
	volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

	/* Called if an assertion passed to configASSERT() fails.  See
	http://www.freertos.org/a00110.html#configASSERT for more information. */

	/* Parameters are not used. */
	(void)ulLine;
	(void)pcFileName;

	printf("ASSERT! Line %ld, file %s, GetLastError() %ld\r\n", ulLine, pcFileName, GetLastError());

	taskENTER_CRITICAL();
	{
		/* Stop the trace recording. */
		if (xPrinted == pdFALSE)
		{
			xPrinted = pdTRUE;
			if (xTraceRunning == pdTRUE)
			{
				vTraceStop();
				prvSaveTraceFile();
			}
		}

		/* You can step out of this function to debug the assertion by using
		the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
		value. */
		while (ulSetToNonZeroInDebuggerToContinue == 0)
		{
			__asm { NOP };
			__asm { NOP };
		}
	}
	taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

static void prvSaveTraceFile(void)
{
	FILE* pxOutputFile;

	fopen_s(&pxOutputFile, "Trace.dump", "wb");

	if (pxOutputFile != NULL)
	{
		fwrite(RecorderDataPtr, sizeof(RecorderDataType), 1, pxOutputFile);
		fclose(pxOutputFile);
		printf("\r\nTrace output saved to Trace.dump\r\n");
	}
	else
	{
		printf("\r\nFailed to create trace dump file\r\n");
	}
}
/*-----------------------------------------------------------*/

static void  prvInitialiseHeap(void)
{
	/* The Windows demo could create one large heap region, in which case it would
	be appropriate to use heap_4.  However, purely for demonstration purposes,
	heap_5 is used instead, so start by defining some heap regions.  No
	initialisation is required when any other heap implementation is used.  See
	http://www.freertos.org/a00111.html for more information.

	The xHeapRegions structure requires the regions to be defined in start address
	order, so this just creates one big array, then populates the structure with
	offsets into the array - with gaps in between and messy alignment just for test
	purposes. */
	static uint8_t ucHeap[configTOTAL_HEAP_SIZE];
	volatile uint32_t ulAdditionalOffset = 19; /* Just to prevent 'condition is always true' warnings in configASSERT(). */
	const HeapRegion_t xHeapRegions[] =
	{
		/* Start address with dummy offsets						Size */
		{ ucHeap + 1,											mainREGION_1_SIZE },
		{ ucHeap + 15 + mainREGION_1_SIZE,						mainREGION_2_SIZE },
		{ ucHeap + 19 + mainREGION_1_SIZE + mainREGION_2_SIZE,	mainREGION_3_SIZE },
		{ NULL, 0 }
	};

	/* Sanity check that the sizes and offsets defined actually fit into the
	array. */
	configASSERT((ulAdditionalOffset + mainREGION_1_SIZE + mainREGION_2_SIZE + mainREGION_3_SIZE) < configTOTAL_HEAP_SIZE);

	/* Prevent compiler warnings when configASSERT() is not defined. */
	(void)ulAdditionalOffset;

	vPortDefineHeapRegions(xHeapRegions);
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer, uint32_t* pulIdleTaskStackSize)
{
	/* If the buffers to be provided to the Idle task are declared inside this
	function then they must be declared static - otherwise they will be allocated on
	the stack and so not exists after this function exits. */
	static StaticTask_t xIdleTaskTCB;
	static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

	/* Pass out a pointer to the StaticTask_t structure in which the Idle task's
	state will be stored. */
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	/* Pass out the array that will be used as the Idle task's stack. */
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	/* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer, StackType_t** ppxTimerTaskStackBuffer, uint32_t* pulTimerTaskStackSize)
{
	/* If the buffers to be provided to the Timer task are declared inside this
	function then they must be declared static - otherwise they will be allocated on
	the stack and so not exists after this function exits. */
	static StaticTask_t xTimerTaskTCB;

	/* Pass out a pointer to the StaticTask_t structure in which the Timer
	task's state will be stored. */
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	/* Pass out the array that will be used as the Timer task's stack. */
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	/* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}