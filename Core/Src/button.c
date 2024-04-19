
#include"button.h"


void refresh_btn_state(struct Button btn){
	switch(btn.Btn_state){

	case IDLE :

		break;
	case PRESSED:
		break;

	}


}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	switch (GPIO_Pin){

	case btn_up.Pin:
		if(HAL_GPIO_ReadPin(btn_up.Port, btn_up.Pin) == GPIO_PIN_SET) {
			btn_up.Btn_state = PRESSED;
			// vertical motor start, direction up
			//MOTOR_B_ENA_Pin  MOTOR_B_DIR_Pin

		} else {
			btn_up.Btn_state = IDLE;
			// vertical motor stop
		}
		break;

	case btn_down.Pin:
		if(HAL_GPIO_ReadPin(btn_down.Port, btn_down.Pin) == GPIO_PIN_SET) {
			btn_down.Btn_state = PRESSED;
			// vertical motor start, direction down
			//MOTOR_B_ENA_Pin  MOTOR_B_DIR_Pin

		} else {
			btn_down.Btn_state = IDLE;

		}
		break;

	case btn_in.Pin:
		if(HAL_GPIO_ReadPin(btn_in.Port, btn_in.Pin) == GPIO_PIN_SET) {
			btn_in.Btn_state = PRESSED;
			// horizontal motor start, direction in

			// MOTOR_A_ENA_Pin  // MOTOR_A_DIR_Pin

		} else {
			btn_in.Btn_state = IDLE;
			// horizontal motor stop

		}
		break;

	case btn_back.Pin:
		if(HAL_GPIO_ReadPin(btn_back.Port, btn_back.Pin) == GPIO_PIN_SET) {
			btn_back.Btn_state = PRESSED;
			// horizontal motor start, direction back
			// MOTOR_A_ENA_Pin  // MOTOR_A_DIR_Pin

		} else {
			btn_back.Btn_state = IDLE;

		}
		break;
	case ENDER_PE_Pin:
		if(HAL_GPIO_ReadPin(ENDER_PE_GPIO_Port, ENDER_PE_Pin) == GPIO_PIN_SET) {

				// PAPER EXIST
		} else {
			// PAPER ISN'T EXIST

		}
		break;

	case ENDER_LASER_Pin:
		if(HAL_GPIO_ReadPin(ENDER_LASER_GPIO_Port, ENDER_LASER_Pin) == GPIO_PIN_SET) {
				// obstacle detected, stop moving up  (maybe move down while sensor is up)

		} else {
			// no obstacles

		}
		break;

	case ENDER_ENDSTOP_Pin:
		if(HAL_GPIO_ReadPin(ENDER_ENDSTOP_GPIO_Port, ENDER_ENDSTOP_Pin) == GPIO_PIN_SET) {
				//obstacle detected, stop moving forward

		} else {
			// no obstacles
		}
		break;
	}

}
