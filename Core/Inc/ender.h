
#include "stm32g431xx.h"

//typedef enum {
//
//	INIT,
//	CHECKED,
//	ACTIVE,
//	INACTIVE
//
//} Ender_state_t;

typedef struct  {
	//Ender_state_t State;
	uint16_t Pin;
	GPIO_TypeDef* Port;
	unsigned long Detect_start_time;
	uint8_t Checked;

} Ender_t ;


