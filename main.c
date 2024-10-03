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

#define T 2000 //Tempo total de um ciclo semaf�rico [ms]

#include <time.h>
#include "semphr.h"

SemaphoreHandle_t xNS_Straight, xEW_Straight, xNS_Left, xEW_Left;

int geraAleatorio(int inicial, int final) {
	srand(time(NULL)); // Seed peseudo aleat�ria
	return rand() % (final + 1) + inicial;
}

void Cruzamento() {
	xSemaphoreGive(xEW_Straight);
	xSemaphoreGive(xNS_Left);

	//N�o mexer
	while (1) {
		
		// Fechando
		xSemaphoreTake(xEW_Straight, portMAX_DELAY); // Fecha Via Leste-Oeste frente-direita
		xSemaphoreTake(xNS_Left, portMAX_DELAY);     // Fecha Via Norte-Sul esquerda
		
		// Abrindo
		xSemaphoreGive(xNS_Straight); //Abre Via Norte-Sul frente-direita
		xSemaphoreGive(xEW_Left);     //Abre Via Leste-Oeste esquerda

		printf(" -------------------------------------------\n");
		printf(" # EW-frente_direita | > NS-frente_direita  \n");
		printf(" # NS-esquerda       | > EW-esquerda        \n");
		printf(" -------------------------------------------\n");

		vTaskDelay(pdMS_TO_TICKS(T)); // Vermelho / Verde

		printf(" -------------------------------------------\n");
		printf(" # EW-frente_direita | * EW-esquerda        \n");
		printf(" # NS-esquerda       | * NS-frente_direita  \n");
		printf(" -------------------------------------------\n");

		vTaskDelay(pdMS_TO_TICKS(T/4)); // Vermelho / Amarelo 

		// Fechando
		xSemaphoreTake(xNS_Straight, portMAX_DELAY);	// Fecha Via Norte-Sul frente-direita
		xSemaphoreTake(xEW_Left, portMAX_DELAY);		// Fecha Via Leste-Oeste esquerda

		// Abrindo
		xSemaphoreGive(xEW_Straight);	//Abre Via Leste-Oeste frente-direita
		xSemaphoreGive(xNS_Left);		//Abre Via Norte-Sul esquerda

		printf(" -------------------------------------------\n");
		printf(" > EW-frente_direita | # NS-frente_direita  \n");
		printf(" > NS-esquerda       | # EW-esquerda        \n");
		printf(" -------------------------------------------\n");

		vTaskDelay(pdMS_TO_TICKS(T)); // Verde / Vermelho

		printf(" -------------------------------------------\n");
		printf(" * EW-frente_direita | # NS-frente_direita  \n");
		printf(" * NS-esquerda       | # EW-esquerda        \n");
		printf(" -------------------------------------------\n");

		vTaskDelay(pdMS_TO_TICKS(T / 4)); // Amarelo / Vermelho
	}
}

int* definirRota() {
	srand(time(NULL)); // Seed peseudo aleat�ria

	int pont_nascimento = rand() % 8; 	//Ponto de surgimento do carro [0 - 7]
	int i, N = 20;

	int direcoes[20];                  //[0, 1, 2] -> [Direita, Esquerda, Frente]

	//Primeiro elemento � a posi��o de surgimento
	direcoes[0] = pont_nascimento; 
	direcoes[1] = rand() % 3;

	//PAR
	if (pont_nascimento % 2 == 0) {
		if (direcoes[1] == 0) {
			direcoes[2] = -1;
		}
		else if (direcoes[1] == 1) {
			for (i = 2; i <= N; i++)
			{
				direcoes[i] = rand() % 3;
				if (direcoes[i] != 0) {
					direcoes[i + 1] = -1;
					break;
				}
			}
		}
		else {
			for (i = 2; i <= N; i++)
			{
				direcoes[i] = rand() % 3;
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
				direcoes[i] = rand() % 3;
				if (direcoes[i] != 1) {
					direcoes[i + 1] = -1;
					break;
				}
			}
		}
		else {
			for (i = 2; i <= N; i++)
			{
				direcoes[i] = rand() % 3;
				if (direcoes[i] != 0) {
					direcoes[i + 1] = -1;
					break;
				}
			}
		}
	}

	printf("Nasceu em %d\n", pont_nascimento);
	return direcoes;
}

void Carro() {
	int i = 0, N = 20;
	
	int direcaoAtual; // 0 = NS,  1= EW, 2 = SN, 3 = WE;
	int velocidade = 50;
	int distancia = 500;

	int* rota = definirRota();

	//Define dire��o inicial 
	switch (rota[0])
	{
		case 0:
		case 1:
			direcaoAtual = 0;  //NS 
			break;
		case 2:
		case 3:
			direcaoAtual = 1;  //EW 
			break;
		case 4:
		case 5:
			direcaoAtual = 2;  //SN 
			break;
		case 6:
		case 7:
			direcaoAtual = 3;  //WE 
			break;
		default:
			direcaoAtual = 0;  //NS 
			break;

	}

	while (1) {
		i++;
		printf("rota[i] = %d\n", rota[i]);
		if (rota[i] == -1) { break; }


		//Define velociade
		if (direcaoAtual == 0 || direcaoAtual == 1) { //NS ou SN
			velocidade = 60 + geraAleatorio(0, 10) - 5;
		}
		else if (direcaoAtual == 1 || direcaoAtual == 3) { // EW ou WE
			velocidade = 50 + geraAleatorio(0, 10) - 5;;
		}

		printf("O Carro esta no sentido %d a %d km/h\n", direcaoAtual, velocidade);
		//Espera chegar no cruzamento
		vTaskDelay(pdMS_TO_TICKS(500));
		printf("Carro chegou no cruzamneto!\n");

		//Esperar sem�foro com base na dire��o
		if (direcaoAtual == 0 || direcaoAtual == 2) {//Dire��o NS e SN
			switch (rota[i]) {
			case 0://Direita 
				printf("O carro quer virar a direita\n");
				xSemaphoreTake(xNS_Straight, portMAX_DELAY);
				xSemaphoreGive(xNS_Straight);
				break;
			case 1://Esquerda
				printf("O carro quer virar a esquerda\n");
				xSemaphoreTake(xNS_Left, portMAX_DELAY);
				xSemaphoreGive(xNS_Left);
				break;
			case 2://Frente
				printf("O carro quer seguir em fente\n");
				xSemaphoreTake(xNS_Straight, portMAX_DELAY);
				xSemaphoreGive(xNS_Straight);
				break;
			default:
				printf("ERRO!!!!\n\n");
				break;
			}

		}
		else if (direcaoAtual == 1 || direcaoAtual == 3) { //Dire��o EW e WE
			switch (rota[i]) {
			case 0://Direita
				printf("O carro quer virar a direita\n");
				xSemaphoreTake(xEW_Straight, portMAX_DELAY);
				xSemaphoreGive(xEW_Straight);
				break;
			case 1://Esquerda
				printf("O carro quer virar a esquerda\n");
				xSemaphoreTake(xEW_Left, portMAX_DELAY);
				xSemaphoreGive(xEW_Left);
				break;
			case 2://Frente
				printf("O carro quer seguir em fente\n");
				xSemaphoreTake(xEW_Straight, portMAX_DELAY);
				xSemaphoreGive(xEW_Straight);
				break;
			default:
				printf("ERRO!!!!\n\n");
				break;
			}
		
		}
		else { printf("ERRO!!!!");}
		
		printf("O Carro seguiu\n");

		//Muda dire��o
		if (rota[i] == 0) {
			switch (direcaoAtual) {
			case 0:
				direcaoAtual = 1;
				break;
			case 1:
				direcaoAtual = 2;
				break;
			case 2:
				direcaoAtual = 3;
				break;
			case 3:
				direcaoAtual = 0;
				break;
			default:
				direcaoAtual = direcaoAtual;
				break;
			}
		
		}
		else if (rota[i] == 1) {
			switch (direcaoAtual) {
			case 0:
				direcaoAtual = 3;
				break;
			case 1:
				direcaoAtual = 0;
				break;
			case 2:
				direcaoAtual = 1;
				break;
			case 3:
				direcaoAtual = 2;
				break;
			default:
				direcaoAtual = direcaoAtual;
				break;
			}
		
		}
		else { direcaoAtual = direcaoAtual; }
		
	}
	printf("O carro Morreu!!!!\n");
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

	xNS_Straight = xSemaphoreCreateBinary();
	xEW_Straight = xSemaphoreCreateBinary();
	xNS_Left     = xSemaphoreCreateBinary();
	xEW_Left     = xSemaphoreCreateBinary();

	xTaskHandle Cruz, Car;
	/* create task */
	xTaskCreate(Cruzamento, (signed char*)"Cruzamento", configMINIMAL_STACK_SIZE, (void*)NULL, 5, &Cruz);
	xTaskCreate(Carro, (signed char*)"Carro", configMINIMAL_STACK_SIZE, (void*)NULL, 1, &Car);


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

