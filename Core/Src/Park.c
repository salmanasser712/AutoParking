#include "Park.h"
#include "SharedVar.h"
#include "Motor.h"
#include "Ultra_Sonic.h"


static ParkMode_t ParkMode=INIT_STEP;
static u8 OneTimeFlag=1;

/* Moving forward for 1sec as an initial parking step then start the first parking step */
static void FirstStep_Parking(void)
{
	u32 time;
	time=Get_time();
	if(time>=3500000){
		Motor1_Stop();
		Disable_Timer();
		ParkMode=FIRST_PARK_MODE;
	}
}

/* Moving left forward for 1.6sec when parking on the right side then move to the second parking step */
static void FirstStep_ParkingRight(void)
{
	u32 time;
	if(OneTimeFlag)
	{
		Motor_Forward(ForwardWD_speed);
		Motor_Left();
		Enable_Timer();
		OneTimeFlag=0;
	}
	time=Get_time();
	if(time>=1600000)
	{
		Motor1_Stop();
		Motor2_Stop();
		Disable_Timer();
		OneTimeFlag=1;
		ParkMode=SECOND_PARK_MODE;
	}
}

/* Moving right forward for 1.6sec when parking on the left side then move to the second parking step */
static void FirstStep_ParkingLeft(void)
{
	u32 time;
	if(OneTimeFlag)
	{
		Motor_Forward(ForwardWD_speed);
		Motor_Right();
		Enable_Timer();
		OneTimeFlag=0;
	}
	time=Get_time();
	if(time>=1600000)
	{
		Motor1_Stop();
		Motor2_Stop();
		Disable_Timer();
		OneTimeFlag=1;
		ParkMode=SECOND_PARK_MODE;
	}
}

/* Moving right backward for 1.2sec when parking on the right side then move to the third parking step */
static void SecondStep_ParkingRight(void)
{
	u32 time;
	if(OneTimeFlag)
	{
		Motor_Backward(BackwardWD_speed);
		Motor_Right();
		Enable_Timer();
		OneTimeFlag=0;
	}
	time=Get_time();
	if(time>=1200000)
	{
		Motor1_Stop();
		Motor2_Stop();
		Disable_Timer();
		OneTimeFlag=1;
		ParkMode=THIRD_PARK_MODE;
	}
}

/* Moving left backward for 1.2sec when parking on the left side then move to the third parking step */
static void SecondStep_ParkingLeft(void)
{
	u32 time;
	if(OneTimeFlag)
	{
		Motor_Backward(BackwardWD_speed);
		Motor_Left();
		Enable_Timer();
		OneTimeFlag=0;
	}
	time=Get_time();
	if(time>=1200000)
	{
		Motor1_Stop();
		Motor2_Stop();
		Disable_Timer();
		OneTimeFlag=1;
		ParkMode=THIRD_PARK_MODE;
	}
}

/* Moving backward and activate back and right back sensor and get sensor reading
 * until the back sensor measures less than or equal 40cm ----> then move to the fourth parking step */
static void ThirdStep_ParkingRight(void)
{
	u16 num=ultrasonics_arr_Getter(Sensor4_ind);
	u8 flagnum=ultrasonics_Flagarr_Getter(Sensor4_ind);
	if(OneTimeFlag)
	{
		Motor_Backward(Backward_speed);
		Ultrasonic_RightSidePark();
		OneTimeFlag=0;
	}
	while(ParkMode!=FOURTH_PARK_MODE){
		num=ultrasonics_arr_Getter(Sensor4_ind);
		flagnum=ultrasonics_Flagarr_Getter(Sensor4_ind);
		if(num<=40 && flagnum==1)
		{
			Motor1_Stop();
			OneTimeFlag=1;
			ParkMode=FOURTH_PARK_MODE;
		}
	}
}

/* Moving backward and activate back and left back sensor and get sensor reading
 * until the back sensor measures less than or equal 40cm ----> then move to the fourth parking step */
static void ThirdStep_ParkingLeft(void)
{
	u16 num=ultrasonics_arr_Getter(Sensor4_ind);
	u8 flagnum=ultrasonics_Flagarr_Getter(Sensor4_ind);
	if(OneTimeFlag)
	{
		Motor_Backward(Backward_speed);
		Ultrasonic_LeftSidePark();
		OneTimeFlag=0;
	}
	while(ParkMode!=FOURTH_PARK_MODE){
		num=ultrasonics_arr_Getter(Sensor4_ind);
		flagnum=ultrasonics_Flagarr_Getter(Sensor4_ind);
		if(num<=30 && flagnum==1)
		{
			Motor1_Stop();
			OneTimeFlag=1;
			ParkMode=FOURTH_PARK_MODE;
		}
	}
}

/* Moving left backward and activate back and right back sensor and get sensor reading
 * until the back sensor measures (less than or equal 10cm)
 *  OR the right back sensor measures (less than or equal 3cm) ----> then move to the fifth parking step */
static void FourthStep_ParkingRight(void)
{
	u16 num=ultrasonics_arr_Getter(Sensor3_ind);
	u16 num1=ultrasonics_arr_Getter(Sensor4_ind);
	u8 flagnum=ultrasonics_Flagarr_Getter(Sensor3_ind);
	u8 flagnum1=ultrasonics_Flagarr_Getter(Sensor4_ind);
	if(OneTimeFlag)
	{
		Ultrasonic_RightSidePark();
		Motor_Backward(BackwardWD_speed);
		Motor_Left();
		OneTimeFlag=0;
	}
	if((num1<=10 && flagnum1==1) || (num<=3 && flagnum==1))
	{
		Motor1_Stop();
		Motor2_Stop();
		OneTimeFlag=1;
		ParkMode=FIFTH_PARK_MODE;
	}
}

/* Moving right backward and activate back and left back sensor and get sensor reading
 * until the back sensor measures (less than or equal 10cm)
 *  OR the left back sensor measures (less than or equal 3cm) ----> then move to the fifth parking step */
static void FourthStep_ParkingLeft(void)
{
	u16 num=ultrasonics_arr_Getter(Sensor5_ind);
	u16 num1=ultrasonics_arr_Getter(Sensor4_ind);
	u8 flagnum=ultrasonics_Flagarr_Getter(Sensor5_ind);
	u8 flagnum1=ultrasonics_Flagarr_Getter(Sensor4_ind);
	if(OneTimeFlag)
	{
		Ultrasonic_LeftSidePark();
		Motor_Backward(BackwardWD_speed);
		Motor_Right();
		OneTimeFlag=0;
	}
	if((num1<=10 && flagnum1==1) || (num<3 && flagnum==1))
	{
		Motor1_Stop();
		Motor2_Stop();
		OneTimeFlag=1;
		ParkMode=FIFTH_PARK_MODE;
	}
}

/* Moving forward for 1sec then turn the front wheel right for 0.3sec then stop the two motors and
 * move to sixth step */
static void FifthStep_ParkingRight(void)
{
	u32 time;
	if(OneTimeFlag)
	{
		Enable_Timer();
		OneTimeFlag=0;
	}
	time=Get_time();
	if(time>=1300000)
	{
		Motor1_Stop();
		Motor2_Stop();
		Disable_Timer();
		OneTimeFlag=1;
		ParkMode=SIXTH_PARK_MODE;
	}
	else if(time>=1000000)
	{
		Motor_Forward(ForwardWD_fast_speed);
		Motor_Right();
	}
	else
	{
		Motor_Forward(Forward_speed);
	}

}

/* Moving forward for 1sec then turn the front wheel left for 0.3sec then stop the two motors and
 * move to sixth step */
static void FifthStep_ParkingLeft(void)
{
	u32 time;
	if(OneTimeFlag)
	{
		Enable_Timer();
		OneTimeFlag=0;
	}
	time=Get_time();
	if(time>=1300000)
	{
		Motor1_Stop();
		Motor2_Stop();
		Disable_Timer();
		OneTimeFlag=1;
		ParkMode=SIXTH_PARK_MODE;
	}
	else if(time>=1000000)
	{
		Motor_Forward(ForwardWD_fast_speed);
		Motor_Left();
	}
	else
	{
		Motor_Forward(Forward_speed);
	}
}

/* a periodic function that executes the current step based on ParkMode variable */
void Park_Runnable(void)
{
	Mode_t mode = mode_Getter();
	if(mode==PARK_RIGHT)
	{
		if(ParkMode==INIT_STEP)
		{
			FirstStep_Parking();
		}
		else if(ParkMode==FIRST_PARK_MODE)
		{
			FirstStep_ParkingRight();
		}
		else if(ParkMode==SECOND_PARK_MODE)
		{
			SecondStep_ParkingRight();
		}
		else if(ParkMode==THIRD_PARK_MODE)
		{
			ThirdStep_ParkingRight();
		}
		else if(ParkMode==FOURTH_PARK_MODE)
		{
			FourthStep_ParkingRight();
		}
		else if(ParkMode==FIFTH_PARK_MODE)
		{
			FifthStep_ParkingRight();
		}
		else if(ParkMode==SIXTH_PARK_MODE)
		{
			stop_reading();
			/*	u32 time;
			if(OneTimeFlag)
			{
				Enable_Timer();
				OneTimeFlag=0;
			}
			time=Get_time();
			if(time>=15000000){
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);
			 */
			ParkMode = INIT_STEP;
			//	Disable_Timer();
			mode_Setter(IDLE);
			//	}
		}
	}
	else if(mode==PARK_LEFT)
	{
		if(ParkMode==INIT_STEP)
		{
			FirstStep_Parking();
		}
		else if(ParkMode==FIRST_PARK_MODE)
		{
			FirstStep_ParkingLeft();
		}
		else if(ParkMode==SECOND_PARK_MODE)
		{
			SecondStep_ParkingLeft();
		}
		else if(ParkMode==THIRD_PARK_MODE)
		{
			ThirdStep_ParkingLeft();
		}
		else if(ParkMode==FOURTH_PARK_MODE)
		{
			FourthStep_ParkingLeft();
		}
		else if(ParkMode==FIFTH_PARK_MODE)
		{
			FifthStep_ParkingLeft();
		}
		else if(ParkMode==SIXTH_PARK_MODE)
		{
			stop_reading();
			ParkMode = INIT_STEP;
			mode_Setter(IDLE);
		}
	}
}
