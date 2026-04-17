#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

#include "queue.h"
#include "list.h"
#include <stdio.h>

static void task1_handler (void *parameters);
static void task2_handler (void *parameters);
int main()
{
	TaskHandle_t task1_handle;
	TaskHandle_t task2_handle;	
	BaseType_t status;

	status=xTaskCreate( task1_handler,"Check Humidity",200,"Hello world from task-1",2,&task1_handle );
	configASSERT (status == pdPASS);
	
	
	status=xTaskCreate( task2_handler,"Showin on LCD",200,"Hello world from task-2",2,&task2_handle );
	configASSERT (status == pdPASS);
	//Start Scheduler
	vTaskStartScheduler();
	/*	If the control comes to here , then the lauch of the scheduler has failed due to
	insufficient memory in heap*/

while(1){}


}
static void task1_handler (void *parameters)
{
	while(1)
	{
		printf("%s\n",(char*)parameters);
	}

}
static void task2_handler (void *parameters)
{
	while(1)
	{
		printf("%s\n",(char*)parameters);
	}

}