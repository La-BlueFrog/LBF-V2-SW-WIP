/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ex.c
  * @author  AST/AMS Application Team
  * @version V1.0.0
  * @date    9-Sept-2014
  * @brief   This file provides set of firmware functions to manage:
  *          - I2C Slave Communication
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
#include "stm32f4xx_nucleo_ex.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32F4XX_NUCLEO_EX
  * @{
  */   
    
/** @addtogroup STM32F4XX_NUCLEO_EX_LOW_LEVEL 
  * @brief This file provides set of firmware functions to manage Leds and push-button
  *        available on STM32F4xx-Nucleo Kit from STMicroelectronics.
  * @{
  */ 

/** @defgroup STM32F4XX_NUCLEO_EX_LOW_LEVEL_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32F4XX_NUCLEO_EX_LOW_LEVEL_Private_Defines
  * @{
  */ 

/**
  * @brief STM32F4xx NUCLEO BSP Driver Extension version number V1.0.0
  */


/** @defgroup STM32F4XX_NUCLEO_EX_LOW_LEVEL_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F4XX_NUCLEO_EX_LOW_LEVEL_Private_Variables
  * @{
  */ 
UART_HandleTypeDef UartHandle;

/**
 * @brief BUS variables
 */

/**
  * @}
  */ 

/** @defgroup STM32F4XX_NUCLEO_EX_LOW_LEVEL_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */ 

/** @defgroup STM32F4XX_NUCLEO_EX_LOW_LEVEL_Private_Functions
  * @{
  */ 


    
/******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/

/********************************* LINK I2C ************************************/

/**
 * @brief  Configures I2C interface.
 * @param  None
 * @retval None
 */
void UART_STLink_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX clock */
    USARTx_TX_GPIO_CLK_ENABLE();
    /* Enable USART clock */
    USARTx_CLK_ENABLE();
    /* Enable DMA clock */
    DMAx_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USARTx_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = USARTx_TX_AF;

    HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART1 configured as follow:
            - Word Length = 8 Bits
            - Stop Bit = One Stop bit
            - Parity = None
            - BaudRate = 9600 baud
            - Hardware flow control disabled (RTS and CTS signals) */
    UartHandle.Instance        = USARTx;
    UartHandle.Init.BaudRate   = 115200;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX;

    if(HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        //          Error_Handler();
      while(1);
    }
}


/**
  * @brief  Prepare transmitting bytes to I2C slave periph.
  * @param  pData data buffer pointer
  * @param  Size number of bytes to read
  * @retval HAL Status.
  */
void UART_STLink_IO_Transmit(uint8_t *pData, uint16_t Size)
{
  HAL_UART_Transmit(&UartHandle, pData, Size, 1000);
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
