/**
  ******************************************************************************
  * File Name          : main.c
  * Date               : 25/11/2014 11:44:33
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2014 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN 0 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "x_nucleo_ikp01a1.h"
#include "x_nucleo_ikp01a_display.h"
#include "vl6180x_api.h"

#define SWITCH_VAL_RANGING  0

enum runmode_t{
    RunRangePoll=0,
    RunAlsPoll,
    InitErr,
    ScaleSwap,
};
/* USER CODE END 0 */



int main(void)
{

  /* USER CODE BEGIN 1 */
 int status;
  //int range=10000;
  int mode=-1;
  int switch_state = -1;
  int new_switch_state;
	 int loop= 0;
  int uptime=1;
  int measurementCycle = 2/uptime*15;
   VL6180x_RangeData_t Range;
   VL6180x_AlsData_t Als;
 
    uint8_t IntStatus;
    

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* System interrupt init*/
  /* Sets the priority grouping field */
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
 

  /* USER CODE BEGIN 2 */
  VL6180x_Shield_I2C1_Init(&hi2c1); 
	init_display();
	display_8888(300);
	
	VL6180x_Shield_Reset(1);
  WaitMilliSec(1);
	
  VL6180x_WaitDeviceBooted(theVL6180xDev);
  VL6180x_InitData(theVL6180xDev);
	VL6180x_StaticInit(theVL6180xDev);
	/* USER CODE END 2 */

  /* USER CODE BEGIN 3 */
  /* Infinite loop */
  while (1)
  {
{
      new_switch_state = VL6180x_Shield_GetSwitch();
       if( new_switch_state!= switch_state )
           {
            /* stop but ensure some delay fo rdevcie to stop */
            switch_state = new_switch_state;
            
            if( switch_state == SWITCH_VAL_RANGING )
                {
								
                status=VL6180x_Prepare(theVL6180xDev);
								
                if(status )
                    {
                    display_err(1000);
                    mode = InitErr;
                    }
                else
                    {
                    mode = RunRangePoll;
                    }
                }
            else
                {

									status = VL6180x_Prepare(theVL6180xDev);
                if( status )
                    {
                    display_err(1000);
                    mode = InitErr;
                    }
                else
                    {
                    mode = RunAlsPoll;
                    }
                }
             }
           
           
       switch (mode) 
       {
        case RunRangePoll:  
       
              if (Range.range_mm >= VL6180x_GetUpperLimit(theVL6180xDev)) 
                    {
                    display_err(uptime);
                    }
                    else
										display_number(Range.range_mm, uptime);
                    
             if (loop == measurementCycle)
            {
            loop=0;
               /* start single range measurement */
             status=VL6180x_RangeSetSystemMode(theVL6180xDev, MODE_START_STOP|MODE_SINGLESHOT);
             // poll the VL6180X till new sample ready
             VL6180x_RangeGetInterruptStatus(theVL6180xDev, &IntStatus);
             do 
              {
                if (Range.range_mm >= VL6180x_GetUpperLimit(theVL6180xDev)) 
                    {
                    display_err(uptime);
                    }
                    else
                    display_number(Range.range_mm, uptime);
										
                VL6180x_RangeGetInterruptStatus(theVL6180xDev, &IntStatus);
              }
              while (IntStatus!= RES_INT_STAT_GPIO_NEW_SAMPLE_READY);
             //get new range value
             status = VL6180x_RangeGetMeasurement(theVL6180xDev,&Range);
            //  clear all interrupt source
            status = VL6180x_ClearInterrupt(theVL6180xDev, INTERRUPT_CLEAR_ERROR|INTERRUPT_CLEAR_RANGING);
                    
             }
             loop++; // count how many time display been updated
         break; 
              
       case RunAlsPoll:
                display_number(Als.lux, uptime);
                
           if (loop == measurementCycle)
            {
            loop=0;
               /* start single ALS measurement */
             status=VL6180x_AlsSetSystemMode(theVL6180xDev, MODE_START_STOP|MODE_SINGLESHOT);
             // poll the VL6180X till new sample ready
             status = VL6180x_AlsGetInterruptStatus(theVL6180xDev, &IntStatus);
             do 
              {
              display_number(Als.lux, uptime);
              status = VL6180x_AlsGetInterruptStatus(theVL6180xDev, &IntStatus);
              }
              while (IntStatus != RES_INT_STAT_GPIO_NEW_SAMPLE_READY);
             //get new ALS value
             VL6180x_AlsGetMeasurement(theVL6180xDev,&Als); 
                  
             status = VL6180x_AlsClearInterrupt(theVL6180xDev);
              }
             loop++; // count how many time display been updated
        break;
        
        }
       

        clear_display();
      }

   }
  }
  /* USER CODE END 3 */



/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

}


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
