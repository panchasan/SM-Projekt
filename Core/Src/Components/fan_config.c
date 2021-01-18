/*
 * fan_config.c
 *
 *  Created on: Jan 18, 2021
 *      Author: MacBook (Ziober)
 */


/*Includes*/

#include "Components/fan.h"
#include "Components/fan_config.h"
#include "main.h"
#include "tim.h"

/*Inicjalizacja grzałki*/
FAN_HandleTypeDef hfan1= {
  .Timer = &htim3,
  .Channel = TIM_CHANNEL_3, //PC8
  .Duty = 0
};
