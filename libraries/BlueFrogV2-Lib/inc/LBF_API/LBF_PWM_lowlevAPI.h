/*******************************************************************************
 * LBF_PWM_lowlevAPI.h
 * 
 * Author: La BlueFrog, 2015
 *
 * This program is free software; you can redistribute it and/or modify
 * t under the terms of the GNU General Public License as published by
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LBF_PWM_LOWLEVAPI_H
#define __LBF_PWM_LOWLEVAPI_H


/* Constants        ---------------------------------------------------------*/


/* Custom Types      ---------------------------------------------------------*/
typedef enum {
  PWM4 = 4
}    
Pwm_ID_t;

typedef enum {
  PWM4_CH3 = 43,
  PWM4_CH4 = 44
}    
Pwm_ID_Chan_t;


/* Exported functions ------------------------------------------------------- */

void LBF_PWM_SetPeriod_us (Pwm_ID_t Pwm_ID, uint32_t Period_us);
void LBF_PWM_SetPulse_us (Pwm_ID_Chan_t Pwm_ID_Chan, uint32_t  Pulse_us);
void LBF_PWM_Start (Pwm_ID_Chan_t Pwm_ID_Chan);
void LBF_PWM_Stop (Pwm_ID_Chan_t Pwm_ID_Chan);


#endif  /*__LBF_PWM_LOWLEVAPI_H*/

/***************************************************************END OF FILE****/
