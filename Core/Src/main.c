/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Ultra_Sonic.h"
#include "Uart.h"
#include "Motor.h"
#include "SharedVar.h"
#include "Driving.h"
#include "Search.h"
#include "Park.h"

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
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;

/* Definitions for Driving_Task */
osThreadId_t Driving_TaskHandle;
const osThreadAttr_t Driving_Task_attributes = {
		.name = "Driving_Task",
		.stack_size = 600 * 4,
		.priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for Searching_Task */
osThreadId_t Searching_TaskHandle;
const osThreadAttr_t Searching_Task_attributes = {
		.name = "Searching_Task",
		.stack_size = 600 * 4,
		.priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for Parking_Task */
osThreadId_t Parking_TaskHandle;
const osThreadAttr_t Parking_Task_attributes = {
		.name = "Parking_Task",
		.stack_size = 600 * 4,
		.priority = (osPriority_t) osPriorityNormal1,
};
/* USER CODE BEGIN PV */
u16 Ultra_reading[6][10];

/*****************************************************************************
 *****************************************************************************
 ** create event Group to handle and synchronize between the three tasks    **
 **task_1:Driving_Task                                                      **
 **task_2:Seaching_Task                                                     **
 **task_3:Parking_Task                                                      **
 *****************************************************************************                                                      *
 *****************************************************************************/
osEventFlagsId_t EventGroup_1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART2_UART_Init(void);
void Start_Driving_Task_Int(void *argument);
void Start_Searching_Task_Int(void *argument);
void Start_Parking_Task_Int(void *argument);

/* USER CODE BEGIN PFP */
void Bluetooth_test();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* Declare a volatile variable to store a single byte of data (character)*/
volatile uint8_t ch;

/* Declare a volatile flag to indicate whether UART data has been received*/
volatile static uint8_t Uart_flag;

/* Declare a volatile variable to store received UART data*/
volatile static uint8_t rxData;

/* Declare a volatile variable to store the system mode (Assuming 'Mode_t' is a user-defined type)*/
volatile Mode_t mode;

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/*******************************************************************
	 ************** Initialize all configured peripherals **************
	 *******************************************************************/
	MX_GPIO_Init();
	MX_TIM2_Init();
	MX_TIM1_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_USART2_UART_Init();
	Init_Uart((uint8_t*)&Uart_flag,(uint8_t*)&rxData);
	Driving_Init();
	SharedVar_Init();
	/* USER CODE BEGIN 2 */
	HAL_Delay(80);
	/* USER CODE END 2 */
	/* Init scheduler */
	osKernelInitialize();

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
	/* creation of Driving_Task */
	Driving_TaskHandle = osThreadNew(Start_Driving_Task_Int, NULL, &Driving_Task_attributes);

	/* creation of Searching_Task */
	Searching_TaskHandle = osThreadNew(Start_Searching_Task_Int, NULL, &Searching_Task_attributes);

	/* creation of Parking_Task */
	Parking_TaskHandle = osThreadNew(Start_Parking_Task_Int, NULL, &Parking_Task_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void)
{

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 64-1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 62000;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 64-1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 62000;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void)
{

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 64-1;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 62000;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */

}

/**
 * @brief TIM4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM4_Init(void)
{

	/* USER CODE BEGIN TIM4_Init 0 */

	/* USER CODE END TIM4_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM4_Init 1 */

	/* USER CODE END TIM4_Init 1 */
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 64-1;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 20000-1;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM4_Init 2 */

	/* USER CODE END TIM4_Init 2 */
	HAL_TIM_MspPostInit(&htim4);

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_9
			|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_12|GPIO_PIN_13
			|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_4|GPIO_PIN_5
			|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9, GPIO_PIN_RESET);

	/*Configure GPIO pins : PA1 PA4 PA5 PA9
                           PA10 PA11 */
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_9
			|GPIO_PIN_10|GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB1 PB12 PB13
                           PB14 PB15 PB4 PB5
                           PB6 PB7 PB9 */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_12|GPIO_PIN_13
			|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_4|GPIO_PIN_5
			|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/*******************************************************************************************
 *******************************************************************************************
 ** Function Of_HAL_UART_RxCpltCallback:                                                  **
 ** the HAL_UART_RxCpltCallback function is called in each time the UART ISR is fired.    **
 ** the UART ISR is fired in each time it receives a character from the bluetooth.        **
 ** Based on the received character  the corresponding event is set                       **
 ** for synchronization between tasks.                                                    **
 ** ****************************************************************************************                                                     *
 ******************************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Check if the interrupt is from USART2*/
	if(huart->Instance==USART2)
	{
		/* Set the UART flag to indicate that data has been received*/
		Uart_flag = 1;

		/*receive the Data (character) from bluetooth by using USART2*/
		ch = Get_Bluetooth_Command();

		/*when character "W" is received the  SEARCH_RIGHT mode is activated*/
		if((ch=='W'))
		{
			/* Set the mode to SEARCH_RIGHT*/
			mode_Setter(SEARCH_RIGHT);

			/* Set an event flag to notify the main program about the action */
			osEventFlagsSet(EventGroup_1, 2);
		}

		/*when character "U" is received the  SEARCH_LEFT mode is activated*/
		else if((ch=='U'))
		{
			/*Set the mode to SEARCH_LEFT*/
			mode_Setter(SEARCH_LEFT);

			/* Set an event flag to notify the main program about the action*/
			osEventFlagsSet(EventGroup_1,2);
		}

		/*when  received  any character other than (W,U,S,D) the IDLE mode is activated*/
		else if(ch!='S'&&ch!='N')
		{
			/*Set the mode to IDLE for other Bluetooth commands*/
			mode_Setter(IDLE);

			/*Set an event flag to notify the main program about the action*/
			osEventFlagsSet(EventGroup_1,4);

		}
		/* Enabling interrupt to be able to receive again*/
		HAL_UART_Receive_IT(&huart2,(uint8_t*)&rxData,1);
	}
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_Start_Driving_Task_Int */
/**
 * @brief  Function implementing the Driving_Task thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_Driving_Task_Int */
void Start_Driving_Task_Int(void *argument)
{
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for(;;)
	{
		/* Wait for the event flag with bit 4 set, indicating a driving task is requested*/
		osEventFlagsWait(EventGroup_1, 4, osFlagsWaitAll, osWaitForever);

		/* Continue executing the following code while the system is in IDLE mode*/
		while (mode_Getter() == IDLE)
		{
			/* Execute the Driving_Runnable function with the current Bluetooth command 'ch'*/
			Driving_Runnable(ch);

			/* Perform obstacle avoidance logic*/
			Obstacle_Avoidance();

			/* Clear the event flag bit 4, indicating that the driving task is completed*/
			osEventFlagsClear(EventGroup_1, 4);
		}
		/* USER CODE END 5 */
	}
}
/* USER CODE BEGIN Header_Start_Searching_Task_Int */
/**
 * @brief Function implementing the Searching_Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_Searching_Task_Int */
void Start_Searching_Task_Int(void *argument)
{
	/* USER CODE BEGIN Start_Searching_Task_Int */
	/* Infinite loop */
	for(;;)
	{
		/* Wait for the event flag with bit 2 set, indicating a searching task is requested */
		osEventFlagsWait(EventGroup_1, 2, osFlagsWaitAll, osWaitForever);

		//osEventFlagsClear(EventGroup_1, 4); (test)

		/* Get the current mode */
		mode = mode_Getter();

		/* Check if the system is in SEARCH_RIGHT or SEARCH_LEFT mode */
		if(mode == SEARCH_RIGHT || mode == SEARCH_LEFT)
		{
			/* Execute the Search_Runnable function for searching actions */
			Search_Runnable();

			/* Clear the event flag bit 2, indicating that the searching task is completed */
			osEventFlagsClear(EventGroup_1, 2);
		}
		/* Check if the system is in PARK_RIGHT or PARK_LEFT mode */
		else if(mode == PARK_RIGHT || mode == PARK_LEFT)
		{
			/* Set an event flag bit 3, indicating a parking task should be performed */
			osEventFlagsSet(EventGroup_1, 3);
		}
	}
	/* USER CODE END Start_Searching_Task_Int */
}

/* USER CODE BEGIN Header_Start_Parking_Task_Int */
/**
 * @brief Function implementing the Parking_Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_Parking_Task_Int */
void Start_Parking_Task_Int(void *argument)
{
	/* USER CODE BEGIN Start_Parking_Task_Int */
	/* Infinite loop */
	for(;;)
	{
		/* Wait for the event flag with bit 3 set, indicating a parking task is requested */
		osEventFlagsWait(EventGroup_1, 3, osFlagsWaitAll, osWaitForever);

		/* Continue executing the following code while the system is in PARK_RIGHT or SEARCH_LEFT mode */
		while (mode_Getter() == PARK_RIGHT || mode_Getter() == SEARCH_LEFT)
		{
			/* Execute the Park_Runnable function for parking actions */
			Park_Runnable();

			/* Clear event flag bit 3, indicating that the parking task is completed */
			osEventFlagsClear(EventGroup_1, 3);

			/* Clear event flag bit 2 (SEARCHING task), indicating that searching is completed (if set) */
			osEventFlagsClear(EventGroup_1, 2);
		}
	}
	/* USER CODE END Start_Parking_Task_Int */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
