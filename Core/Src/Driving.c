#include "Driving.h"
#include "SharedVar.h"
#include "Motor.h"
#include "Park.h"
#include "Search.h"
#include "Ultra_Sonic.h"



static Speed_t Speed = S_50;
static u8 motor_flag = 0;
static u8 onetimeflagF=1,onetimeflagB=1;
static u8 Distance_Arr[11]={2,15,40,60,80,95,110,120,130,140,150};

   /* Starting the PWM signal */
void Driving_Init(void)
{
	Motor_Init_Timer();
}

    /* function to update forward and backward motor speed */
static void update_speed(Speed_t speed){
	if(motor_flag == 1){
		Motor_Forward(Speed);
	}
	else if(motor_flag == 2){
		Motor_Backward(Speed);
	}
}

   /* function to control the speed and the direction of the car based on a character received
    * from bluetooth */
void Driving_Runnable(u8 num)
{
	if((num>='0'&&num<='9')||(num=='q')){
		switch(num)
		{
		case '0':
			Speed=S_0;
			break;
		case '1':
			Speed=S_10;
			break;
		case '2':
			Speed=S_20;
			break;
		case '3':
			Speed=S_30;
			break;
		case '4':
			Speed=S_40;
			break;
		case '5':
			Speed=S_50;
			break;
		case '6':
			Speed=S_60;
			break;
		case '7':
			Speed=S_70;
			break;
		case '8':
			Speed=S_80;
			break;
		case '9':
			Speed=S_90;
			break;
		case 'q':
			Speed=S_100;
			break;
		}
		update_speed(Speed);
	}
	else{
		switch(num)
		{
		case 'S':
			Motor1_Stop();
			Motor2_Stop();
			motor_flag = 0;
			break;
		case 'F':
			Motor2_Stop();
			Motor_Forward(Speed);
			motor_flag = 1;
			break;
		case 'B':
			Motor2_Stop();
			Motor_Backward(Speed);
			motor_flag = 2;
			break;
		case 'L':
			Motor1_Stop();
			Motor_Left();
			motor_flag = 0;
			break;
		case 'R':
			Motor1_Stop();
			Motor_Right();
			motor_flag = 0;
			break;
		case 'G':
			Motor_Forward(Speed);
			Motor_Left();
			motor_flag = 1;
			break;
		case 'I':
			Motor_Forward(Speed);
			Motor_Right();
			motor_flag = 1;
			break;
		case 'H':
			Motor_Backward(Speed);
			Motor_Left();
			motor_flag = 2;
			break;
		case 'J':
			Motor_Backward(Speed);
			Motor_Right();
			motor_flag = 2;
			break;
		case 'V':
			Obstacle_mode_Setter(OBSTACLE_HIGH_ON);
			break;
		case 'v':
			Obstacle_mode_Setter(OBSTACLE_OFF);
			break;
		case 'D':
			Motor1_Stop();
			Motor2_Stop();
			motor_flag = 0;
			break;
		case 'W':
			mode_Setter(SEARCH_RIGHT);
			break;
		case 'U':
			mode_Setter(SEARCH_LEFT);
			break;
		}
	}
	/*if(mode_Getter()==SEARCH_RIGHT){
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);
	}*/
}

   /* if obstacle avoidance is on we enable the ultrasonic sensor based on direcion the car is moving in
    * if the activated sensor measures less than or equal a specific distance (this distance depends on the current speed of the car) */
void Obstacle_Avoidance(void)
{
	if(Obstacle_mode_Getter()==OBSTACLE_HIGH_ON){
		if(motor_flag==1)
		{
			u16 num=ultrasonics_arr_Getter(Sensor1_ind);
			u8 flagnum=ultrasonics_Flagarr_Getter(Sensor1_ind);
			if(onetimeflagF)
			{
				onetimeflagF=0;
				Ultrasonic_RightSideSearch();
			}
			if((num<=Distance_Arr[(Speed/10)]) && (flagnum==1))
			{
				Motor1_Stop();
				Motor2_Stop();
			}
		}
		else if(motor_flag==2)
		{
			u16 num=ultrasonics_arr_Getter(Sensor4_ind);
			u8 flagnum=ultrasonics_Flagarr_Getter(Sensor4_ind);
			if(onetimeflagB)
			{
				onetimeflagB=0;
				Ultrasonic_RightSidePark();
			}
			if((num<=Distance_Arr[(Speed/10)]) && (flagnum==1))
			{
				Motor1_Stop();
				Motor2_Stop();
			}
		}
		else{
			stop_reading();
			onetimeflagF=1;
			onetimeflagB=1;
		}
	}
	else{

	}
}


//////////////////////////////////////////////////////////////////
/*
void Parking_modes(uint8 ch){
	Mode_t mode;
	mode = mode_Getter();
	if(mode == IDLE){
		Driving_Runnable(ch);
		Obstacle_Avoidance();
	}
	else if(mode == SEARCH_RIGHT || mode == SEARCH_LEFT){
		Search_Runnable();
	}
	else if(mode == PARK_RIGHT || mode == PARK_LEFT){
		Park_Runnable();
	}
}
*/




