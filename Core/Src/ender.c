/*
 * ender.c
 *
 *  Created on: May 15, 2024
 *      Author: kate
 */

#include "ender.h"

void turn_on(Ender_t obj){
	obj.State = 1;
	obj.Checked = 1;
	HAL_GPIO_WritePin(obj.gpio_out_Port, obj.gpio_out_Pin, obj.State);
}


void turn_off(Ender_t obj){
	obj.State = 0;
	obj.Checked = 0;
	HAL_GPIO_WritePin(obj.gpio_out_Port, obj.gpio_out_Pin, obj.State);
}

