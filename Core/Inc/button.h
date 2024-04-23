
#include <stdint.h>
#include "stm32g431xx.h"


typedef enum {

	IDLE,
	DEBOUNCE,
	PRESSED,
	SHORT_PRESSED,
	LONG_PRESSED,
	RELEASED // to detect short, long e.t.c types button pushing

} Button_state_t;

typedef struct   {

	Button_state_t State;
	uint16_t Pin;
	GPIO_TypeDef* Port;
	uint32_t Press_start_time;
	uint16_t Send_log;
	uint8_t Checked;
} Button_t;



