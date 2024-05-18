#include "stm32f1xx_hal.h"
#include "Ultra_Sonic.h"

// External timer handles for Input capture and Overflow ISRS
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

// Definition of sensor structure
static Sensor Sensor1, Sensor2, Sensor3, Sensor4, Sensor5, Sensor6;

// Array for Ultra-Sonic readings
static u16 *Ultra_reading;

// First Captured value on raising edge for Ultra-Sonic
static u16 values1_captured[6] = {0};

// flags array to indicate storing the first distance measured of the current active Ultra-Sonic sensors
static volatile uint8 *values2_flags;

// flags array to indicate storing the first captured value on raising edge of the current active Ultra-Sonic sensors
static uint8 values1_flags[6] = {0};

// This mode variable specifies which Ultra-Sonic sensors are currently active and is triggered every 60ms.
// mode = 0  ->  none of the sensors are active.
// mode = 1  -> the front and front right sensors are active "sensor1 and sensor2"
// mode = 2  -> the back and back right sensors are active "sensor3 and sensor4"
// mode = 3  -> the front and the front left sensors are active "sensor1 and sensor6"
// mode = 4  -> the back and the back left sensors are active "sensor4 and sensor5"
static uint8 mode = 0;

//variable to count the number of overflows
volatile u32 current_time = 0;

static u16 cal_reading(u16 val1, u16 val2);
static void Update_Sensor_Reading(TIM_HandleTypeDef *htim, Sensor_channel sen_channel, Sensor_ind Sen_ind);


// Initializing Sensors structures with the specific timers and channels for each sensor and
// Starting the Timers as an input capture in asynchronous mode
void Ultra_sonic_INT(u16 *Ultra_arr, uint8 *Reading_flags){
	Ultra_reading = Ultra_arr;
	values2_flags = Reading_flags;

	Sensor1.Timer = Sensor1_timer;
	Sensor1.sen_channel = Sensor1_channel;
	Sensor1.sen_active_channel = Sensor1_act_channel;

	Sensor2.Timer = Sensor2_timer;
	Sensor2.sen_channel = Sensor2_channel;
	Sensor2.sen_active_channel = Sensor2_act_channel;

	Sensor3.Timer = Sensor3_timer;
	Sensor3.sen_channel = Sensor3_channel;
	Sensor3.sen_active_channel = Sensor3_act_channel;

	Sensor4.Timer = Sensor4_timer;
	Sensor4.sen_channel = Sensor4_channel;
	Sensor4.sen_active_channel = Sensor4_act_channel;

	Sensor5.Timer = Sensor5_timer;
	Sensor5.sen_channel = Sensor5_channel;
	Sensor5.sen_active_channel = Sensor5_act_channel;

	Sensor6.Timer = Sensor6_timer;
	Sensor6.sen_channel = Sensor6_channel;
	Sensor6.sen_active_channel = Sensor6_act_channel;

	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);

	HAL_TIM_IC_Start_IT(&Sensor1_timer, Sensor1_channel);
	HAL_TIM_IC_Start_IT(&Sensor2_timer, Sensor2_channel);
	HAL_TIM_IC_Start_IT(&Sensor3_timer, Sensor3_channel);
	HAL_TIM_IC_Start_IT(&Sensor4_timer, Sensor4_channel);
	HAL_TIM_IC_Start_IT(&Sensor5_timer, Sensor5_channel);
	HAL_TIM_IC_Start_IT(&Sensor6_timer, Sensor6_channel);
}


// triggering 2 given Ultra-Sonics
static void Trigger_pin(GPIO_TypeDef * port1, uint16_t pin1, GPIO_TypeDef * port2, uint16_t pin2){
	HAL_GPIO_WritePin(port1, pin1, 1);
	HAL_GPIO_WritePin(port2, pin2, 1);
	while (__HAL_TIM_GET_COUNTER (&htim1) < 11);
	HAL_GPIO_WritePin(port1, pin1, 0);
	HAL_GPIO_WritePin(port2, pin2, 0);
}


// Setting sensors flags to 0 indicating that the "mode" variable changed and the first distance measurment
// done by the active sensors haven't been received yet.
static void set_flag_arr(){
	values2_flags[0] = values2_flags[1] = values2_flags[2] = values2_flags[3] = values2_flags[4] = values2_flags[5] = 0;
}

// activating the front and front right sensors "sensor1 and sensor2" and disabling the rest of the sensors
void Ultrasonic_RightSideSearch(void){
	mode = 1;
	set_flag_arr();
}

// activating the back and back right sensors "sensor3 and sensor4" and disabling the rest of the sensors
void Ultrasonic_RightSidePark(void){
	mode = 2;
	set_flag_arr();
}

// activating the front and the front left sensors "sensor1 and sensor6" and disabling the rest of the sensors
void Ultrasonic_LeftSideSearch(void){
	mode = 3;
	set_flag_arr();
}

// activating the back and the back left sensors "sensor4 and sensor5" and disabling the rest of the sensors
void Ultrasonic_LeftSidePark(void){
	mode = 4;
	set_flag_arr();
}

// deactivating reading from all Ultra-Sonic
void stop_reading(){
	set_flag_arr();
	mode = 0;
}

// this overflow ISR CallBack is used to trigger the active sensors every 60ms based on the mode variable
// values1_flag is set to 0 to handle most noise cases
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){
	if(htim->Instance == TIM1){
		current_time++;
		if(mode == 1){
			values1_flags[Sensor1_ind] = values1_flags[Sensor2_ind] = 0;
			__HAL_TIM_SET_COUNTER(&htim1, 0);
			Trigger_pin(Sensor1_Trig_Port, Sensor1_trig_Pin, Sensor2_Trig_Port, Sensor2_trig_Pin);
			__HAL_TIM_SET_COUNTER(&htim1, 0);
		}
		else if(mode == 2){
			values1_flags[Sensor3_ind] = values1_flags[Sensor4_ind] = 0;
			__HAL_TIM_SET_COUNTER(&htim1, 0);
			Trigger_pin(Sensor3_Trig_Port, Sensor3_trig_Pin, Sensor4_Trig_Port, Sensor4_trig_Pin);
			__HAL_TIM_SET_COUNTER(&htim1, 0);
		}
		else if(mode == 3){
			values1_flags[Sensor1_ind] = values1_flags[Sensor6_ind] = 0;
			__HAL_TIM_SET_COUNTER(&htim1, 0);
			Trigger_pin(Sensor1_Trig_Port, Sensor1_trig_Pin, Sensor6_Trig_Port, Sensor6_trig_Pin);
			__HAL_TIM_SET_COUNTER(&htim1, 0);
		}
		else if(mode == 4){
			values1_flags[Sensor4_ind] = values1_flags[Sensor5_ind] = 0;
			__HAL_TIM_SET_COUNTER(&htim1, 0);
			Trigger_pin(Sensor5_Trig_Port, Sensor5_trig_Pin, Sensor4_Trig_Port, Sensor4_trig_Pin);
			__HAL_TIM_SET_COUNTER(&htim1, 0);
		}
	}
}

// This Input Capture CallBack is used to identify the timer and channel on which this input capture happened
// to identify which sensor is sending this Echo.
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == Sensor1.Timer.Instance && htim->Channel == Sensor1.sen_active_channel){
		Update_Sensor_Reading(&Sensor1.Timer, Sensor1.sen_channel, Sensor1_ind);
	}
	else if(htim->Instance == Sensor2.Timer.Instance && htim->Channel == Sensor2.sen_active_channel){
		Update_Sensor_Reading(&Sensor2.Timer, Sensor2.sen_channel, Sensor2_ind);
	}
	else if (htim->Instance == Sensor3.Timer.Instance && htim->Channel == Sensor3.sen_active_channel)
	{
		Update_Sensor_Reading(&Sensor3.Timer, Sensor3.sen_channel, Sensor3_ind);
	}
	else if(htim->Instance == Sensor4.Timer.Instance && htim->Channel == Sensor4.sen_active_channel){
		Update_Sensor_Reading(&Sensor4.Timer, Sensor4.sen_channel, Sensor4_ind);
	}
	else if(htim->Instance == Sensor5.Timer.Instance && htim->Channel == Sensor5.sen_active_channel){
		Update_Sensor_Reading(&Sensor5.Timer, Sensor5.sen_channel, Sensor5_ind);
	}
	else if(htim->Instance == Sensor6.Timer.Instance && htim->Channel == Sensor6.sen_active_channel){
		Update_Sensor_Reading(&Sensor6.Timer, Sensor6.sen_channel, Sensor6_ind);
	}

}


// Function to calculate Ultra-Sonic reading based on two captured values
static u16 cal_reading(u16 val1, u16 val2){
	if(val1 >  val2){
		u16 temp = (62000 - val1) + val2;
		return (temp / 58);
	}
	return ((val2 - val1) / 58);
}

// This function is called in Input capture ISR
// given a specifc sensor, timer and channel i can store the captured counter
// as a first value or the second value to Ultra_reading array of the echo being processed
static void Update_Sensor_Reading(TIM_HandleTypeDef *htim, Sensor_channel sen_channel, Sensor_ind Sen_ind){
	if(values1_flags[Sen_ind] == 0){
		values1_captured[Sen_ind] = HAL_TIM_ReadCapturedValue(htim, sen_channel);
		values1_flags[Sen_ind] = 1;
		__HAL_TIM_SET_CAPTUREPOLARITY(htim, sen_channel, TIM_INPUTCHANNELPOLARITY_FALLING);
	}
	else if(values1_flags[Sen_ind] == 1){
		u16 value2 = HAL_TIM_ReadCapturedValue(htim, sen_channel);
		Ultra_reading[Sen_ind] = cal_reading(values1_captured[Sen_ind], value2);

		__HAL_TIM_SET_CAPTUREPOLARITY(htim, sen_channel, TIM_INPUTCHANNELPOLARITY_RISING);
		values1_flags[Sen_ind] = 2;
		values2_flags[Sen_ind] = 1;
	}
}


// Start the timer from 0 to start measuring a certain time
void Enable_Timer(){
	current_time = 0;
	__HAL_TIM_SET_COUNTER(&htim1, 0);
}

// Used to reset the current_timer variable to prevent overflow
void Disable_Timer(){
	current_time = 0;
}


// calculate the time passed since enabling the timer by calling Enable_Timer method
u32 Get_time(){
	u32 time = TIM1->CNT + (current_time * 62000);
	return time;
}
