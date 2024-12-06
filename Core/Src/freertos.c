/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SD_use.h"
#include "sdmmc.h"
#include "touch.h"
#include "lvgl.h"
#include "../LVGL/porting/lv_port_disp.h"
#include "../LVGL/porting/lv_port_indev.h"
#include "benchmark/lv_demo_benchmark.h"
#include "../../App/inc/init_task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId Debug_taskHandle;
osThreadId lvgl_taskHandle;
osThreadId flie_taskHandle;
osThreadId key_taskHandle;
osThreadId led_taskHandle;
osThreadId main_taskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void debug_thread(void const * argument);
void lvgl_thread(void const * argument);
void flie_thread(void const * argument);
void key_thread(void const * argument);
void led_thread(void const * argument);
void main_thread(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* Hook prototypes */
void vApplicationTickHook(void);

/* USER CODE BEGIN 3 */
__weak void vApplicationTickHook( void )
{
    /* This function will be called by each tick interrupt if
    configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. Bsp code can be
    added here, but the tick hook is called from an interrupt context, so
    code must not attempt to block, and only the interrupt safe FreeRTOS API
    functions can be used (those that end in FromISR()). */
    lv_tick_inc(1);
}
/* USER CODE END 3 */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* definition and creation of defaultTask */
    osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 1024);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* definition and creation of Debug_task */
    osThreadDef(Debug_task, debug_thread, osPriorityNormal, 0, 1024);
    Debug_taskHandle = osThreadCreate(osThread(Debug_task), NULL);

    /* definition and creation of lvgl_task */
    osThreadDef(lvgl_task, lvgl_thread, osPriorityLow, 0, 1024);
    lvgl_taskHandle = osThreadCreate(osThread(lvgl_task), NULL);

    /* definition and creation of flie_task */
    osThreadDef(flie_task, flie_thread, osPriorityNormal, 0, 1024);
    flie_taskHandle = osThreadCreate(osThread(flie_task), NULL);

    /* definition and creation of key_task */
    osThreadDef(key_task, key_thread, osPriorityLow, 0, 512);
    key_taskHandle = osThreadCreate(osThread(key_task), NULL);

    /* definition and creation of led_task */
    osThreadDef(led_task, led_thread, osPriorityLow, 0, 512);
    led_taskHandle = osThreadCreate(osThread(led_task), NULL);

    /* definition and creation of main_task */
    osThreadDef(main_task, main_thread, osPriorityIdle, 0, 2048);
    main_taskHandle = osThreadCreate(osThread(main_task), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
    /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_debug_thread */
/**
* @brief Function implementing the Debug_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_debug_thread */
void debug_thread(void const * argument)
{
    /* USER CODE BEGIN debug_thread */
    wifi_init();
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
    /* USER CODE END debug_thread */
}

/* USER CODE BEGIN Header_lvgl_thread */
/**
* @brief Function implementing the lvgl_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_lvgl_thread */
void lvgl_thread(void const * argument)
{
    /* USER CODE BEGIN lvgl_thread */
    /* Infinite loop */
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    lv_demo_benchmark();

    for(;;)
    {
        lv_task_handler();
        osDelay(1);
    }
    /* USER CODE END lvgl_thread */
}

/* USER CODE BEGIN Header_flie_thread */
/**
* @brief Function implementing the flie_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_flie_thread */
void flie_thread(void const * argument)
{
    /* USER CODE BEGIN flie_thread */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
    /* USER CODE END flie_thread */
}

/* USER CODE BEGIN Header_key_thread */
/**
* @brief Function implementing the key_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_key_thread */
void key_thread(void const * argument)
{
    /* USER CODE BEGIN key_thread */
    /* Infinite loop */

    for(;;)
    {
        osDelay(300);
    }
    /* USER CODE END key_thread */
}

/* USER CODE BEGIN Header_led_thread */
/**
* @brief Function implementing the led_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_led_thread */
void led_thread(void const * argument)
{
    /* USER CODE BEGIN led_thread */
    LED_R_GPIO_Port->ODR |= LED_R_Pin;
    LED_B_GPIO_Port->ODR |= LED_B_Pin;
    /* Infinite loop */
    for(;;)
    {
        osDelay(500);
        LED_B_GPIO_Port->ODR ^= LED_B_Pin;
    }
    /* USER CODE END led_thread */
}

/* USER CODE BEGIN Header_main_thread */
/**
* @brief Function implementing the main_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_main_thread */
void main_thread(void const * argument)
{
    /* USER CODE BEGIN main_thread */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
    /* USER CODE END main_thread */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
