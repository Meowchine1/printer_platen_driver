#include "init_components.h"

void init_components() {
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

	// --------ENDERS INITIALIZATION -----------------------------------
	PE.Pin = ENDER_PE_Pin;
	PE.Port = ENDER_PE_GPIO_Port;
	PE.State = 0;
	PE.gpio_out_Port = GPIOA;
	PE.gpio_out_Pin = GPIO_OUT_PE_Pin;

	ASF.Pin = ENDER_LASER_Pin;
	ASF.Port = ENDER_LASER_GPIO_Port;
	ASF.State = 0;
	ASF.gpio_out_Port = GPIOA;
	ASF.gpio_out_Pin = GPIO_OUT_ASF_Pin;

	Endstop.Pin = ENDER_ENDSTOP_Pin;
	Endstop.Port = ENDER_ENDSTOP_GPIO_Port;

	//---------ENCODER-----------------------------------------------------

	Enc.enc_counter = 0;
	Enc.enc_increment = 0;
	Enc.enc_prev = 0;

}

