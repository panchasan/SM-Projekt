/*
 * heater_config.c
 *
 *  Created on: Jan 18, 2021
 *      Author: MacBook (Ziober)
 */

/*Includes*/

#include "Components/heater.h"
#include "Components/heater_config.h"
#include "main.h"
#include "tim.h"

/*Inicjalizacja grzałki*/
HEATER_HandleTypeDef hheater1 = {
  .Timer = &htim3,
  .Channel = TIM_CHANNEL_4, //PB1
  .Duty = 0
};
