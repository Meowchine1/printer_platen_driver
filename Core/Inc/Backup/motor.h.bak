#include <stdint.h>

#include "main.h"
#include "stm32g431xx.h"

#define MOTOR_DIR_FORWARD 1
#define MOTOR_DIR_REVERSE 0

#define MOTOR_DIR_UP 1
#define MOTOR_DIR_DOWN 0

typedef struct {

	TIM_HandleTypeDef *Tim;
	uint32_t Channel;

	uint16_t ENA_Pin;
	GPIO_TypeDef *ENA_Port;

	uint16_t STEP_Pin; // TIM
	GPIO_TypeDef *STEP_Port;

	uint16_t DIR_Pin;
	GPIO_TypeDef *DIR_Port;

	uint8_t Dirrection;  // 1 - up/forward  0 - down/back
	uint8_t Enable;
	int32_t Position;
	uint32_t Speed;
	uint8_t State;

} Motor_t;

void start_motor(Motor_t obj);
void stop_motor(Motor_t obj);

void set_motor_up(Motor_t obj);
void set_motor_down(Motor_t obj);
void set_motor_forward(Motor_t obj);
void set_motor_back(Motor_t obj);


