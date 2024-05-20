
#include "stm32g431xx.h"
#include "main.h"
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
	uint8_t State;
	unsigned long Start_time_duration;

	uint16_t gpio_out_Pin;
	GPIO_TypeDef* gpio_out_Port;

	uint8_t rising;
	uint8_t falling;
	uint32_t encoder_count_before;
	uint32_t encoder_count_duaration;

} Ender_t;

void turn_on(Ender_t* obj);
void turn_off(Ender_t* obj);



