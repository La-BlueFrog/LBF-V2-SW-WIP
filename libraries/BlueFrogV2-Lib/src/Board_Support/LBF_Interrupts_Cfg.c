/*******************************************************************************
 * LBF_Interrupts_Cfg.c
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

/* Init of hw required for various services e.g. TimerTick for delay function etc. */


#include "LBF_lowlev_API.h"

#include "IT_Priorities_UserDefinable.h"

/* Privates prototypes --------------------------------------------------*/
static void Assign_System_Interrupts(void);
static void Assign_OnChip_Interrupts();
static void Assign_OnBoard_Interrupts(void);
static void Assign_LBFext_Interrupts(void);
static void Configure_NVIC(void);

/* -----------------------------------------------------------------*/
void LBF_Interrupts_Cfg(void) 
{
        // start SYSCFG clock:
        //RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN, ENABLE);

  	/*** IRQs from STM32 core  */		
	Assign_System_Interrupts();

	/*** IRQs  from STM32 on-chip peripherals */
	Assign_OnChip_Interrupts();

	/*** IRQs from other sources on LBF board (through STM32 GPIO) */
	Assign_OnBoard_Interrupts();

     	/*** IRQs from sources external to LBF board 
             (through LBF connector then STM32 GPIO)  */
	Assign_LBFext_Interrupts();

	Configure_NVIC();
}


/* === Local Functions :=============================================*/


void Assign_System_Interrupts(void) 
    {

    /**  SysTick Interrupt       ****************************************/
    
    //--> Set SysTick for Interrupts every 1ms    
    // See Services.c for principle of operation of Delay funtionality based on SysTick 

    //SysTick_Config( (SystemCoreClock/1000) ) ; ===> Now handled in HAL_Init() section of LBF_BoardInit()
    //CMSIS function that will generate a system tick every 1000*1us = 1ms

	/* ADDITIONAL NOTE about SysTick Interrupts :
	Delay generation is based on a variable (sysTickCounter) decremented in interrupt 
	service routine (ISR) of SysTick. This implies that if Delay_ms() is called from 
	a peripheral ISR process, then the SysTick interrupt must have higher priority 
	(numerically lower) than the peripheral interrupt, otherwise the caller ISR 
	process will be	blocked. 
	Use NVIC_SetPriority() function to change the SysTick interrupt priority.*/
	/* The following post may also be helpful when tinkering with SysTick priority 
	and pre-emption level :
	https://my.st.com/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/Flat.aspx?RootFolder=https://my.st.com/public/STe2ecommunities/mcu/Lists/	cortex_mx_stm32/STM32%20Interrupt%20priorities%20and%20preemption&FolderCTID=0x01200200770978C69A1141439FE559EB459D7580009C4E14902C3CDE46A77F0FFD06506F5B&currentviews=25
	*/
    }

/* -----------------------------------------------------------------*/

void Assign_OnChip_Interrupts(void) 
    {
    }

/* -----------------------------------------------------------------*/

void Assign_OnBoard_Interrupts(void) 
    {
GPIO_InitTypeDef GPIO_InitStruct;

/* ==  Power Management  ===================== */
    // Power Off from Debounced Push-Button   
    // ONOFF_STAT (PC13) -  Input, weak pull-up
    // Suitable for active low Open-Drain IT
    GPIO_InitStruct.Pin = ONOFF_STAT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;  // active low
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ONOFF_STAT_PORT, &GPIO_InitStruct);


/* ==  LSM6DS3           ===================== */

#ifdef ENABLE_INT1_ACC_GYR 	// defined in IT_UserDefinable.h

    // INT1 from LSM6DS3 Accelero/Gyr
    // LSM6DS3_INT1 (PB15) -  Input, NO pull-up 
    // Following settings suitable for active HIGH push-pull IT (this is LSM6DS3default)
    // !! Must be tuned if LSM6DS3 is programmed differently
    GPIO_InitStruct.Pin = INT1_ACC_GYR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;  // active HIGH
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(INT1_ACC_GYR_PORT, &GPIO_InitStruct);

#endif


#ifdef ENABLE_INT2_ACC_GYR 	// defined in IT_UserDefinable.h

    // INT2 from LSM6DS3 Accelero/Gyr
    // LSM6DS3_INT2 (PC6) -  Input, NO pull-up 
    // Following settings suitable for active HIGH push-pull IT (this is LSM6DS3default)
    // !! Must be tuned if LSM6DS3 is programmed differently
    GPIO_InitStruct.Pin = INT2_ACC_GYR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;  // active HIGH
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(INT2_ACC_GYR_PORT, &GPIO_InitStruct);

#endif



/* ==  etc.           ===================== */
//TODO

    }

/* -----------------------------------------------------------------*/


void Assign_LBFext_Interrupts(void) 
    {
    }

/* -----------------------------------------------------------------*/

void Configure_NVIC(void) 
    {

       /* NOTE:
         - NVIC Priority Grouping has already been defined in
         HAL_Init() section of LBF_Board_Fixed_Inits().
         - SysTick Priority has been set at the same place
         (based in user choice defined in same .h as other IT, see below)
       */

       /* Note - Following assumes NVIC Priority Grouping = 4, 
          i.e. Preemption priority on 4 bits, Sub-priority on 0 bits (i.e. not used) */

        
       // Usage: 
       // HAL_NVIC_SetPriority( xxx_IRQn, <pre-emption level> , <subpriority always 0 here>); 


       // ===   EXTIx_IRQn  Interrupts : =========================================== */
       // These are triggered by pin "x" of ports GPIO.A/B/C/D when these are
       //  declared as external Interrupt source
       //  __EXTIx_IRQn_PRIO is user-defined in IT_Priorities_UserDefinable.h

       // EXTI0_IRQn 
	HAL_NVIC_SetPriority(EXTI0_IRQn, __EXTI0_IRQn_PRIO , 0); 
        //HAL_NVIC_EnableIRQ(EXTI0_IRQn);  !!!!!! NOT ENABLED for Now

       // EXTI1_IRQn 
	HAL_NVIC_SetPriority(EXTI1_IRQn, __EXTI1_IRQn_PRIO , 0); 
        //HAL_NVIC_EnableIRQ(EXTI1_IRQn);  !!!!!! NOT ENABLED for Now

       // EXTI2_IRQn 
	HAL_NVIC_SetPriority(EXTI2_IRQn, __EXTI2_IRQn_PRIO , 0); 
        //HAL_NVIC_EnableIRQ(EXTI2_IRQn);  !!!!!! NOT ENABLED for Now

       // EXTI3_IRQn 
	HAL_NVIC_SetPriority(EXTI3_IRQn, __EXTI3_IRQn_PRIO , 0); 
        //HAL_NVIC_EnableIRQ(EXTI3_IRQn);  !!!!!! NOT ENABLED for Now

       // EXTI4_IRQn 
	HAL_NVIC_SetPriority(EXTI4_IRQn, __EXTI4_IRQn_PRIO , 0); 
        //HAL_NVIC_EnableIRQ(EXTI4_IRQn);  !!!!!! NOT ENABLED for Now

       // EXTI9_5_IRQn 
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, __EXTI9_5_IRQn_PRIO , 0); 
        //HAL_NVIC_EnableIRQ(EXTI9_5_IRQn); 

        // EXTI15_10_IRQn 		
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, __EXTI15_10_IRQn_PRIO , 0); 
        HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); 



       // ===   On-Chip  Interrupts : =========================================== */




    }


/***************************************************************END OF FILE****/
