#include "Uart.h"
extern UART_HandleTypeDef huart2;

// Static variables for storing previous received value, receive data pointer, and UART flag pointer
static u8 perv_value = 0;
static uint8_t *ptr_rxData;
static uint8_t *ptr_Uart_flag;

// Function to initialize UART with pointers to UART flag and receive data
void Init_Uart(uint8_t *ptr_1, uint8_t *ptr_2) {
	// Assign pointers
	ptr_Uart_flag = ptr_1;
	ptr_rxData = ptr_2;
	// Start UART receive interrupt
	HAL_UART_Receive_IT(&huart2, ptr_rxData, 1);
}

// Function to get Bluetooth command from UART
uint8_t Get_Bluetooth_Command() {
	// Check if UART flag is set
	if (*ptr_Uart_flag) {
		// Check if the received data is different from the previous value
		if (*ptr_rxData != perv_value) {
			// Update previous value and clear the UART flag
			perv_value = *ptr_rxData;
			*ptr_Uart_flag = 0;
			// Return the received data
			return (*ptr_rxData);
		}
		// Clear the UART flag if data is the same as the previous value
		*ptr_Uart_flag = 0;
	}
	// Return 'N' if no new data is received
	return 'N';
}


