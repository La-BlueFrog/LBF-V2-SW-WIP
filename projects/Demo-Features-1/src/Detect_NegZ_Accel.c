/*******************************************************************************
 * Detect_NegZ_Accel.c
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
 *************************************************************************/


#include "LBF_Global.h"
#include "User_Configuration.h"


boolean_t Detect_NegZ_Accel(void)
{

int8_t  Z_Accel_HighByte;
boolean_t  ChangeRequested ;


    while ( (I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x1E) & 0x01) == 0x00);
            // Status Reg: Wait for XLDA bit to be set


    Z_Accel_HighByte =  (int8_t) I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x2D);   

    ChangeRequested = ( Z_Accel_HighByte < 0 ); // (shake or upside down)

    return ChangeRequested ;
}  




/***************************************************************END OF FILE****/// 
