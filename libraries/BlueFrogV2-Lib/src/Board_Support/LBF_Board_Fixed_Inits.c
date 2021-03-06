/*******************************************************************************
 * LBF_Board_Init.c
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


#include "LBF_Board_Fixed_Inits.h"

#include "LBF_lowlev_API.h"

#include "LBF_SysClkCfg.h"

#include "LBF_LSE_RTC_Cfg.h"

#include "LBF_I2C2_Init.h"
#include "LBF_SPI1_Init.h"
//#include "LBF_SPI2_Init.h"
#include "LBF_SPI3_Init.h"
#include "LBF_UART1_Init.h"
//#include "LBF_USART2_Init.h"

#include "LBF_I2C2_IOcfg.h"
#include "LBF_SPI1_IOcfg.h"
#include "LBF_SPI2_IOcfg.h"
#include "LBF_SPI3_IOcfg.h"
#include "LBF_UART1_IOcfg.h"
#include "LBF_USART2_IOcfg.h"

//#include "LBF_BATT_IOcfg.h"
#include "LBF_FLASH_IOcfg.h"
#include "LBF_LED+Switches_IOcfg.h"
#include "LBF_PWR_IOcfg.h"
#include "LBF_OLED_IOcfg.h"
#include "LBF_BTLE_IOcfg.h"
#include "LBF_PWR_IOcfg.h"
#include "LBF_FLASH_Init.h"

//#include "LBF_OLED_Init.h"

#include "User_Configuration.h"
//#include "LBF_Conn_Cfg_UserDefd.h"  
#include "IT_Priorities_UserDefinable.h"



/* Privates prototypes -------------------------------------------------------*/
static void HAL_Init_Private(void);


/* Functions -----------------------------------------------------------------*/


/*******************************************************************************/

/** Full Board Initializations and Configurations          **/
/** except OLED VDDH powering and init, handled separately **/
/** (as OLED may or may not be present)                    **/


void LBF_Board_Fixed_Inits(void) 
{

/* -------- Configure eFlash Prefetch,        ---------------- */
/* --       Init and launch SysTick time base ---------------- */
/* --       Set NVIC for pre-emption only (no sub-prio)    --- */
    
     HAL_Init_Private();


/* -------- Configure system clocks with default values  -------*/
/* --       User may override this in his own code if desired   */
/* -- NOTE: LSE not handled here (long start-up not compatible  */
/* --       with max latency to acknowledge PMIC after power-up)*/

     LBF_SysClkCfg();  



/* --------  STM32 IO Configuration  ------------------------  */

    /* First, enable GPIO clocks */
     __GPIOA_CLK_ENABLE();
     __GPIOB_CLK_ENABLE();
     __GPIOC_CLK_ENABLE();
     __GPIOD_CLK_ENABLE();


				// PB6, PC0, PC2, PC13
     LBF_PWR_IOcfg();  		// PMIC IO interfacing


				// SPI3 - PB3, PB4, P5
     LBF_SPI3_IOcfg();   	// interfacing with: Data Flash


				// USART2 - PA2, PA3, PA4
     LBF_USART2_IOcfg();   	// interfacing with: OLED


				// SPI1 - Pins PA5, PA6, PA7 (excl. nCS)
     LBF_SPI1_IOcfg();		// interface to OLED as back-up of USART2
				// !!! Special temporary arrangement


   			        // UART1 - Pins PA9, PA10
     LBF_UART1_IOcfg();		// interfacing with: BTLE


				// PC7, PC1
//TODO     LBF_BATT_IOcfg();   	// Battery VBat: measurement enable and analog voltage pins

				// PC3 
     LBF_LED_IOcfg();  		// LED IO interfacing
				// PA15, PC8


     LBF_SelSwitches_IOcfg();   // Selection switches IO interfacing


     				// I2C2 - Pins PB10, PB11
     LBF_I2C2_IOcfg();   	// interfacing with: all sensors 
				//   + optionally BTLE


				// PB7
     LBF_FLASH_IOcfg();  	// Data Flash IO interfacing


				// PA1, PB1, PC4, PC5,
     LBF_OLED_IOcfg();          // OLED IO interfacing
				// PA1= Oled orientation - here or with PowerCfg?


				// PC9
     LBF_BTLE_IOcfg();  	// BlueTooth Low-Energy IO interfacing

			

     // LBF_USB_IOcfg() (PA11, PA12) Not Needed
     // USB DM/DP already configured by default on STM32L1

     // Other pins that don't need configuration:
     // Boot0; PA8, PD2: unused; PA13/PA14=SWD i/f; PB2=Boot1; NRST; 
     // PC14/PC15=Xtal 32KHz; PH0/PH1_Osc_In/Out=8MHz



/* -------- On-Chip Peripheral Inits  -----------------------  */  
    

    /* Always-enabled peripherals : */

     LBF_SPI1_Init();  // used by OLED as back-up alternative to USART

     LBF_SPI3_Init();  // used by DataFlash

     LBF_I2C2_Init();  // used by all sensors (+ optionnally BTLE) 

     LBF_LSE_RTC_Cfg(); // External 32KHz (Low-Speed External, LSE) init
		       // used by RTC (NB: osc. start-up takes some time)


// TBD: USART2 (OLED / ext conn)



/* --------  Inits required by other on-board chips   ----------  */


     LBF_FLASH_Init();    // ADESTO Data Flash
 

}


/*******************************************************************************/

/********************************************************************
 * HAL_Init_Private():    
 *     In-lining of HAL_Init() with custom SysTick priority, 
 *     defined by _SysTick_IRQn_PRIO                        
 ********************************************************************/
/* @brief This function configures the Flash prefetch, 

 *        Configures time base source and NVIC   
 * @note This function is called at the beginning of program after reset and before 
 *       the clock configuration
 * @note The time base configuration is based on MSI clock when exiting from Reset.
 *       Once done, time base tick start incrementing.  
 *        In the default implementation,Systick is used as source of time base.

 *        the tick variable is incremented each 1ms in its ISR.
 */
 
void HAL_Init_Private(void) 
{

#if (PREFETCH_ENABLE != 0)   
     __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
#endif

     HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
     	    // Leads to 4 bits to define preemption priority 
            // and 0 for sub-priority (i.e., not used)

     HAL_InitTick(__SysTick_IRQn_PRIO);   
    	    // Use SysTick as time base source and configure 1ms tick 
            // __SysTick_IRQn_PRIO is defined in UserConfiguration file
            // Caution: SysTick prio must be numerically lower (higher pre-emption)
            //          than any ISR making use of SysTick - else will hang !

     // HAL_MspInit() not called as empty anyway - not used 
     // (IO config and periph init done in LBF_Board_Fixed_Inits(), above )

}




/***************************************************************END OF FILE****/
