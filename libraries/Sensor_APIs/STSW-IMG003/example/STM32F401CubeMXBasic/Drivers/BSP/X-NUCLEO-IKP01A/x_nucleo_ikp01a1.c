/**
 ******************************************************************************
 * @file    x_nucleo_ikp01a1.c
 * @author  
 * @version v1.0
 * @date    10-Dec-2014
 * @brief   This file provides a set of functions needed to manage the VL6180X sensor.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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
#include "x_nucleo_ikp01a1.h"
#include "vl6180x_platform.h"
#include "stm32f4xx_hal.h"

#ifdef USE_FREE_RTOS
#include "cmsis_os.h"
#endif


/** @addtogroup BSP
 * @{
 */

/** @addtogroup X_NUCLEO_IKP01A1
 * @{
 */

/** @addtogroup X_NUCLEO_IKP01A1
 * @{
 */


/** @defgroup X_NUCLEO_IKP01A1_Private_TypesDefinitions X_NUCLEO_IKP01A1_Private_TypesDefinitions
 * @{
 */


/**
 * @}
 */

/** @defgroup X_NUCLEO_IKP01A1_Private_Defines X_NUCLEO_IKP01A1_Private_Defines
 * @{
 */
 
#define V1_CHIP_SWITCH_PORT GPIOA
#define V1_CHIP_SWITCH_PIN  GPIO_PIN_7
#define V1_CHIP_ENABLE_PORT GPIOA
#define V1_CHIP_ENABLE_PIN  GPIO_PIN_5
#define V1_IRQ_PIN          GPIO_PIN_6

#ifndef NULL
#define NULL      (void *) 0
#endif
/**
 * @}
 */

/** @defgroup X_NUCLEO_IKP01A1_Private_Macros X_NUCLEO_IKP01A1_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup X_NUCLEO_IKP01A1_Private_Variables X_NUCLEO_IKP01A1_Private_Variables
 * @{
 */
I2C_HandleTypeDef hi2c1;



int VL6180x_I2CWrite(VL6180xDev_t addr, uint8_t  *buff, uint8_t len){
    int status;
    status = HAL_I2C_Master_Transmit(i2c_bus,  addr, buff, len , def_i2c_time_out);
    if( status ){
        VL6180x_Shield_I2C1_Init(&hi2c1);
    }
    return status;
}

int VL6180x_I2CRead(VL6180xDev_t addr, uint8_t  *buff, uint8_t len){
    int status;
    status = HAL_I2C_Master_Receive(i2c_bus,  addr, buff, len , def_i2c_time_out);
    if( status ){
        VL6180x_Shield_I2C1_Init(&hi2c1);
    }

    return status;
}

#if !FREE_RTOS

volatile uint32_t g_TickCnt;
char buff[10]="---.--";
float v=0000.1;
void HAL_SYSTICK_Callback(void){
    g_TickCnt++;
}

void WaitMilliSec(int ms){
    uint32_t start, now;
    int dif;
    start=g_TickCnt;
    //debug("waiting %d @%d\n",ms, g_TickCnt);
    do{
        now=g_TickCnt;
        dif= now -start;
    }
    while(dif<ms);
    //debug("waited  %d @%d\n",dif, g_TickCnt);
}

#else
//for FreeRtos ue os wait
void WaitMilliSec(int ms)   vTaskDelay( ms/ portTICK_PERIOD_MS)

#endif

/* init functions */
void VL6180x_Shield_Reset(int state){
    HAL_GPIO_WritePin(V1_CHIP_ENABLE_PORT, V1_CHIP_ENABLE_PIN , (GPIO_PinState)state);
}

int VL6180x_Shield_GetSwitch(void){
    GPIO_PinState state ;
    state = HAL_GPIO_ReadPin(V1_CHIP_SWITCH_PORT, V1_CHIP_SWITCH_PIN);
    return state;
}



void VL6180x_Shield_I2C1_Init(I2C_HandleTypeDef *hi2c1) {
    GPIO_InitTypeDef GPIO_InitStruct;
    /* Peripheral clock enable */
    __I2C1_CLK_ENABLE();

    /**I2C1 GPIO Configuration
     PB8     ------> I2C1_SCL	
     PB9     ------> I2C1_SDA
     */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    hi2c1->Instance = I2C1;
    hi2c1->Init.ClockSpeed = 400000;
    hi2c1->Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1->Init.OwnAddress1 = 0;
    hi2c1->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1->Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    hi2c1->Init.OwnAddress2 = 0;
    hi2c1->Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    hi2c1->Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
    HAL_I2C_Init(hi2c1);
}




/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PA2   ------> USART2_TX
     PA3   ------> USART2_RX
*/

void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();
  __GPIOH_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA4 PA5 
                           PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB10 PB4 PB5 
                           PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/     
