#include "Search.h"
#include "SharedVar.h"
#include "Motor.h"
#include "Ultra_Sonic.h"


static u8 flag=1;
static u8 EnableInterrupt_flag=1;

static void SearchRight_Runnable(void)
{
	u32 time;
	u16 num;
	u8 flagnum;
	while(mode_Getter()!=PARK_RIGHT)
	{
		  /* Get Sensor2 Reading  (Right Front Sensor) */
		num=ultrasonics_arr_Getter(Sensor2_ind);
		  /* Get Sensor2 Flag (I Check with this flag if Ultrasonic read the First reading OR Not) */
		flagnum=ultrasonics_Flagarr_Getter(Sensor2_ind);

		 /* if Sensor2 more than or equal 20cm and less than or equal 40cm for 2sec and
		    if he found a place to park change mode to Park mode to start Parking */
		if(num>=20 && num<=40 && flagnum==1){
			if(EnableInterrupt_flag){
				Enable_Timer();
				EnableInterrupt_flag=0;
			}
			time=Get_time();
			if(time>=2500000){
				EnableInterrupt_flag=1;
				flag=1;
				mode_Setter(PARK_RIGHT);
			}
		}
		else{
			Disable_Timer();
			EnableInterrupt_flag=1;
		}
	}
}

static void SearchLeft_Runnable(void)
{
	u32 time;
	u16 num;
	u8 flagnum;
	while(mode_Getter()!=PARK_RIGHT)
	{
		    /* Get Sensor6 Reading  (Left Front Sensor) */
		num=ultrasonics_arr_Getter(Sensor6_ind);
		    /* Get Sensor6 Flag (I Check with this flag if Ultrasonic read the First reading OR Not) */
		flagnum=ultrasonics_Flagarr_Getter(Sensor6_ind);
	    	/* if Sensor6 more than or equal 20cm and less than or equal 40cm for 2sec and
			   if he found a place to park change mode to Park mode to start Parking */
		if(num>=20 && num<=40 && flagnum==1){
			if(EnableInterrupt_flag){
				Enable_Timer();
				EnableInterrupt_flag=0;
			}
			time=Get_time();
			if(time>=2500000){
				EnableInterrupt_flag=1;
				flag=1;
				mode_Setter(PARK_LEFT);
			}
		}
		else{
			Disable_Timer();
			EnableInterrupt_flag=1;
		}
	}
}


void Search_Init(void)
{
	SharedVar_Init();
}

void Search_Runnable(void)
{
	   /* Get the Mode to know I will park Right or Left */
	Mode_t mode=mode_Getter();
	   /* if I'm searching right we need to activate the front and right front sensor
	      and keep moving forward and calling SearchRight_Runnable function until detecting a parking spot */
	if(mode==SEARCH_RIGHT)
	{
		if(flag)
		{
			Motor2_Stop();
			Motor_Forward(Search_speed);
			Ultrasonic_RightSideSearch();
			flag=0;
		}
		SearchRight_Runnable();
	}
	/* else if I'm searching Left we need to activate the front and left front sensor
       and keep moving forward and calling SearchLeft_Runnable function until detecting a parking spot */
	else if(mode==SEARCH_LEFT)
	{
		if(flag)
		{
			Motor2_Stop();
			Motor_Forward(Search_speed);
			Ultrasonic_LeftSideSearch();
			flag=0;
		}
		SearchLeft_Runnable();
	}
}

