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
#include "gpio.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "sdram.h"
#include "lv_demo_benchmark.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
osMutexId_t lvgl_MutexHandle; //创建互斥锁
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
const osMutexAttr_t lvgl_Mutex_attributes = {
  .name = "lvgl_Mutex"
};
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for base_lvgl_task */
osThreadId_t base_lvgl_taskHandle;
const osThreadAttr_t base_lvgl_task_attributes = {
  .name = "base_lvgl_task",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for lvgl_gui */
osThreadId_t lvgl_guiHandle;
const osThreadAttr_t lvgl_gui_attributes = {
  .name = "lvgl_gui",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void lvgl_task(void *argument);
void lvgl_gui1(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationTickHook(void);

/* USER CODE BEGIN 3 */
void vApplicationTickHook( void )
{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
  lv_tick_inc(1); //在FreeRTOS的tick hook中调用LVGL的tick增加函数
}
/* USER CODE END 3 */

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
  lvgl_MutexHandle = osMutexNew(&lvgl_Mutex_attributes);
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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of base_lvgl_task */
  base_lvgl_taskHandle = osThreadNew(lvgl_task, NULL, &base_lvgl_task_attributes);

  /* creation of lvgl_gui */
  lvgl_guiHandle = osThreadNew(lvgl_gui1, NULL, &lvgl_gui_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */

  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    //printf("Hello World!\r\n");
    vTaskDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_lvgl_task */
/**
* @brief Function implementing the base_lvgl_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_lvgl_task */
void lvgl_task(void *argument)
{
  /* USER CODE BEGIN lvgl_task */
  uint32_t time;
  lv_init();           // 初始化LVGL库
  lv_port_disp_init(); // 初始化显示驱动
  lv_port_indev_init(); // 初始化输入设备
  lv_demo_benchmark(); // 运行benchmark演示
  
  /* Infinite loop */
  for(;;)
  {
    //在互斥锁内执行LVGL任务
    osMutexAcquire(lvgl_MutexHandle, portMAX_DELAY);
    lv_task_handler();
    osMutexRelease(lvgl_MutexHandle);

    Touch_Scan(); //扫描触摸屏
    // 延时不能小于 LV_DISP_DEF_REFR_PERIOD，否则会闪烁，另外触摸扫描的延时也不能太低
    osDelay(LV_DISP_DEF_REFR_PERIOD);
  }
  /* USER CODE END lvgl_task */
}

/* USER CODE BEGIN Header_lvgl_gui1 */
/**
* @brief Function implementing the lvgl_gui thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_lvgl_gui1 */
void lvgl_gui1(void *argument)
{
  /* USER CODE BEGIN lvgl_gui1 */
  /* Infinite loop */
  for(;;)
  {

    vTaskDelay(5);
  }
  /* USER CODE END lvgl_gui1 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

