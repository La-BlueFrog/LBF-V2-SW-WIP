/*******************************************************************************
 * LBF_PWM_lowlevAPI.c
 * 
 * 
 * Author: La BlueFrog, 2015
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 ******************************************************************************/

#include "LBF_lowlev_API.h"

#include <stdint.h>


/*******************************************************************************
 * @brief  : Sets Period of a PWM source
 * @param  : Pwm_ID (TIMER to use), Period in us
 * @retval : none.
 ******************************************************************************/
void LBF_PWM_SetPeriod_us (Pwm_ID_t Pwm_ID, uint32_t Period_us)
{

  if (Pwm_ID == PWM4)
  {
     __HAL_TIM_SetAutoreload( &htim4, Period_us);  
	// Update value of Reload Register that defines PWM period / frequency

/*
    htim4.Init.Period = Period_us - 1 ;  // Sets period of Timer 4
         // Formula: PrescaledClock / TargetPWM Clock -1, and here Prescaled Clock set to 1MHz at init
         // all other parameters unchanged vs what's in LBF_PWM4_Ch3-4_Init()
    HAL_TIM_PWM_Init(&htim4);
*/
  }

//TODO Other available PWM sources on connector
 
}


/*******************************************************************************
 * @brief  : Sets duty Cycle of a PWM source; Period must have been previously set
 *           using LBF_PWM_SetPeriod. ONE SINGLE PERIOD FOR ALL CHANNELS OF SAME TIMER. 
 * @param  : Pwm_ID_Chan (TIMER and CHANNEL to use), High Pulse in us.
 * @retval : none.
 ******************************************************************************/
void LBF_PWM_SetPulse_us (Pwm_ID_Chan_t Pwm_ID_Chan, uint32_t  Pulse_us)
{

  if (Pwm_ID_Chan == PWM4_CH3)
  {
     __HAL_TIM_SetCompare( &htim4, TIM_CHANNEL_3, Pulse_us);  
	// Update value of Compare Registers that defines PWM pulse duration
/*
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = Pulse_us ;  // Defines Duty Cycle : Pulse_us / Period_us
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3);
*/
  }

  if (Pwm_ID_Chan == PWM4_CH4)
  {
     __HAL_TIM_SetCompare( &htim4, TIM_CHANNEL_4, Pulse_us);  
  }
}

/*******************************************************************************
 * @brief  : Start a PWM source
 * @param  : Pwm_ID_Chan (TIMER and CHANNEL to use)
 * @retval : none.
 ******************************************************************************/
void LBF_PWM_Start (Pwm_ID_Chan_t Pwm_ID_Chan)
{

  if (Pwm_ID_Chan == PWM4_CH3)
  {
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  }

  if (Pwm_ID_Chan == PWM4_CH4)
  {
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
  }

}

/*******************************************************************************
 * @brief  : Stop a PWM source
 * @param  : Pwm_ID_Chan (TIMER and CHANNEL to use)
 * @retval : none.
 ******************************************************************************/
void LBF_PWM_Stop (Pwm_ID_Chan_t Pwm_ID_Chan)
{

  if (Pwm_ID_Chan == PWM4_CH3)
  {
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
  }

  if (Pwm_ID_Chan == PWM4_CH4)
  {
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);
  }

}

/***************************************************************END OF FILE****/
