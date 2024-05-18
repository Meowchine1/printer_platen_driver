#include "main.h"
#include "ender.h"
#include "config.h"
#include "motor.h"
#include "button.h"
#include "tim.h"

extern Button_t btn_up, btn_down, btn_in, btn_back;
extern Ender_t PE, ASF, Endstop;
extern Encoder_t Enc;
extern Motor_t motor_vertical, motor_horizontal;
//
//Button_t btn_up = {
//		.Pin = BTN_UP_Pin,
//		.Port = BTN_UP_GPIO_Port,
//		.State = IDLE,
//		.Checked = 0
//};
//
//Button_t btn_down = {
//		.Pin = BTN_DOWN_Pin,
//		.Port = BTN_DOWN_GPIO_Port,
//		.State = IDLE,
//		.Checked = 0
//};
//
//Button_t btn_in = {
//		.Pin = BTN_IN_Pin,
//		.Port = BTN_IN_GPIO_Port,
//		.State = IDLE,
//		.Checked = 0
//};
//
//Button_t btn_back = {
//		.Pin = BTN_BACK_Pin,
//		.Port = BTN_BACK_GPIO_Port,
//		.State = IDLE,
//		.Checked = 0
//};
//
//Ender_t PE = {
//		.Pin = ENDER_PE_Pin,
//		.Port = ENDER_PE_GPIO_Port,
//		.State = 0,
//		.gpio_out_Port = GPIOA,
//		.gpio_out_Pin = GPIO_OUT_PE_Pin
//};
//
//Ender_t ASF = {
//		.Pin = ENDER_LASER_Pin,
//		.Port = ENDER_LASER_GPIO_Port,
//		.State = 0,
//		.gpio_out_Port = GPIOA,
//		.gpio_out_Pin = GPIO_OUT_ASF_Pin
//};
//
//Ender_t Endstop = {
//		.Pin = ENDER_ENDSTOP_Pin,
//		.Port = ENDER_ENDSTOP_GPIO_Port,
//		.State = 0
//};
//
//Encoder_t Enc = {
//		.enc_counter = 0,
//		.enc_increment = 0,
//		.enc_prev = 0
//};
//
//Motor_t motor_vertical = {
//		.Tim = &htim15,
//		.Channel = TIM_CHANNEL_1,
//		.Dirrection = MOTOR_DIR_UP,
//		.STEP_Pin = MOTOR_B_STEP_Pin,
//		.STEP_Port = MOTOR_B_STEP_GPIO_Port,
//		.ENA_Pin = MOTOR_B_ENA_Pin,
//		.ENA_Port = MOTOR_B_ENA_GPIO_Port,
//		.DIR_Pin = MOTOR_B_DIR_Pin,
//		.DIR_Port = MOTOR_B_DIR_GPIO_Port,
//		.Enable = 0,
//		.Position = 0
//};
//
//Motor_t motor_horizontal = {
//		.Tim = &htim4,
//		.Channel = TIM_CHANNEL_4,
//		.Dirrection = MOTOR_DIR_FORWARD,
//		.STEP_Pin = MOTOR_A_STEP_Pin,
//		.STEP_Port = MOTOR_A_STEP_GPIO_Port,
//		.ENA_Pin = MOTOR_A_ENA_Pin,
//		.ENA_Port = MOTOR_A_ENA_GPIO_Port,
//		.DIR_Pin = MOTOR_A_DIR_Pin,
//		.DIR_Port = MOTOR_A_DIR_GPIO_Port,
//		.Enable = 0,
//};

//void init_components(void);
