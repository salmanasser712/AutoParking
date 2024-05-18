/*
 * Ultra_Sonic.h
 *
 *  Created on: Nov 21, 2023
 *      Author: Owner
 */

#ifndef INC_ULTRA_SONIC_H_
#define INC_ULTRA_SONIC_H_

#include "std_typo.h"
#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;


#define 	Sensor1_timer             htim2
#define 	Sensor2_timer             htim3
#define 	Sensor3_timer      		  htim2
#define 	Sensor4_timer 			  htim2
#define 	Sensor5_timer 			  htim2
#define 	Sensor6_timer 			  htim3

#define     Sensor1_Trig_Port              GPIOB
#define     Sensor2_Trig_Port              GPIOB
#define     Sensor3_Trig_Port              GPIOA
#define     Sensor4_Trig_Port              GPIOA
#define     Sensor5_Trig_Port              GPIOB
#define     Sensor6_Trig_Port              GPIOA

#define 	Sensor1_trig_Pin          GPIO_PIN_1
#define 	Sensor2_trig_Pin          GPIO_PIN_5
#define 	Sensor3_trig_Pin          GPIO_PIN_5
#define 	Sensor4_trig_Pin          GPIO_PIN_4
#define 	Sensor5_trig_Pin          GPIO_PIN_0
#define 	Sensor6_trig_Pin          GPIO_PIN_1

#define     Sensor1_Echo_Port         GPIOA
#define     Sensor1_Echo_pin          GPIO_PIN_15

#define     Sensor2_Echo_Port         GPIOA
#define     Sensor2_Echo_pin          GPIO_PIN_6

#define     Sensor3_Echo_Port         GPIOB
#define     Sensor3_Echo_pin          GPIO_PIN_3

#define     Sensor4_Echo_Port         GPIOB
#define     Sensor4_Echo_pin          GPIO_PIN_10

#define     Sensor5_Echo_Port         GPIOB
#define     Sensor5_Echo_pin          GPIO_PIN_11

#define     Sensor6_Echo_Port         GPIOA
#define     Sensor6_Echo_pin          GPIO_PIN_7


// Indices of sensors in the Ultra_reading array, values1_captured, value1_flags and value2_flags
typedef enum{
	Sensor1_ind = 0,
	Sensor2_ind,
	Sensor3_ind,
	Sensor4_ind,
	Sensor5_ind,
	Sensor6_ind
}Sensor_ind;

// defining the timer channels Ultra-Sonic sensors are connected to
typedef enum{
	Sensor1_channel = TIM_CHANNEL_1,
	Sensor2_channel = TIM_CHANNEL_1,
	Sensor3_channel = TIM_CHANNEL_2,
	Sensor4_channel = TIM_CHANNEL_3,
	Sensor5_channel = TIM_CHANNEL_4,
	Sensor6_channel = TIM_CHANNEL_2
}Sensor_channel;

// defining the timer channels Ultra-Sonic sensors are connected to, "for comparing in the Input Capture ISR"
typedef enum{
	Sensor1_act_channel = HAL_TIM_ACTIVE_CHANNEL_1,
	Sensor2_act_channel = HAL_TIM_ACTIVE_CHANNEL_1,
	Sensor3_act_channel = HAL_TIM_ACTIVE_CHANNEL_2,
	Sensor4_act_channel = HAL_TIM_ACTIVE_CHANNEL_3,
	Sensor5_act_channel = HAL_TIM_ACTIVE_CHANNEL_4,
	Sensor6_act_channel = HAL_TIM_ACTIVE_CHANNEL_2
}Sensor_active_channel;

// a structure for defining the timer and channel which Ultra-sonic sensor is sending the echo to.
typedef struct{
	TIM_HandleTypeDef Timer;
	uint32_t sen_channel;
	uint32_t sen_active_channel;
}Sensor;


void Ultra_sonic_INT(u16 *Ultra_arr, uint8 *Reading_flags);
void Ultrasonic_RightSideSearch(void);
void Ultrasonic_RightSidePark(void);
void Ultrasonic_LeftSideSearch(void);
void Ultrasonic_LeftSidePark(void);
void stop_reading(void);
void Enable_Timer(void);
void Disable_Timer(void);
u32 Get_time(void);

#endif /* INC_ULTRA_SONIC_H_ */
