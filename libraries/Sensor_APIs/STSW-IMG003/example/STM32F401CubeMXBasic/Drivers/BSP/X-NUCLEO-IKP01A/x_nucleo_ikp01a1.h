/**
 ******************************************************************************
 * @file    x_nucleo_ikp01a1.h
 * @author  
 * @version v1.0
 * @date    10-Dec-2014
 * @brief   This file contains definitions for x_nucleo_ikp01a1.c
  *          firmware driver.
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
  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __X_NUCLEO_IKP01A1_H
#define __X_NUCLEO_IKP01A1_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

   
/** @addtogroup BSP
  * @{
  */
  
/** @addtogroup X_NUCLEO_IKP01A1
  * @{
  */ 
/* Private variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
  
/** @defgroup X_NUCLEO_IKP01A1_Exported_Types X_NUCLEO_IKP01A1_Exported_Types
  * @{
  */

/**
  * @}
  */
  
/** @defgroup X_NUCLEO_IKP01A1_Exported_Constants X_NUCLEO_IKP01A1_Exported_Constants
  * @{
  */
   
void MX_GPIO_Init(void);
void VL6180x_Shield_WaitMilliSec(int n);
void WaitMilliSec(int ms);
void VL6180x_Shield_I2C1_Init(I2C_HandleTypeDef *hi2c1);
int VL6180x_Shield_GetSwitch(void);
void VL6180x_Shield_Reset(int state);
    
/**
  * @}
  */   
          
          
/**
  * @}
  */
  
/** @defgroup X_NUCLEO_IKP01A1_Exported_Macros X_NUCLEO_IKP01A1_Exported_Macros
  * @{
  */
/**
 * VL6180x Cube Fxx i2c porting implementation
 */

#define theVL6180xDev   0x52    // what we use as "API device

#define i2c_bus      (&hi2c1)
#define def_i2c_time_out 100
/**
  * @}
  */
 
/** @defgroup X_NUCLEO_IKP01A1_Exported_Functions X_NUCLEO_IKP01A1_Exported_Functions
  * @{
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
  
/**
  * @}
  */
  
#ifdef __cplusplus
  }
#endif
  
#endif /* __X_NUCLEO_IKP01A1_H */
  


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
