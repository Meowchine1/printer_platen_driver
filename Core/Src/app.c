#include "app.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	enc_counter = __HAL_TIM_GET_COUNTER(htim);
	buflen = sprintf((char*) buf, "%d\r\n", (int) enc_counter);
	CDC_Transmit_FS(buf, buflen);

}

//
//void update_encoder(Encoder_t *encoder_value, TIM_HandleTypeDef *htim)
// {
//	uint32_t temp_counter = __HAL_TIM_GET_COUNTER(htim);
//
//    if(temp_counter == encoder_value->last_counter_value){
//
//		encoder_value->velocity = 0;
//
//	} else if(temp_counter > encoder_value->last_counter_value){
//
//		if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim)){
//		  encoder_value->velocity = -encoder_value->last_counter_value -
//		(__HAL_TIM_GET_AUTORELOAD(htim) - temp_counter);
//
//		} else {
//		  encoder_value->velocity = temp_counter -
//			   encoder_value->last_counter_value;
//		}
//	  } else {
//		if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim)){
//		encoder_value->velocity = temp_counter -
//				encoder_value->last_counter_value;
//
//		} else {
//		encoder_value->velocity = temp_counter +
//		(__HAL_TIM_GET_AUTORELOAD(htim) -
//				  encoder_value->last_counter_value);
//		}
//	   }
//
//	encoder_value->position += encoder_value->velocity;
//	encoder_value->last_counter_value = temp_counter;
// }
//
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//
//	if(htim == htim6){
//		  timer_counter = __HAL_TIM_GET_COUNTER(&htim6);
//		  // measure velocity, position
//		  update_encoder(&Enc, &htim6);
//		  int64_t encoder_position = Enc.position;
//		  int16_t encoder_velocity = Enc.velocity;
//	}
//}

void write_com() {
	CDC_Transmit_FS((unsigned char*) buf_tx, strlen(buf_tx));
}

void app_init(void) {

//-----------------------STATE FLAG------------------------------
	for (int i = 0; i < BARR_ELBITS * BARR_COUNT; i += BARR_ELBITS) {
		BARR_SET(State_flag, i);
	}
//-----------BUTTONS INITIALIZATION------------------------------
	btn_up.Pin = BTN_UP_Pin;
	btn_up.Port = BTN_UP_GPIO_Port;
	btn_up.State = IDLE;
	btn_up.Checked = 0;

	btn_down.Pin = BTN_DOWN_Pin;
	btn_down.Port = BTN_DOWN_GPIO_Port;
	btn_down.State = IDLE;
	btn_down.Checked = 0;

	btn_in.Pin = BTN_IN_Pin;
	btn_in.Port = BTN_IN_GPIO_Port;
	btn_in.State = IDLE;
	btn_in.Checked = 0;

	btn_back.Pin = BTN_BACK_Pin;
	btn_back.Port = BTN_BACK_GPIO_Port;
	btn_back.State = IDLE;
	btn_back.Checked = 0;
// ----------------------------------------------------------------
// --------- MOTOR INITIALIZATION ---------------------------------
	motor_horizontal.Tim = &htim4;
	motor_horizontal.Channel = TIM_CHANNEL_4;
	motor_horizontal.Dirrection = MOTOR_DIR_FORWARD;
	motor_horizontal.STEP_Pin = MOTOR_A_STEP_Pin;
	motor_horizontal.STEP_Port = MOTOR_A_STEP_GPIO_Port;
	motor_horizontal.ENA_Pin = MOTOR_A_ENA_Pin;
	motor_horizontal.ENA_Port = MOTOR_A_ENA_GPIO_Port;
	motor_horizontal.DIR_Pin = MOTOR_A_DIR_Pin;
	motor_horizontal.DIR_Port = MOTOR_A_DIR_GPIO_Port;
	motor_horizontal.Enable = 0;
	motor_horizontal.Position = 0; // TODO

	motor_vertical.Tim = &htim15;
	motor_vertical.Channel = TIM_CHANNEL_1;
	motor_vertical.Dirrection = MOTOR_DIR_UP;
	motor_vertical.STEP_Pin = MOTOR_A_STEP_Pin;
	motor_vertical.STEP_Port = MOTOR_A_STEP_GPIO_Port;
	motor_vertical.ENA_Pin = MOTOR_A_ENA_Pin;
	motor_vertical.ENA_Port = MOTOR_A_ENA_GPIO_Port;
	motor_vertical.DIR_Pin = MOTOR_A_DIR_Pin;
	motor_vertical.DIR_Port = MOTOR_A_DIR_GPIO_Port;
	motor_vertical.Enable = 0;
	motor_vertical.Position = 0; // TODO
//------------------------------------------------------------------
// --------ENDERS INITIALIZATION -----------------------------------
	PE.Pin = ENDER_PE_Pin;
	PE.Port = ENDER_PE_GPIO_Port;
	Laser.Pin = ENDER_LASER_Pin;
	Laser.Port = ENDER_LASER_GPIO_Port;
	Endstop.Pin = ENDER_ENDSTOP_Pin;
	Endstop.Port = ENDER_ENDSTOP_GPIO_Port;
//------------------------------------------------------------------

}

void write_buttons_state_in_com(void) {
	if (BARR_TEST(State_flag, 1)) {
		sprintf(buf_tx, "UP DEBOUNCE TICK = %d\r\n", (int) HAL_GetTick());
		write_com();
	}
	if (BARR_TEST(State_flag, 9)) {
		sprintf(buf_tx, "DOWN DEBOUNCE TICK = %d\r\n", (int) HAL_GetTick());
		write_com();
	}
	if (BARR_TEST(State_flag, 17)) {
		sprintf(buf_tx, "IN DEBOUNCE TICK = %d\r\n", (int) HAL_GetTick());
		write_com();
	}
	if (BARR_TEST(State_flag, 25)) {
		sprintf(buf_tx, "BACK DEBOUNCE \r\n");
		write_com();
	}
	if (BARR_TEST(State_flag, 2)) {
		sprintf(buf_tx, "UP PRESSED \r\n");
		write_com();
	}
	if (BARR_TEST(State_flag, 10)) {
		sprintf(buf_tx, "DOWN PRESSED \r\n");
		write_com();
	}
	if (BARR_TEST(State_flag, 18)) {
		sprintf(buf_tx, "IN PRESSED \r\n");
		write_com();
	}
	if (BARR_TEST(State_flag, 26)) {
		sprintf(buf_tx, "BACK PRESSED \r\n");
		write_com();
	}
}

void check_debounce_flag(int bnum, uint16_t pin, GPIO_TypeDef *port,
		uint32_t start_time) {
	if (BARR_TEST(State_flag, bnum)) {
		if ((int) HAL_GetTick() - start_time > SHORT_DURATION) {
			BARR_CLEAR(State_flag, bnum);
			BARR_SET(State_flag, (bnum + 1));
		}
	}
}

void start_motor(Motor_t motor) {
	HAL_GPIO_WritePin(motor.ENA_Port, motor.ENA_Pin, 1);
	HAL_GPIO_WritePin(motor.DIR_Port, motor.DIR_Pin, motor.Dirrection);
	HAL_TIMEx_OCN_Start(motor.Tim, motor.Channel);
	motor.Enable = 1;
}

void stop_motor(Motor_t motor) {

	HAL_GPIO_WritePin(motor.ENA_Port, motor.ENA_Pin, 0);
	HAL_TIMEx_OCN_Stop(motor.Tim, motor.Channel);
	motor.Enable = 0;
}

void zero_btns() {
	btn_up.Checked = 0;
	btn_down.Checked = 0;
	btn_in.Checked = 0;
	btn_back.Checked = 0;
}

void app_loop(void) {
	check_debounce_flag(1, btn_up.Pin, btn_up.Port, btn_up.Press_start_time); // btn_up
	check_debounce_flag(9, btn_down.Pin, btn_down.Port,
			btn_down.Press_start_time); 		// btn_down
	check_debounce_flag(17, btn_in.Pin, btn_in.Port, btn_in.Press_start_time); // btn_in
	check_debounce_flag(25, btn_back.Pin, btn_back.Port,
			btn_back.Press_start_time);		// btn_back
	check_debounce_flag(33, PE.Pin, PE.Port, PE.Detect_start_time);		// PE
	check_debounce_flag(41, Laser.Pin, Laser.Port, Laser.Detect_start_time);// Laser
	check_debounce_flag(49, Endstop.Pin, Endstop.Port,
			Endstop.Detect_start_time);	// Endstop

	btn_up.Checked = BARR_TEST(State_flag, 2);
	btn_down.Checked = BARR_TEST(State_flag, 10);
	btn_in.Checked = BARR_TEST(State_flag, 18);
	btn_back.Checked = BARR_TEST(State_flag, 26);
	Endstop.Checked = BARR_TEST(State_flag, 50);

	if (platenState == INIT) {

#ifdef TESTING

		platenState = READY_TO_PRINT;
//		if (Endstop.Checked) {  // For testing!!!!!!!!!!!
//			platenState = READY_TO_PRINT;
//
//		} else
#endif
		if (btn_up.Checked) {
			platenState = SEND_UP;

		} else if (btn_down.Checked) {
			platenState = SEND_DOWN;

		} else if (btn_in.Checked) {
			platenState = SEND_IN;

		} else if (btn_back.Checked) {
			platenState = SEND_BACK;

		}
		zero_btns();

	} else if (platenState == SEND_UP) {

		if (btn_up.Checked) { // LONG PUSH BUTTON MANAGEMENT
			if (height_limit()) {
				stop_motor(motor_vertical);
				platenState = INIT;

			} else {
				motor_vertical.Dirrection = MOTOR_DIR_UP;
				start_motor(motor_vertical);
			}

			if (btn_down.Checked) {
				stop_motor(motor_vertical);
				platenState = SEND_DOWN;
			}
		} else {
			stop_motor(motor_vertical);
			platenState = INIT;

		}

	} else if (platenState == SEND_DOWN) {
		if (btn_down.Checked) {  // LONG PUSH BUTTON MANAGEMENT
			if (height_limit()) {
				stop_motor(motor_vertical);
				platenState = READY_TO_PRINT;

			} else {
				motor_vertical.Dirrection = MOTOR_DIR_DOWN;
				start_motor(motor_vertical);
			}

			if (btn_up.Checked) {
				stop_motor(motor_vertical);
				platenState = SEND_UP;
			}

		} else {
			stop_motor(motor_vertical);
			platenState = INIT;
		}

	} else if (platenState == SEND_IN) {

		if (Endstop.Checked) {
			stop_motor(motor_horizontal);
			platenState = READY_TO_PRINT;

		} else {
			motor_horizontal.Dirrection = MOTOR_DIR_FORWARD;
			start_motor(motor_horizontal);
		}

		if (btn_back.Checked) {
			stop_motor(motor_horizontal);
			platenState = SEND_BACK;
		}

	} else if (platenState == SEND_BACK) {

		if (putting_forward_limit()) {
			stop_motor(motor_horizontal);
			btn_back.Checked = 0;
			platenState = INIT;

		} else {
			motor_horizontal.Dirrection = MOTOR_DIR_REVERSE;
			start_motor(motor_horizontal);
		}

		if (btn_in.Checked) {
			stop_motor(motor_horizontal);
			platenState = SEND_IN;

		} else {
			stop_motor(motor_horizontal);
		}

	} else if (platenState == READY_TO_PRINT) {

		if (btn_back.Checked) {
			platenState = SEND_BACK;
		} else {
			enc_difference = enc_counter - enc_prev_counter;
			enc_prev_counter = enc_counter;
			if (enc_difference > 10) {
				buflen = sprintf((char*) buf, "Encoder speed = %d\r\n",
						(int) enc_difference);
				CDC_Transmit_FS(buf, buflen);

				PE_Start_count_encoder = enc_counter;
				PE_Start_time_duration = HAL_GetTick();
				ASF_Start_time_duration = HAL_GetTick();
				ASF_state = 1;
				HAL_GPIO_WritePin(GPIOA, GPIO_OUT_ASF_Pin, ASF_state);
				buflen = sprintf((char*) buf, "ASF включен\r\n");
				CDC_Transmit_FS(buf, buflen);
				buflen = sprintf((char*) buf, "Прокрутка перед печатью\r\n");
				CDC_Transmit_FS(buf, buflen);
				platenState = SCROLLING;
			}

		}
	} else if (platenState == SCROLLING) {

		if ((enc_counter - PE_Start_count_encoder
				> ENC_SCROLL + PE_ENCODER_COUNT) && !PE_state) {
			PE_state = 1;
			HAL_GPIO_WritePin(GPIOA, GPIO_OUT_PE_Pin, PE_state);
			buflen = sprintf((char*) buf, "PE  включен\r\n");
			CDC_Transmit_FS(buf, buflen);
		}

		if ((HAL_GetTick() - ASF_Start_time_duration > ASF_DURATION_MILLIS)
				&& ASF_state) {

			ASF_state = 0;
			HAL_GPIO_WritePin(GPIOA, GPIO_OUT_ASF_Pin, ASF_state);
			HAL_GPIO_WritePin(GPIOA, GPIO_OUT_PE_Pin, PE_state);
			buflen = sprintf((char*) buf, "ASF отключен\r\n");
			CDC_Transmit_FS(buf, buflen);
		}

		if (enc_counter - PE_Start_count_encoder > 2 * ENC_SCROLL) {
			platenState = PRINTING;
			enc_prev_time = HAL_GetTick();
			buflen = sprintf((char*) buf, "Печать начинается\r\n");
			CDC_Transmit_FS(buf, buflen);

		}

		// timeout

	} else if (platenState == PRINTING) {

		if (HAL_GetTick() - enc_prev_time > 1000) {
			enc_prev_time = HAL_GetTick();
			enc_difference = enc_counter - enc_prev_counter;
			enc_prev_counter = enc_counter;

			if (enc_difference > ENC_FAST_SCROLL_SPEED) {
				platenState = ROLLER_EXTRACT;
				buflen = sprintf((char*) buf, "Выталкивание валика\r\n");
				CDC_Transmit_FS(buf, buflen);
			}

		}

	} else if (platenState == ROLLER_EXTRACT) {
		if ((enc_counter - PE_Start_count_encoder > PE_ENCODER_COUNT)
				&& PE_state) {
			PE_state = 0;
			HAL_GPIO_WritePin(GPIOA, GPIO_OUT_PE_Pin, PE_state);
			buflen = sprintf((char*) buf, "PE  отключен\r\n");
			CDC_Transmit_FS(buf, buflen);
		}

		enc_difference = enc_counter - enc_prev_counter;
		enc_prev_counter = enc_counter;

		if (enc_difference < 10) {
			platenState = READY_TO_PRINT; // ОСТАЕТСЯ НА КОНЦЕВИКЕ
			buflen = sprintf((char*) buf, "Печать закончена\r\n");
			CDC_Transmit_FS(buf, buflen);
		}

	}
}

static inline void default_State_flag_switch(int bit_to_clear, int bit_to_set) {
	BARR_CLEAR(State_flag, bit_to_clear);
	BARR_SET(State_flag, bit_to_set);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == btn_up.Pin) {
		if (HAL_GPIO_ReadPin(btn_up.Port, btn_up.Pin) == GPIO_PIN_SET) {
			btn_up.Press_start_time = HAL_GetTick();
			default_State_flag_switch(0, 1);

		} else {
			int bit_to_clear = BARR_TEST(State_flag, 2) ? 2 : 1;
			default_State_flag_switch(bit_to_clear, 0);
		}

	} else if (GPIO_Pin == btn_down.Pin) {
		if (HAL_GPIO_ReadPin(btn_down.Port, btn_down.Pin) == GPIO_PIN_SET) {
			btn_down.Press_start_time = HAL_GetTick();
			default_State_flag_switch(8, 9);

		} else {
			int bit_to_clear = BARR_TEST(State_flag, 10) ? 10 : 9;
			default_State_flag_switch(bit_to_clear, 8);
		}

	} else if (GPIO_Pin == btn_in.Pin) {
		if (HAL_GPIO_ReadPin(btn_in.Port, btn_in.Pin) == GPIO_PIN_SET) {
			btn_in.Press_start_time = HAL_GetTick();
			default_State_flag_switch(16, 17);

		} else {
			int bit_to_clear = BARR_TEST(State_flag, 18) ? 18 : 17;
			default_State_flag_switch(bit_to_clear, 16);
		}

	} else if (GPIO_Pin == btn_back.Pin) {
		if (HAL_GPIO_ReadPin(btn_back.Port, btn_back.Pin) == GPIO_PIN_SET) {
			btn_back.Press_start_time = HAL_GetTick();
			default_State_flag_switch(24, 25);

		} else {
			int bit_to_clear = BARR_TEST(State_flag, 26) ? 26 : 25;
			default_State_flag_switch(bit_to_clear, 24);
		}

	} else if (GPIO_Pin == ENDER_PE_Pin) {
		if (HAL_GPIO_ReadPin(ENDER_PE_GPIO_Port, ENDER_PE_Pin)
				== GPIO_PIN_SET) {
			PE.Detect_start_time = HAL_GetTick();
			default_State_flag_switch(32, 33);

		} else {
			int bit_to_clear = BARR_TEST(State_flag, 34) ? 34 : 33;
			default_State_flag_switch(bit_to_clear, 32);
		}

	} else if (GPIO_Pin == ENDER_LASER_Pin) {
		if (HAL_GPIO_ReadPin(ENDER_LASER_GPIO_Port, ENDER_LASER_Pin)
				== GPIO_PIN_SET) {
			Laser.Detect_start_time = HAL_GetTick();
			default_State_flag_switch(40, 41);

		} else {
			int bit_to_clear = BARR_TEST(State_flag, 42) ? 42 : 41;
			default_State_flag_switch(bit_to_clear, 40);
		}

	} else if (GPIO_Pin == ENDER_ENDSTOP_Pin) {
		if (HAL_GPIO_ReadPin(ENDER_ENDSTOP_GPIO_Port, ENDER_ENDSTOP_Pin)
				== GPIO_PIN_SET) {
			Endstop.Detect_start_time = HAL_GetTick();
			default_State_flag_switch(48, 49);

		} else {
			int bit_to_clear = BARR_TEST(State_flag, 50) ? 50 : 49;
			default_State_flag_switch(bit_to_clear, 48);
		}
	}
}

