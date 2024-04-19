#include "app.h"

void app_start(void){

		// BUTTONS INITIALIZATION------------
		btn_up.Pin = BTN_UP_Pin;
		btn_up.Port = BTN_UP_GPIO_Port;
		btn_up.Btn_state = IDLE;

		btn_down.Pin = BTN_DOWN_Pin;
		btn_down.Port = BTN_DOWN_GPIO_Port;
		btn_down.Btn_state = IDLE;

		btn_in.Pin = BTN_IN_Pin;
		btn_in.Port = BTN_IN_GPIO_Port;
		btn_in.Btn_state = IDLE;

		btn_back.Pin = BTN_BACK_Pin;
		btn_back.Port = BTN_BACK_GPIO_Port;
		btn_back.Btn_state = IDLE;
		// -------------------------------------

		// MOTOR INITIALIZATION ----------------
		motor_horizontal.Dirrection = 1;
		motor_horizontal.Pin = MOTOR_A_STEP_Pin;
		motor_horizontal.Port = MOTOR_A_STEP_GPIO_Port;
		motor_horizontal.Enable = 0;
		motor_horizontal.Position = 0; // TODO

		motor_vertical.Dirrection = 1;
		motor_vertical.Pin = MOTOR_A_STEP_Pin;
		motor_vertical.Port = MOTOR_A_STEP_GPIO_Port;
		motor_vertical.Enable = 0;
		motor_vertical.Position = 0; // TODO
		// -------------------------------------


}
