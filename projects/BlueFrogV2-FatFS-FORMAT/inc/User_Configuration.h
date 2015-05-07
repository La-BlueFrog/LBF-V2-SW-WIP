/*******************************************************************************
 * User_Configuration.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_CONF_H
#define __USER_CONF_H



// ============================================================================
//    OLED presence
// ============================================================================

// Comment out or uncomment next line depending on OLED presence and usage :
// #define USE_OLED  
   // WARNING:
   // - do not keep uncommented if no OLED present
   // - this initializes OLED but powering on/off the screen is left to user 
   //   code, using e.g. functions OLED_Switch_ON() and OLED_Switch_OFF()


// ============================================================================
//    Enable/Disable BlueTooth Low-Energy Functionality
// ============================================================================

// Comment out / Uncomment next line as needed :
// #define ENABLE_BTLE

// Select here the required code to download in the BTLE module 
// Must be stored on the Data Flash - Would typically be put there 
// by USB 
// Ignored if ENABLE_BTLE is undefined
#define  BTLE_CODE_FILENAME   "BTLECODE.BIN"

// indicate here required baudrate in bits/s
// This is for exchanges between STM32 and application running on PAN1740
// Does NOT affect baud rate used for booting PAN1740 from UART (which must not be changed)
// Ignored if BTLE is not enabled
#define	UART1_BAUDRATE	 115200  	






// ============================================================================
//    UART3 Usage on External Connector
// ============================================================================

// Comment out / Uncomment each of next 2 lines as needed :
// --------------------------------------------------------
// #define POS5_IS_UART3_TX 	//defined means Position 5 bears USART3 Tx
// #define POS6_IS_UART3_RX 	//defined means Position 6 bears USART3 Rx



// Assign required values (no effect if no pin enables as UART3 above) :
// ---------------------------------------------------------------------
#define	UART3_BAUDRATE	 57600	// indicate here required baudrate in bits/s

// Other UART parameters default to :
//   8 bit data, 1 Stop bit, no Parity bit ...
// Tune code if you need something different


// ============================================================================
//    xxxxx
// ============================================================================

// etc.
// don't forget ADC options etc.




// ========  

// Conifgurable Interrupt Mapping and Interrupt Priorities should probably come here too... (cf InterruptsCfg.h)


#endif  /*__USER_CONF_H*/

/***************************************************************END OF FILE****/

