/*******************************************************************************
 * LBF_lowlev_API.h
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


/* Define to prevent recursive inclusion --------------*/
#ifndef __LBF_LOWLEV_API_H
#define __LBF_LOWLEV_API_H 

#include "stm32l1xx_hal.h"  //STM32 HAL Drivers 
 // pulls stm32l1xx_hal_conf.h --> stm32l1xx_hal_<ppp>.h --> .stm32l1xx_hal_def.h --> stm32l1xx.h

#include "LBF_lsm6ds3.h"

#include "global_variables.h"
#include "custom_types.h"
#include "pin_aliases.h"
#include "OnBoard_chip_aliases.h"
#include "Services.h"
#include "LBF_FLASH_lowlevAPI.h"
#include "LBF_GPIO_lowlevAPI.h"
#include "LBF_LED_Switches_lowlevAPI.h"
#include "LBF_OLED_lowlevAPI.h"
#include "LBF_PWR_lowlevAPI.h"
#include "LBF_I2C2_lowlevAPI.h"
#include "LBF_UART_lowlevAPI.h"
#include "LBF_Timer_lowlevAPI.h"
#include "LBF_ExtIT_lowlevAPI.h"

#endif /* __LBF_LOWLEV_API_H */

/*********************************END OF FILE****/

