
#include "SharedVar.h"
#include "Ultra_Sonic.h"


static u16 ultrasonics_arr[6] = {0};
static u8 ultrasonics_Flagarr[6] = {0};
static Mode_t mode=IDLE;
static OB_type Obstacle_flag=OBSTACLE_OFF;

void SharedVar_Init(void)
{
	Ultra_sonic_INT(ultrasonics_arr,ultrasonics_Flagarr);
}

void ultrasonics_arr_Setter(u16 num,u8 Loc)
{
	ultrasonics_arr[Loc]=num;
}

u16 ultrasonics_arr_Getter(u8 Loc)
{
	return ultrasonics_arr[Loc];
}


void ultrasonics_Flagarr_Setter(u8 num,u8 Loc)
{
	ultrasonics_Flagarr[Loc]=num;
}

u8 ultrasonics_Flagarr_Getter(u8 Loc)
{
	return ultrasonics_Flagarr[Loc];
}


void mode_Setter(Mode_t m)
{
	mode=m;
}

Mode_t mode_Getter(void)
{
	return mode;
}

void Obstacle_mode_Setter(OB_type b)
{
	Obstacle_flag=b;
}

OB_type Obstacle_mode_Getter(void)
{
	return Obstacle_flag;
}
