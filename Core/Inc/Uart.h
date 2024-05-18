/*
 * Uart.h
 *
 *  Created on: Nov 24, 2023
 *      Author: Owner
 */

#ifndef INC_UART_H_
#define INC_UART_H_
#include "std_typo.h"
#include "stm32f1xx_hal.h"

void Init_Uart(uint8_t *ptr_1,uint8_t *ptr_2);
uint8 Get_Bluetooth_Command(void);
void Execute_Bluetooth_Command(void);

#endif /* INC_UART_H_ */
