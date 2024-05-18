#include "Motor.h"

// External timer sending the PWM signal
extern TIM_HandleTypeDef htim4;

// Starting the timer and the PWM signal but stopping the back motor
void Motor_Init_Timer(void){
	HAL_TIM_Base_Start(&Motor1_Timer);
	HAL_TIM_PWM_Start(&Motor1_Timer, TIM_CHANNEL_3);
	Motor1_Stop();
}

// Stopping the back motor by nearly closing the PWM signal
// sending HIGH value to the two pins controlling the direction of the Dc-motor
void Motor1_Stop(void){
	HAL_GPIO_WritePin(Motor1_Port, Motor1_pin1, 1);
	HAL_GPIO_WritePin(Motor1_Port, Motor1_pin2, 1);
	TIM4->CCR3 = 0;
}

// Setting the front wheels facing forward by stopping the front DC-motor
// By sending LOW to Direction pins and the Enable pin
void Motor2_Stop(void){
	HAL_GPIO_WritePin(Motor2_pin1_Port, Motor2_pin1, 0);
	HAL_GPIO_WritePin(Motor2_pin2_Port, Motor2_pin2, 0);
	HAL_GPIO_WritePin(Motor2_EN_Port, Motor2_EN, 0);
}

// Make the back motor move with a certain speed by changing the duty cycle of the PWM pulse
// Making the back motor move forward
void Motor_Forward(Speed_t speed){
	HAL_GPIO_WritePin(Motor1_Port, Motor1_pin1, 1);
	HAL_GPIO_WritePin(Motor1_Port, Motor1_pin2, 0);
	TIM4->CCR3 = (speed * 200);
}

// Make the back motor move with a certain speed by changing the duty cycle of the PWM pulse
// Making the back motor move backward
void Motor_Backward(Speed_t speed){
	HAL_GPIO_WritePin(Motor1_Port, Motor1_pin1, 0);
	HAL_GPIO_WritePin(Motor1_Port, Motor1_pin2, 1);
	TIM4->CCR3 = (speed * 200);
}

// change the steering of the front wheels by changing the direction of rotation of the front DC-motor to right
void Motor_Right(void){
	HAL_GPIO_WritePin(Motor2_pin1_Port, Motor2_pin1, 1);
	HAL_GPIO_WritePin(Motor2_pin2_Port, Motor2_pin2, 0);
	HAL_GPIO_WritePin(Motor2_EN_Port, Motor2_EN, 1);
}

// change the steering of the front wheels by changing the direction of rotation of the front DC-motor to left
void Motor_Left(void){
	HAL_GPIO_WritePin(Motor2_pin1_Port, Motor2_pin1, 0);
	HAL_GPIO_WritePin(Motor2_pin2_Port, Motor2_pin2, 1);
	HAL_GPIO_WritePin(Motor2_EN_Port, Motor2_EN, 1);
}
