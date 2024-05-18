

#ifndef INC_PARK_H_
#define INC_PARK_H_
#include "std_typo.h"

typedef enum{
	INIT_STEP,
	FIRST_PARK_MODE,
	SECOND_PARK_MODE,
	THIRD_PARK_MODE,
	FOURTH_PARK_MODE,
	FIFTH_PARK_MODE,
	SIXTH_PARK_MODE
}ParkMode_t;

#define Forward_speed       S_10
#define Backward_speed      S_10
#define ForwardWD_speed       S_40
#define BackwardWD_speed      S_40
#define ForwardWD_fast_speed       S_50
#define BackwardWD_slow_speed      S_30

void Park_Runnable(void);

#endif /* INC_PARK_H_ */
