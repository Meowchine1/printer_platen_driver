#include "motor.h"



void start_motor(Motor_t motor) {
	motor.State = 1;
	HAL_GPIO_WritePin(motor.ENA_Port, motor.ENA_Pin, 1);
	HAL_GPIO_WritePin(motor.DIR_Port, motor.DIR_Pin, motor.Dirrection);
	HAL_TIMEx_OCN_Start(motor.Tim, motor.Channel);
}

void stop_motor(Motor_t motor) {
	motor.State = 0;
	HAL_GPIO_WritePin(motor.ENA_Port, motor.ENA_Pin, 0);
	HAL_TIMEx_OCN_Stop(motor.Tim, motor.Channel);
}

void set_motor_up(Motor_t motor){
	motor.Dirrection = MOTOR_DIR_UP;
}

void set_motor_down(Motor_t motor){
	motor.Dirrection = MOTOR_DIR_DOWN;
}

void set_motor_forward(Motor_t motor){
	motor.Dirrection = MOTOR_DIR_FORWARD;
}

void set_motor_back(Motor_t motor){
	motor.Dirrection = MOTOR_DIR_REVERSE;

}
