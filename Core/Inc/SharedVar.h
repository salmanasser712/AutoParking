
#ifndef INC_SHAREDVAR_H_
#define INC_SHAREDVAR_H_

#include "std_typo.h"

typedef enum{
	IDLE,
	SEARCH_RIGHT,
	SEARCH_LEFT,
	PARK_RIGHT,
	PARK_LEFT
}Mode_t;

typedef enum{
	OBSTACLE_OFF,
	OBSTACLE_HIGH_ON
}OB_type;

void SharedVar_Init(void);
void ultrasonics_arr_Setter(u16 num,u8 Loc);
u16 ultrasonics_arr_Getter(u8 Loc);
void ultrasonics_Flagarr_Setter(u8 num,u8 Loc);
u8 ultrasonics_Flagarr_Getter(u8 Loc);
void mode_Setter(Mode_t m);
Mode_t mode_Getter(void);
void Obstacle_mode_Setter(OB_type b);
OB_type Obstacle_mode_Getter(void);

#endif /* INC_SHAREDVAR_H_ */
