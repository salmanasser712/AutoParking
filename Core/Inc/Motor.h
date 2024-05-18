/*
 * Motor.h
 *
 *  Created on: Nov 21, 2023
 *      Author: Owner
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_
#include "stm32f1xx_hal.h"

#define Motor1_Timer            htim4

#define Motor1_Port             GPIOB
#define Motor1_pin1				GPIO_PIN_4
#define Motor1_pin2				GPIO_PIN_7
#define Motor1_EN				GPIO_PIN_8

#define Motor2_pin1_Port				GPIOA
#define Motor2_pin1				GPIO_PIN_11

#define Motor2_pin2_Port				GPIOB
#define Motor2_pin2				GPIO_PIN_9

#define Motor2_EN_Port				GPIOB
#define Motor2_EN				GPIO_PIN_6


// defining Speed percents
typedef enum{
	S_0  = 0,
	S_10 = 10,
	S_20 = 20,
	S_30 = 30,
	S_40 = 40,
	S_50 = 50,
	S_60 = 60,
	S_70 = 70,
	S_80 = 80,
	S_90 = 90,
	S_100 = 100
}Speed_t;


void Motor_Init_Timer(void);
void Motor1_Stop(void);
void Motor2_Stop(void);
void Motor_Forward(Speed_t speed);
void Motor_Backward(Speed_t speed);
void Motor_Right(void);
void Motor_Left(void);

#endif /* INC_MOTOR_H_ */
