/*******************************************************************************
 * LBF_I2C2_lowlevAPI.h
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



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LBF_I2C2_LOWLEVAPI_H
#define __LBF_I2C2_LOWLEVAPI_H

#include "custom_types.h"  //to use type boolean_t in below prototype and keep this header file self-sufficient
#include <stdint.h>        //same rationale, to use uint8_t below


//#include "pin_aliases.h"

/* Exported constants --------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */


void I2C2_WriteSingleReg (uint8_t ChipID, uint16_t RegAdd, uint8_t RegVal);
void  I2C2_WriteMultipleReg (uint8_t ChipID, uint16_t RegAdd, uint8_t* pVal, uint16_t NumByteToWrite );

uint8_t I2C2_ReadSingleReg (uint8_t ChipID, uint16_t RegAdd);
void I2C2_ReadMultipleReg (uint8_t ChipID, uint16_t RegAdd, uint8_t* pVal, uint16_t NumByteToRead );

void I2C2_RmodWSingleReg (uint8_t ChipID, uint16_t RegAdd, uint8_t RegMask, uint8_t RegUpdateVal);


#endif  /*__LBF_I2C2_LOWLEVAPI_H*/

/***************************************************************END OF FILE****/
