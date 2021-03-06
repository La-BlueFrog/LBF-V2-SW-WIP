/*******************************************************************************
 * OnBoard_chip_aliases.h
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

#ifndef __ONBOARD_CHIP_ALIASES_H
#define __ONBOARD_CHIP_ALIASES_H


/**  EXTERNAL CHIP ID AND REGISTER   A L I A S E S     ****************/

// Magnetometer :  ST LIS3MDL
#define LIS3MDL_CHIPID	0x1C 
#define LIS3MDL_WHOAMI	0x0F	
#define LIS3MDL_WHOAMI_CONTENTS	0x3D	

// Accelerometer/Gyro :  ST LSM6DS3
#define LSM6DS3_CHIPID	0x6A 
#define LSM6DS3_WHOAMI	0x0F	
#define LSM6DS3_WHOAMI_CONTENTS	0x69

// ALS/Proximity : ST VL6180X
#define VL6180X_CHIPID	0x29 
#define VL6180X_WHOAMI	0x00
#define VL6180X_WHOAMI_CONTENTS	0xB4	

// Pressure/Temp Sensor : ST LPS25H
#define LPS25H_CHIPID	0x5C 
#define LPS25H_WHOAMI	0x0F	
#define LPS25H_WHOAMI_CONTENTS	0xBD


#endif /*__ONBOARD_CHIP_ALIASES_H*/

/***************************************************************END OF FILE****/
