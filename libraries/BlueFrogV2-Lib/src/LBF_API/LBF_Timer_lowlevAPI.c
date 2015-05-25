/*******************************************************************************
 * LBF_Timer_lowlevAPI.c
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

#include "IT_Priorities_UserDefinable.h"


TIM_HandleTypeDef htim2; 
TIM_HandleTypeDef htim3; 
TIM_HandleTypeDef htim4;  
TIM_HandleTypeDef htim5;  
TIM_HandleTypeDef htim6;   
TIM_HandleTypeDef htim7;  
TIM_HandleTypeDef htim9; 
TIM_HandleTypeDef htim10;   
TIM_HandleTypeDef htim11;   


// ----------------------------------------------------------------------------
//
// Note- STM32L1 has the following timers:
//   TIM2,3,4 (general-purpose, 16 bits) + TIM5 (general-purpose, 32 bits) +
//   TIM6,7 (basic) + TIM9 (2-channel) + TIM10-11 (1-channel)
// !!! Caution: may be different when switching to L4 !!!
//
// NOTE: Only TIM5 counts on 32 bits,others on 16 bits
//
// ----------------------------------------------------------------------------




/*******************************************************************************
 * @brief  : Set-up the timer indicated by caller
 * @param  : TimerID: timer to use
 * @param  : Units_in_us: Units (expressed as multiple of 1us) in which Period will be expressed (=Prescaled clock period)
 * @param  : Period_in_units: Timer Period (i.e Counter Reload Valu) expressed in above unit
 * @retval : none.
 ******************************************************************************/

void LBF_Timer_Setup( TimerID_t TimerID, uint16_t Timer_TimeUnit_us, uint32_t Period_as_TimerUnits )
{
TIM_HandleTypeDef htim;  // handle used by TIM HAL functions 
uint32_t TimerClockFqcy;

  // Select Right Timer and make sure associated handle will
  //  reflect what's done with local handle here

  switch( TimerID )
  {
    case TIMER2:
        __TIM2_CLK_ENABLE();
  	htim.Instance = TIM2;
        htim2 = htim;  // "shallow" copy i.e. pointer is copied (both point to same data)
        break;
    case TIMER3:
        __TIM3_CLK_ENABLE();
  	htim.Instance = TIM3;
        htim3 = htim;
        break;
    case TIMER4:
        __TIM4_CLK_ENABLE();
  	htim.Instance = TIM4;	
        htim4 = htim;
        break;
    case TIMER5:
        __TIM5_CLK_ENABLE();
  	htim.Instance = TIM5;	
        htim5 = htim;
        break;

    case TIMER6:
        __TIM6_CLK_ENABLE();
  	htim.Instance = TIM6;	
        htim6 = htim;
        break;
    case TIMER7:
        __TIM7_CLK_ENABLE();
  	htim.Instance = TIM7;	
        htim7 = htim;
        break;
 
    case TIMER9:
        __TIM9_CLK_ENABLE();
  	htim.Instance = TIM9;
        htim9 = htim;
        break;
 
    case TIMER10:
        __TIM10_CLK_ENABLE();
  	htim.Instance = TIM10;	
        htim10 = htim;
        break;
    case TIMER11:
        __TIM11_CLK_ENABLE();
  	htim.Instance = TIM11;	
        htim11 = htim;
        break;

    default: break;
  }

  // Get Peripheral Clock Frequency
  if ( (TimerID == 9) || (TimerID == 10) || (TimerID == 11) )
  {  // then timer clock is APB2
  	TimerClockFqcy = HAL_RCC_GetPCLK2Freq();
        // Caution: TIMnCLK is 1x PCLK2 if APB2 prescaler=1 else is 2x -- see STM32 UserMan
  }
  else
  {  // else timer clock is APB1
  	TimerClockFqcy = HAL_RCC_GetPCLK1Freq();
        // Caution: TIMnCLK is 1x PCLK1 if APB1 prescaler=1 else is 2x -- see STM32 UserMan
  }

  // Configure the TIM peripheral Time Base
  htim.Init.Prescaler = (TimerClockFqcy/1000000)*Timer_TimeUnit_us - 1; // so prescaled clock period = Timer_TimeUnit_us micro-seconds
  htim.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim.Init.Period = Period_as_TimerUnits -1 ;  // specified by caller
  htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // ie no division
  HAL_TIM_Base_Init(&htim);

}


/*******************************************************************************
 * @brief  : Start the timer and optionally generate an interrupt when elapsed
 * @param  : TimerID: timer to use; Want_IRQ: set to TRUE to generate an IT, else FALSE
 * @retval : none.
 ******************************************************************************/

void LBF_Timer_Start_ITout( TimerID_t TimerID )
{
TIM_HandleTypeDef htim;  // handle used by TIM HAL functions 
IRQn_Type  IRQnum;
uint32_t   IRQprio;

  // Select Right Timer 
  switch( TimerID )
  {
    case TIMER2:
        IRQnum = TIM2_IRQn;
        IRQprio = __TIM2_IRQn_PRIO;
        htim = htim2;
        break;
    case TIMER3:
        IRQnum = TIM3_IRQn;
        IRQprio = __TIM3_IRQn_PRIO;
        htim = htim3;
        break;
    case TIMER4:
        IRQnum = TIM4_IRQn;
        IRQprio = __TIM4_IRQn_PRIO;
        htim = htim4;
        break;
    case TIMER5:
        IRQnum = TIM5_IRQn;
        IRQprio = __TIM5_IRQn_PRIO;
        htim = htim5;
        break;

    case TIMER6:
        IRQnum = TIM6_IRQn;
        IRQprio = __TIM6_IRQn_PRIO;
        htim = htim6;
        break;
    case TIMER7:
        IRQnum = TIM7_IRQn;
        IRQprio = __TIM7_IRQn_PRIO;
        htim = htim7;
        break;

    case TIMER9:
        IRQnum = TIM9_IRQn;
        IRQprio = __TIM9_IRQn_PRIO;
	htim = htim9;
        break;

    case TIMER10:
        IRQnum = TIM10_IRQn;
        IRQprio = __TIM10_IRQn_PRIO;
        htim = htim10;
        break;

    case TIMER11:
    default :  // to avoid warning "IRQnum may be used initialized in this fucntion"
        IRQnum = TIM11_IRQn;
        IRQprio = __TIM11_IRQn_PRIO;
        htim = htim11;
        break;
  }

  // Start Time Base Generation, in interrupt mode 
    HAL_TIM_Base_Start_IT(&htim);
    HAL_NVIC_SetPriority(IRQnum, IRQprio , 0); 
    HAL_NVIC_EnableIRQ(IRQnum); 

}


/*******************************************************************************
 * @brief  : Stop the timer (IT gets disabled)
 * @param  : TimerID: timer to use
 * @retval : none.
 ******************************************************************************/

void LBF_Timer_Stop( TimerID_t TimerID )
{
// We disable using HAL_TIM_Base_Stop_IT 
// even when TIM was starting without IT generation
// Not a pb as this simply means we're going to disable an
// IT generation flag that was not set anyway...

  switch( TimerID )
  {
    case TIMER2:
    	HAL_TIM_Base_Stop_IT(&htim2);
        break;
    case TIMER3:
    	HAL_TIM_Base_Stop_IT(&htim3);
        break;
    case TIMER4:
    	HAL_TIM_Base_Stop_IT(&htim4);
        break;
    case TIMER5:
    	HAL_TIM_Base_Stop_IT(&htim5);
        break;

    case TIMER6:
    	HAL_TIM_Base_Stop_IT(&htim6);
        break;
    case TIMER7:
    	HAL_TIM_Base_Stop_IT(&htim7);
        break;

    case TIMER9:
    	HAL_TIM_Base_Stop_IT(&htim9);
        break;

    case TIMER10:
    	HAL_TIM_Base_Stop_IT(&htim10);
        break;

    case TIMER11:
    	HAL_TIM_Base_Stop_IT(&htim11);
        break;

    default: break;
  }
 

}


/*
// TODO ?
void LBF_Timer_Freeze( );
void LBF_Timer_Continue( );
*/
// Caution: if trying to implement eg. a Timer_Update_Period() function that uses
// macro __HAL_TIM_SetAutoreload to directly access the reload register...
// ...if programming a smaller value than current while counter is already past that value, 
// unpredictable behavior occurs (looks like counter is directly written rather than updated at next
// roll-over



/*=======  PWM Specifics ========================================================*/

// !!!!!!!!!!!!!!!!!!!!!!!
// Here we only deal with PWM that can be made available on the extension connector
// of La Blue Frog. Namely:
// - TIMER4, Channels 3 and 4 (on connector positions 9 and 10)
// - TIMER3, Channels 1 and 3 (on connector positions 2 and 3)
// - TIMER10, Channel 1 (on connector position 2)
// - TIMER11, Channel 1 (on connector position 3)
// !!!!!!!!!!!!!!!!!!!!!!!


/*******************************************************************************
 * @brief  : Initialise and configure a PWM channel of a timer 
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void LBF_PWMchannel_Setup ( TimerID_t TimerID, ChannelID_t ChannelID, uint32_t Pulse_as_TimerUnits )
{
// Note: Timer (TIM) peripheral (i.e. time base) must have been setup first !

TIM_OC_InitTypeDef sConfigOC; 

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = Pulse_as_TimerUnits ; 
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

  if (TimerID == TIMER4)
  {
      if (ChannelID == CHANNEL3)
      {
	  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3);  
      }
      if (ChannelID == CHANNEL4)
      {
	  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4);  
      }
  }
  if (TimerID == TIMER3)
  {
      if (ChannelID == CHANNEL1)
      {
	  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);  
      }
      if (ChannelID == CHANNEL3)
      {
	  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);  
      }
  }
  if (TimerID == TIMER10)
  {
      if (ChannelID == CHANNEL1)
      {
	  HAL_TIM_PWM_ConfigChannel(&htim10, &sConfigOC, TIM_CHANNEL_1);  
      }
  }
  if (TimerID == TIMER11)
  {
      if (ChannelID == CHANNEL1)
      {
	  HAL_TIM_PWM_ConfigChannel(&htim11, &sConfigOC, TIM_CHANNEL_1);  
      }
  }
}
/*******************************************************************************
 * @brief  : Sets duty Cycle of a PWM source; Period must have been previously set
 *           using LBF_PWM_SetPeriod. ONE SINGLE PERIOD FOR ALL CHANNELS OF SAME TIMER. 
 * @param  : Pwm_ID_Chan (TIMER and CHANNEL to use), High Pulse in us.
 * @retval : none.
 ******************************************************************************/

void LBF_PWMChannel_UpdatePulse (TimerID_t TimerID, ChannelID_t ChannelID, uint32_t  Pulse_as_TimerUnits)
{

  if (TimerID == TIMER4)
  {
      if (ChannelID == CHANNEL3)
      {
         __HAL_TIM_SetCompare( &htim4, TIM_CHANNEL_3, Pulse_as_TimerUnits-1);  
         // Update value of Compare Registers that defines PWM pulse duration
      }
      if (ChannelID == CHANNEL4)
      {
         __HAL_TIM_SetCompare( &htim4, TIM_CHANNEL_4, Pulse_as_TimerUnits-1);  
	 // Update value of Compare Registers that defines PWM pulse duration
      }
  }
  if (TimerID == TIMER3)
  {
      if (ChannelID == CHANNEL1)
      {
         __HAL_TIM_SetCompare( &htim3, TIM_CHANNEL_1, Pulse_as_TimerUnits-1);  
         // Update value of Compare Registers that defines PWM pulse duration
      }
      if (ChannelID == CHANNEL3)
      {
         __HAL_TIM_SetCompare( &htim3, TIM_CHANNEL_3, Pulse_as_TimerUnits-1);  
	 // Update value of Compare Registers that defines PWM pulse duration
      }
  }
  if (TimerID == TIMER10)
  {
      if (ChannelID == CHANNEL1)
      {
         __HAL_TIM_SetCompare( &htim10, TIM_CHANNEL_1, Pulse_as_TimerUnits-1);  
         // Update value of Compare Registers that defines PWM pulse duration
      }
  }
  if (TimerID == TIMER11)
  {
      if (ChannelID == CHANNEL1)
      {
         __HAL_TIM_SetCompare( &htim11, TIM_CHANNEL_1, Pulse_as_TimerUnits-1);  
         // Update value of Compare Registers that defines PWM pulse duration
      }
  }
}

/*******************************************************************************
 * @brief  : Start a PWM source
 * @param  : TimerID, ChannelID (TIMER and CHANNEL to use)
 * @retval : none.
 ******************************************************************************/
void LBF_PWMChannel_Start (TimerID_t TimerID, ChannelID_t ChannelID)
{
  if (TimerID == TIMER4)
  {
      if (ChannelID == CHANNEL3)
      {
          HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
      }
      if (ChannelID == CHANNEL4)
      {
          HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
      }
  }
  if (TimerID == TIMER3)
  {
      if (ChannelID == CHANNEL1)
      {
         HAL_TIM_PWM_Start( &htim3, TIM_CHANNEL_1);  
      }
      if (ChannelID == CHANNEL3)
      {
         HAL_TIM_PWM_Start( &htim3, TIM_CHANNEL_3);  
      }
  }
  if (TimerID == TIMER10)
  {
      if (ChannelID == CHANNEL1)
      {
         HAL_TIM_PWM_Start( &htim10, TIM_CHANNEL_1);  
      }
  }
  if (TimerID == TIMER11)
  {
      if (ChannelID == CHANNEL1)
      {
         HAL_TIM_PWM_Start( &htim11, TIM_CHANNEL_1);  
      }
  }
}

/*******************************************************************************
 * @brief  : Stop a PWM source
 * @param  : TimerID, ChannelID (TIMER and CHANNEL to use)
 * @retval : none.
 ******************************************************************************/
void LBF_PWMChannel_Stop (TimerID_t TimerID, ChannelID_t ChannelID)
{
  if (TimerID == TIMER4)
  {
      if (ChannelID == CHANNEL3)
      {
          HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
      }
      if (ChannelID == CHANNEL4)
      {
          HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);
      }
  }
  if (TimerID == TIMER3)
  {
      if (ChannelID == CHANNEL1)
      {
         HAL_TIM_PWM_Stop( &htim3, TIM_CHANNEL_1);  
      }
      if (ChannelID == CHANNEL3)
      {
         HAL_TIM_PWM_Stop( &htim3, TIM_CHANNEL_3);  
      }
  }
  if (TimerID == TIMER10)
  {
      if (ChannelID == CHANNEL1)
      {
         HAL_TIM_PWM_Stop( &htim10, TIM_CHANNEL_1);  
      }
  }
  if (TimerID == TIMER11)
  {
      if (ChannelID == CHANNEL1)
      {
         HAL_TIM_PWM_Stop( &htim11, TIM_CHANNEL_1);  
      }
  }

}


/***************************************************************END OF FILE****/
