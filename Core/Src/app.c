#include "app.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		Enc.enc_counter = __HAL_TIM_GET_COUNTER(htim);
	}
//	buflen = sprintf((char*) buf, "%d\r\n", (int) Enc.enc_counter);
//	CDC_Transmit_FS(buf, buflen);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM6) {
		Enc.enc_increment = Enc.enc_counter - Enc.enc_prev;
		Enc.enc_prev = Enc.enc_counter;
		if (Enc.enc_increment < 500) {

			Enc.enc_relative_counter += Enc.enc_increment;
		}
		buflen = sprintf((char*) buf,
				"Enc.enc_counter = %d \t Enc.enc_increment = %d\r\n",
				(int) Enc.enc_counter, (int) Enc.enc_increment);
		CDC_Transmit_FS(buf, buflen);
		buflen = sprintf((char*) buf, "Enc.enc_relative_counter = %d \r\n",
				(int) Enc.enc_relative_counter);
		CDC_Transmit_FS(buf, buflen);
	}
}

void write_com() {
	CDC_Transmit_FS((unsigned char*) buf_tx, strlen(buf_tx));
}

void app_init(void) {

//-----------------------STATE FLAG------------------------------
	for (int i = 0; i < BARR_ELBITS * BARR_COUNT; i += BARR_ELBITS) {
		BARR_SET(State_flag, i);
	}

	//init_components();
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

void zero_btns() {
	btn_up.Checked = 0;
	btn_down.Checked = 0;
	btn_in.Checked = 0;
	btn_back.Checked = 0;
}

void showEnc() {
	buflen = sprintf((char*) buf, "%d\r\n", (int) Enc.enc_counter);
	CDC_Transmit_FS(buf, buflen);
}

void app_loop(void) {

#ifdef TEST_ASF_PE

	if (PE.rising) {
		PE.encoder_count_before = Enc.enc_counter;
		PE.rising = 0;
		buflen = sprintf((char*) buf, "PE: включен %d\r\n", Enc.enc_counter);
		CDC_Transmit_FS(buf, buflen);
	}

	if (PE.falling) {
		PE.encoder_count_duaration = Enc.enc_counter - PE.encoder_count_before;
		PE.falling = 0;

		buflen =
				sprintf((char*) buf,
						"PE: колво импульсов энкодера до вкл датчика = %d \t продолжительность работы датчика в импульсах энкодера = %d \r\n",
						(int) PE.encoder_count_before,
						(int) PE.encoder_count_duaration);
		CDC_Transmit_FS(buf, buflen);

	}

	if (ASF.rising) {
		ASF.encoder_count_before = Enc.enc_counter;
		ASF.rising = 0;
		buflen = sprintf((char*) buf, "ASF: включен %d\r\n", Enc.enc_counter);
		CDC_Transmit_FS(buf, buflen);
	}

	if (ASF.falling) {
		ASF.encoder_count_duaration = Enc.enc_counter
				- ASF.encoder_count_before;
		ASF.falling = 0;

		buflen =
				sprintf((char*) buf,
						"ASF: колво импульсов энкодера до вкл датчика = %d \t продолжительность работы датчика в импульсах энкодера = %d \r\n",
						(int) ASF.encoder_count_before,
						(int) ASF.encoder_count_duaration);
		CDC_Transmit_FS(buf, buflen);
	}

#else

	check_debounce_flag(1, btn_up.Pin, btn_up.Port, btn_up.Press_start_time); // btn_up
	check_debounce_flag(9, btn_down.Pin, btn_down.Port,
			btn_down.Press_start_time); 		// btn_down
	check_debounce_flag(17, btn_in.Pin, btn_in.Port, btn_in.Press_start_time); // btn_in
	check_debounce_flag(25, btn_back.Pin, btn_back.Port,
			btn_back.Press_start_time);		// btn_back
	check_debounce_flag(33, PE.Pin, PE.Port, PE.Detect_start_time);		// PE
	check_debounce_flag(41, ASF.Pin, ASF.Port, ASF.Detect_start_time);	// Laser
	check_debounce_flag(49, Endstop.Pin, Endstop.Port,
			Endstop.Detect_start_time);	// Endstop

	btn_up.Checked = BARR_TEST(State_flag, 2);
	btn_down.Checked = BARR_TEST(State_flag, 10);
	btn_in.Checked = BARR_TEST(State_flag, 18);
	btn_back.Checked = BARR_TEST(State_flag, 26);
	Endstop.Checked = BARR_TEST(State_flag, 50);

//	Enc.enc_increment = Enc.enc_counter - Enc.enc_prev;
//	Enc.enc_prev = Enc.enc_counter;
//	buflen = sprintf((char*) buf, "%d\r\n", (int) Enc.enc_counter);
//	CDC_Transmit_FS(buf, buflen);

//	buflen = sprintf((char*) buf, "Current state: %s\n\r",
//			stateStrings[platenState]);
//	CDC_Transmit_FS(buf, buflen);

	if (platenState == INIT) {

#ifdef TESTING

		platenState = READY_TO_PRINT;
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
				set_motor_up(motor_vertical); //motor_vertical.Dirrection = MOTOR_DIR_UP;
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
				set_motor_down(motor_vertical); // motor_vertical.Dirrection = MOTOR_DIR_DOWN;
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
			set_motor_forward(motor_horizontal); //motor_horizontal.Dirrection = MOTOR_DIR_FORWARD;
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
			set_motor_back(motor_horizontal); //motor_horizontal.Dirrection = MOTOR_DIR_REVERSE;
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

			if (Enc.enc_increment > 5 && Enc.enc_increment < 500) {
				buflen = sprintf((char*) buf, "Прокрутка перед печатью\r\n");
								CDC_Transmit_FS(buf, buflen);
				buflen = sprintf((char*) buf, "Значение энкодера = %d\r\n",
						(int) Enc.enc_counter);
				CDC_Transmit_FS(buf, buflen);

				Enc.enc_relative_counter = 0;
				platenState = SCROLLING;
				ASF.Checked = 0;
				PE.Checked = 0;
			}

		}
	} else if (platenState == SCROLLING) {

		if ((Enc.enc_relative_counter > PE_START_ENCODER_COUNT) && !PE.State) { // Включение датчика PE

			turn_on(PE); //HAL_GPIO_WritePin(GPIOA, GPIO_OUT_PE_Pin, PE.State);
			PE.Checked = 1;
			buflen = sprintf((char*) buf, "PE  включен\r\n");
			CDC_Transmit_FS(buf, buflen);

		}

		if (((Enc.enc_relative_counter > ASF_ENCODER_COUNT)) && !ASF.State && !ASF.Checked) { // Включение датчика ASF

			turn_on(ASF); //HAL_GPIO_WritePin(GPIOA, GPIO_OUT_ASF_Pin, ASF.State);
			ASF.Start_time_duration = HAL_GetTick();
			ASF.Checked = 1;
			buflen = sprintf((char*) buf, "ASF включен\r\n");
			CDC_Transmit_FS(buf, buflen);

		}

		if (((HAL_GetTick() - ASF.Start_time_duration > ASF_DURATION_MILLIS) )
				&& ASF.State) { // Выключение датчика ASF

			turn_off(ASF); //HAL_GPIO_WritePin(GPIOA, GPIO_OUT_ASF_Pin, ASF.State);
			buflen = sprintf((char*) buf, "ASF отключен\r\n");
			CDC_Transmit_FS(buf, buflen);
		}

		if (Enc.enc_relative_counter > 2 * ENC_SCROLL) {
			turn_off(ASF);
			platenState = PRINTING;
			buflen = sprintf((char*) buf, "Печать начинается\r\n");
			CDC_Transmit_FS(buf, buflen);

		}

	} else if (platenState == PRINTING) {
//5268
		if (((Enc.enc_relative_counter > 2900)) && !ASF.State && !ASF.Checked) { // Включение датчика ASF

					turn_on(ASF); //HAL_GPIO_WritePin(GPIOA, GPIO_OUT_ASF_Pin, ASF.State);
					ASF.Start_time_duration = HAL_GetTick();
					ASF.Checked = 1;
					buflen = sprintf((char*) buf, "ASF включен\r\n");
					CDC_Transmit_FS(buf, buflen);
				}


		if ((Enc.enc_relative_counter > 2192) // Выключение датчика PE
				&& PE.State) {
					turn_off(PE); //HAL_GPIO_WritePin(GPIOA, GPIO_OUT_PE_Pin, PE_state);
					buflen = sprintf((char*) buf, "PE  отключен\r\n");
					CDC_Transmit_FS(buf, buflen);
				}
		if ((Enc.enc_relative_counter > 15260) // Выключение датчика PE
						&& !PE.State) {
			turn_on(PE); //HAL_GPIO_WritePin(GPIOA, GPIO_OUT_PE_Pin, PE_state);
			buflen = sprintf((char*) buf, "PE  вкключен\r\n");
			DC_Transmit_FS(buf, buflen);
		}

		if (Enc.enc_increment > ENC_FAST_SCROLL_SPEED) {
			platenState = ROLLER_EXTRACT;
//			Start_count_encoder = Enc.enc_counter;
			//Enc.enc_relative_counter = 0;
			buflen = sprintf((char*) buf, "Выталкивание валика\r\n");
			CDC_Transmit_FS(buf, buflen);
		}

//		}

	} else if (platenState == ROLLER_EXTRACT) {
		if ((Enc.enc_relative_counter > 15260) // Включение датчика PE
		&& !PE.State) {
			turn_on(PE); //HAL_GPIO_WritePin(GPIOA, GPIO_OUT_PE_Pin, PE_state);
			buflen = sprintf((char*) buf, "PE  включен\r\n");
			CDC_Transmit_FS(buf, buflen);
		}

		if (Enc.enc_increment < 10) {
			platenState = READY_TO_PRINT; // ОСТАЕТСЯ НА КОНЦЕВИКЕ
			buflen = sprintf((char*) buf, "Печать закончена\r\n");
			CDC_Transmit_FS(buf, buflen);
			turn_off(PE); turn_off(ASF);
		}

	}



#endif
}

static inline void default_State_flag_switch(int bit_to_clear, int bit_to_set) {
	BARR_CLEAR(State_flag, bit_to_clear);
	BARR_SET(State_flag, bit_to_set);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == PE.Pin) {
		if (HAL_GPIO_ReadPin(PE.Port, PE.Pin)) { // RISING EDGE
			PE.rising = 1;
		} else { // FALLING EDGE
			PE.falling = 1;
		}

	}
	if (GPIO_Pin == ASF.Pin) {
		if (HAL_GPIO_ReadPin(ASF.Port, ASF.Pin)) { // RISING EDGE
			ASF.rising = 1;
		} else { // FALLING EDGE
			ASF.falling = 1;
		}
	}

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
			ASF.Detect_start_time = HAL_GetTick();
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

