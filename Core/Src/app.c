#include "app.h"

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

void write_com(){
	 CDC_Transmit_FS((unsigned char*)buf_tx, strlen(buf_tx));
}

void app_start(void){

//	  HAL_TIMEx_OCN_Start_DMA(&htim4, TIM_CHANNEL_4, buf_vertical, ENCODER_VAL_COUNT);
//	  HAL_TIMEx_OCN_Start_DMA(&htim15, TIM_CHANNEL_1, buf_horizontal, ENCODER_VAL_COUNT);
	// TODO encoder does'nt work
	  HAL_TIM_Encoder_Start_DMA(&htim2, TIM_CHANNEL_ALL, buf_A, buf_B, ENCODER_VAL_COUNT);
	  //HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

//-----------------------STATE FLAG------------------------------
	  for (int i = 0; i < BARR_ELBITS*BARR_COUNT ; i+=BARR_ELBITS){
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

void write_buttons_state_in_com(void){
	if(BARR_TEST(State_flag, 1)){
		sprintf(buf_tx, "UP DEBOUNCE TICK = %d\r\n", (int)HAL_GetTick());
		write_com();
	}
	if(BARR_TEST(State_flag, 9) ){
		sprintf(buf_tx, "DOWN DEBOUNCE TICK = %d\r\n", (int)HAL_GetTick());
		write_com();
	}
	if(BARR_TEST(State_flag, 17)){
		sprintf(buf_tx, "IN DEBOUNCE TICK = %d\r\n", (int)HAL_GetTick());
		write_com();
	}
	if(BARR_TEST(State_flag, 25)){
		sprintf(buf_tx, "BACK DEBOUNCE \r\n");
		write_com();
	}
	if(BARR_TEST(State_flag, 2)){
		sprintf(buf_tx, "UP PRESSED \r\n");
		write_com();
	}
	if(BARR_TEST(State_flag, 10)){
		sprintf(buf_tx, "DOWN PRESSED \r\n");
		write_com();
	}
	if(BARR_TEST(State_flag, 18)){
		sprintf(buf_tx, "IN PRESSED \r\n");
		write_com();
	}
	if(BARR_TEST(State_flag, 26)){
		sprintf(buf_tx, "BACK PRESSED \r\n");
		write_com();
	}
}

void check_debounce_flag(int bnum, uint16_t pin, GPIO_TypeDef* port, uint32_t start_time){
  if(BARR_TEST(State_flag, bnum)){
	 if ((int)HAL_GetTick() - start_time > SHORT_DURATION){
		BARR_CLEAR(State_flag, bnum);
		BARR_SET(State_flag, (bnum+1));
	  }
   }
}

void start_motor(Motor_t motor){
	HAL_GPIO_WritePin(motor.ENA_Port, motor.ENA_Pin, 1);
	HAL_GPIO_WritePin(motor.DIR_Port, motor.DIR_Pin, motor.Dirrection);
	HAL_TIMEx_OCN_Start(motor.Tim, motor.Channel);
	motor.Enable = 1;
}

void stop_motor(Motor_t motor){

	HAL_GPIO_WritePin(motor.ENA_Port, motor.ENA_Pin, 0);
	HAL_TIMEx_OCN_Stop(motor.Tim, motor.Channel);
	motor.Enable = 0;
}


void app_loop(void) {

	check_debounce_flag(1, btn_up.Pin, btn_up.Port, btn_up.Press_start_time); 				// btn_up
	check_debounce_flag(9, btn_down.Pin, btn_down.Port, btn_down.Press_start_time); 		// btn_down
	check_debounce_flag(17, btn_in.Pin, btn_in.Port, btn_in.Press_start_time);				// btn_in
	check_debounce_flag(25, btn_back.Pin, btn_back.Port, btn_back.Press_start_time);		// btn_back
	check_debounce_flag(33, PE.Pin, PE.Port, PE.Detect_start_time);							// PE
	check_debounce_flag(41, Laser.Pin, Laser.Port, Laser.Detect_start_time);				// Laser
	check_debounce_flag(49, Endstop.Pin, Endstop.Port, btnEndstop_back.Detect_start_time);	// Endstop

	btn_up.Checked = BARR_TEST(State_flag, 2);
	btn_down.Checked = BARR_TEST(State_flag, 10);


	if(platenState == SEND_IN){

		if(BARR_TEST(State_flag, 18)){

		   btn_in.Checked = !btn_in.Checked;
		}

		if(btn_in.Checked){
			if (BARR_TEST(State_flag, 50)){ 		// if endstop was detected
				stop_motor(motor_horizontal);		// stop motor
				btn_in.Checked = 0;
				platenState = READY_TO_PRINT; 		//

			} else{
				motor_horizontal.Dirrection = MOTOR_DIR_FORWARD;
				start_motor(motor_horizontal);
			}

			if(BARR_TEST(State_flag, 26)){ // button back was pushed
				stop_motor(motor_horizontal);
				btn_back.Checked = 1;
				btn_in.Checked = 0;
				platenState = SEND_BACK;
			}

		} else {

			stop_motor(motor_horizontal);
		}


	} else if (platenState == READY_TO_PRINT){


		// handle button back
	} else if (platenState == SEND_BACK){

		if(BARR_TEST(State_flag, 26)){

			btn_back.Checked = !btn_back.Checked;
		}

		if(btn_back.Checked){
			if(BARR_TEST(State_flag, 50)){ 		// СОСТОЯНИЕ ЭНКОДЕРА РАВНО ДЛИНЕ СТОЛИКА? Как понять что приехали в начало
				stop_motor(motor_horizontal);		// stop motor
				btn_back.Checked = 0;
				platenState = HOLDING_IN_FRONT;     //

			 } else{
				motor_horizontal.Dirrection = MOTOR_DIR_REVERSE;
				motor_horizontal.Enable = 1;
				start_motor(motor_horizontal);
			 }

			if(BARR_TEST(State_flag, 18)){ // button in was pushed
				stop_motor(motor_horizontal);
				btn_back.Checked = 0;
				btn_in.Checked = 1;
				platenState = SEND_IN;

			 }
		} else {
			stop_motor(motor_horizontal);
		}

	} else if(platenState == HOLDING_IN_FRONT){

		if(btn_up.Checked){
			motor_vertical.Dirrection = MOTOR_DIR_UP;
			start_motor(motor_vertical);

		} else{ stop_motor(motor_vertical); }

		if(btn_down.Checked){
			motor_vertical.Dirrection = MOTOR_DIR_DOWN;
			start_motor(motor_vertical);

		} else { stop_motor(motor_vertical);}

		if(BARR_TEST(State_flag, 18)){
			btn_in.Checked = 1;
			platenState = SEND_IN;


		} else if(BARR_TEST(State_flag, 26)){
			btn_back.Checked = 1;
			platenState = SEND_BACK;

		}

		// handle buttons

	}

	//HAL_TIMEx_OCN_Start(&htim4, TIM_CHANNEL_4);
//		HAL_TIMEx_OCN_Start(&htim15, TIM_CHANNEL_1);
//		HAL_TIMEx_OCN_Stop(&htim4, TIM_CHANNEL_4);
//		HAL_TIMEx_OCN_Stop(&htim15, TIM_CHANNEL_1);


//	if (platenState == SEND_BACK){  		// Запуск самонаведения и извлечения
//
//		 	 	 	 	 	 	 	 	 	// Проверьте, не нажат ли задний концевой выключатель.
//		 if (!BARR_TEST(State_flag, 50)) { 	// Прочитайте нажатие предельного swith. value_bstop in the code from LOW to HIGH
//			 	 	 	 	 	 	 	 	//.ПРИМЕЧАНИЕ. Предполагается, что проводка переключателя отключена (нормально разомкнутый).
//
//			epsonEncoder.write(0); 			// set encoder to 0
//		    stepperPosition = 0; 			// set stepper position to 0
//		    platenState = ROLLER_EXTRACT; 	// Начните извлечение, перейдя к делу 4
//
//		 } else{
//
//			 motor_horizontal.Dirrection = MOTOR_DIR_REVERSE; 	// digitalWrite(motorDirection, HIGH); // Set motor to reverse
//			 motor_horizontal.Enable = 1;
//			 motor_horizontal.Speed = 100;		// digitalWrite(motorStep, HIGH); // Take a step
//
//			 HAL_GPIO_WritePin(motor_horizontal.ENA_Port, motor_horizontal.ENA_Port, motor_horizontal.Enable);
//			 HAL_GPIO_WritePin(motor_horizontal.DIR_Port, motor_horizontal.DIR_Port, motor_horizontal.Dirrection);
//			 // STEP TIM START
//
//			 delayMicroseconds(bootSpeed); 		// Step Speed
//			 motor_horizontal.Speed = 0; 		//digitalWrite(motorStep, LOW);
//			 motor_horizontal.Enable = 0;
//			 HAL_GPIO_WritePin(motor_horizontal.ENA_Port, motor_horizontal.ENA_Port, motor_horizontal.Enable);
//			 // STEP TIM STOP
//		 }
//
//	} else if (platenState == SEND_IN){ // Отправить планшет
//
//		// Check if back limit switch is pressed.
//		if (!BARR_TEST(State_flag, 50)) { 	// Read pressing of limit swith. NOTE: This assumes wiring the switch as NO (Normally Open)
//		    epsonEncoder.write(0); 		// set encoder to 0
//		    stepperPosition = 0; 		// set stepper position to zero
//		    platenState = READY_TO_PRINT; 			// Ready to print
//
//		 } else{
//			 if (!BARR_TEST(State_flag, 26)) { // КНОПКА ИЗВЛЕЧЕНИЯ\ВЫХОДА? <!>
//			 	platenState = ROLLER_EXTRACT; // 4; <?>
//
//			 } else { /// Если кнопка извлечения / выхода нажата, а на столе нет максимального количества шагов или больше. Отправить принтер назад
//				 if(currentMicros - previousMicros > homeSpeed){
//					previousMicros = currentMicros;
//					motor_horizontal.Enable = 1;
//					motor_horizontal.Dirrection = MOTOR_DIR_FORWARD; //digitalWrite(motorDirection, HIGH); 	// Set motor to forward
//					// TIM STEP 					 // digitalWrite(motorStep, HIGH); 			// Take a step
//					delayMicroseconds(3); 			 // Step Speed
//					digitalWrite(motorStep, LOW);
//					motor_horizontal.Enable = 0;
//					stepperPosition--; 				 // Add step taken
//				}
//			 }
//		 }
//
//	} else if (platenState == READY_TO_PRINT){ // Готовность к печати / Печать
//
//		if (!BARR_TEST(State_flag, 18) && !BARR_TEST(State_flag, 50)) { //  ! btnIN && ! endstop
//		    platenState = HOLDING_IN_FRONT;
//		    stepperPosition = 5000;
//		} else {
//			if (!BARR_TEST(State_flag, 50)) { // этот раздел, чтобы медленно снимать каретку с концевого выключателя
//				motor_horizontal.Dirrection = MOTOR_DIR_FORWARD; //digitalWrite(motorDirection, LOW); // Set motor to forward
//				motor_horizontal.Enable = 1;
//				digitalWrite(motorStep, HIGH); // Take a step
//				delayMicroseconds(alignSpeed); // Step Speed
//				digitalWrite(motorStep, LOW);
//				motor_horizontal.Enable = 0;
//				stepperPosition++; // Add the step taken
//				break;
//			}
//			 /// If Eject / Out button is pressed and the platen isn't already at or beyond the maximum number of steps. Send the platen to the front.
//			if (!BARR_TEST(State_flag, 26) && (newEncoder) == 0) {
//				platenState = ROLLER_EXTRACT;
//			}
//
//			//Follow Epson encoder signal
//			newEncoder = epsonEncoder.read();
//
//			if (newEncoder != positionEncoder) { // Проверьте, не сместилось ли колесо энкодера.
//				stepperCount = newEncoder / stepperFactor; // Подсчитайте, сколько шагов должно быть у двигателя
//				positionEncoder = newEncoder;
//			}
//
//			if (stepperCount > stepperPosition) {  // Движение колесика по часовой стрелке
//
//				motor_horizontal.Dirrection = MOTOR_DIR_FORWARD; // digitalWrite(motorDirection, LOW); // Set motor to forward <!>
//				// TIM STEP //digitalWrite(motorStep, HIGH); // Take the step
//			    delayMicroseconds(2);
//				digitalWrite(motorStep, LOW);
//				stepperPosition++; // Add the step taken
//			}
//
//			if (stepperCount < stepperPosition) {  // Counter clockwise motion of the encoder wheel
//				 motor_horizontal.Dirrection = MOTOR_DIR_REVERSE; //digitalWrite(motorDirection, HIGH); // Set motor to reverse
//				 // TIM STEP // digitalWrite(motorStep, HIGH); // Take the step
//				delayMicroseconds(2);
//				digitalWrite(motorStep, LOW);
//				stepperPosition--; // Subtract the step taken
//			}
//
//			// At XXX encoder trigger the PE signal
//			if ((newEncoder) > peMinEncoder & (newEncoder) < peMaxEncoder) {
//				digitalWrite(peSignal, LOW);
//				peState = 0;
//			}
//
//			// At XXXX encoder untrigger the PE Signal and then either eject or rewind for automatic 2nd pass
//			if ((newEncoder) > platenEncoder) {
//			   digitalWrite(peSignal, HIGH);
//				peState = 1;
//				passCount = passCount + 1;
//
//				if (passCount == maxPasses){ //
//					platenState = 4; // Eject
//				} else {
//					  platenState = 2; // Send the platen back in for automatic 2nd pass
//					}
//				}
//		}
//
//	} else if (platenState == ROLLER_EXTRACT){ // Eject Platen
//		// IF maxSteps has been hit stop and send to case 0 (default)
//		// If In button is pressed and platen isn't already in the back. Send the platen to the back.
//		if (BARR_TEST(State_flag, 18) & (stepperPosition) > 0){
//		    platenState = SEND_IN;
//		    passCount = 0;  // Reset the pass counter to 0.
//		    if (digitalRead(passBtn) == HIGH){ // Reads the Rocker 2 pass switch to determine if the platen does 1 pass or 2.
//		        maxPasses = 1;
//		    } else {
//		           maxPasses = 2;
//		        }
//		 }
//
//		 if (stepperPosition > maxSteps) {
//		     platenState = HOLDING_IN_FRONT; // Set to case 5 so it stops and does nothing but waits for the in button to be pressed Установите для случая 5, чтобы он останавливался и ничего не делал, но ждал нажатия кнопки ввода
//		  }
//
//		   if ((maxSteps - stepperPosition) < 15) {
//			   if(currentMicros - previousMicros > ejectSpeed){
//		              previousMicros = currentMicros;
//		              motor_horizontal.Dirrection = MOTOR_DIR_FORWARD; // digitalWrite(motorDirection, LOW); // Set motor to forward
//		              digitalWrite(motorStep, HIGH); // Take a step
//		              delayMicroseconds(3); // Step Speed
//		              digitalWrite(motorStep, LOW);
//		              stepperPosition++; // Add step taken
//		        }
//		    } else {
//		           unsigned long currentMicros = micros();
//		           if(currentMicros - previousMicros > ejectSpeed){
//		               previousMicros = currentMicros;
//		               motor_horizontal.Dirrection = MOTOR_DIR_FORWARD; // digitalWrite(motorDirection, LOW); // Set motor to forward
//		               digitalWrite(motorStep, HIGH); // Take a step
//		               delayMicroseconds(3); // Step Speed
//		               digitalWrite(motorStep, LOW);
//		               stepperPosition++; // Add step taken
//		            }
//		      }
//
//	} else if (platenState == HOLDING_IN_FRONT){ // Holding in the front for button press
//		       digitalWrite(motorEnable, LOW);
//
//		       // If In button is pressed and platen isn't already in the back. Send the platen to the back.
//		       if (value_inBtn == LOW & (stepperPosition) > 0){
//		            digitalWrite(motorEnable, HIGH);
//		            platenState = 2;
//		            passCount = 0;  // Reset the pass counter to 0.
//
//		            if (digitalRead(passBtn) == HIGH){ // Reads the Rocker 2 pass switch to determine if the platen does 1 pass or 2.
//		                      maxPasses = 1;
//		                   }
//		                    else
//		                  {
//		                      maxPasses = 2;
//		                  }
//		            }
//	}


}

static inline void default_State_flag_switch(int bit_to_clear, int bit_to_set){
	BARR_CLEAR(State_flag, bit_to_clear);
	BARR_SET(State_flag, bit_to_set);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == btn_up.Pin){
		if(HAL_GPIO_ReadPin(btn_up.Port, btn_up.Pin) == GPIO_PIN_SET) {
				btn_up.Press_start_time = HAL_GetTick();
				default_State_flag_switch(0,1);

		 } else {
			 int bit_to_clear = BARR_TEST(State_flag, 2)? 2:1;
			 default_State_flag_switch(bit_to_clear,0);
		 }

	} else if (GPIO_Pin == btn_down.Pin){
		if(HAL_GPIO_ReadPin(btn_down.Port, btn_down.Pin) == GPIO_PIN_SET) {
				btn_down.Press_start_time = HAL_GetTick();
				default_State_flag_switch(8,9);

		    } else {
		    	int bit_to_clear = BARR_TEST(State_flag, 10)? 10:9;
		    	default_State_flag_switch(bit_to_clear,8);
			}

	} else if (GPIO_Pin == btn_in.Pin){
		if(HAL_GPIO_ReadPin(btn_in.Port, btn_in.Pin) == GPIO_PIN_SET) {
				btn_in.Press_start_time = HAL_GetTick();
				default_State_flag_switch(16,17);

		} else {
			int bit_to_clear = BARR_TEST(State_flag, 18)? 18:17;
			default_State_flag_switch(bit_to_clear,16);
          }

	} else if (GPIO_Pin == btn_back.Pin){
		if(HAL_GPIO_ReadPin(btn_back.Port, btn_back.Pin) == GPIO_PIN_SET) {
				btn_back.Press_start_time = HAL_GetTick();
				default_State_flag_switch(24,25);

		} else {
			int bit_to_clear = BARR_TEST(State_flag, 26)? 26:25;
			default_State_flag_switch(bit_to_clear,24);
		  }

	} else if(GPIO_Pin == ENDER_PE_Pin){
		if(HAL_GPIO_ReadPin(ENDER_PE_GPIO_Port, ENDER_PE_Pin) == GPIO_PIN_SET) {
				PE.Detect_start_time = HAL_GetTick();
				default_State_flag_switch(32,33);

		} else {
			int bit_to_clear = BARR_TEST(State_flag, 34)? 34:33;
			default_State_flag_switch(bit_to_clear,32);
		}

	} else if(GPIO_Pin == ENDER_LASER_Pin){
		if(HAL_GPIO_ReadPin(ENDER_LASER_GPIO_Port, ENDER_LASER_Pin) == GPIO_PIN_SET) {
				Laser.Detect_start_time = HAL_GetTick();
				default_State_flag_switch(40,41);

		} else {
			int bit_to_clear = BARR_TEST(State_flag, 42)? 42:41;
			default_State_flag_switch(bit_to_clear,40);
		}

	} else if(GPIO_Pin == ENDER_ENDSTOP_Pin){
		if(HAL_GPIO_ReadPin(ENDER_ENDSTOP_GPIO_Port, ENDER_ENDSTOP_Pin) == GPIO_PIN_SET) {
				Endstop.Detect_start_time = HAL_GetTick();
				default_State_flag_switch(48,49);

		} else {
			int bit_to_clear = BARR_TEST(State_flag, 50)? 50:49;
			default_State_flag_switch(bit_to_clear,48);
		}
	}
}


