/*******************************************************************************
 * LBF_UART_lowlevAPI.c
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

#include <stdint.h>



/*******************************************************************************
 * @brief  : Send 8-bit data through UART 
 * @param  : none.
 * @retval : none.
 ******************************************************************************/
void UART_SendData (UartID_t Uart_ID, uint8_t data)
{

if (Uart_ID == UART1)
{
     HAL_UART_Transmit(&huart1, &data, 0x1, 1000);
}
else if (Uart_ID == UART3)
{
     HAL_UART_Transmit(&huart3, &data, 0x1, 1000);
}
    // UART blocking Tx of 1 byte with 5s timeout
    // (huart(x) is global)
}

/*******************************************************************************
 * @brief  : Receive 8-bit data through UART 
 * @param  : Aucun.
 * @retval : Rien.
 ******************************************************************************/
uint8_t UART_ReceiveData (UartID_t Uart_ID)
{
uint8_t RxByte = 0;
 
if (Uart_ID == UART1)
{
    HAL_UART_Receive(&huart1, &RxByte, 0x1, 5000);
}
else if (Uart_ID == UART3)
{
    HAL_UART_Receive(&huart3, &RxByte, 0x1, 5000);
}
    // UART blocking Rx of 1 byte with 5s timeout
    // (huart(x) is global)
    
    return (RxByte);
    
}

/*******************************************************************************
 * @brief  : Send string of characters through UART 
 * @param  : Aucun.
 * @retval : Rien.
 ******************************************************************************/
void UART_SendString(UartID_t Uart_ID, char* pString)
{
// Do it in a more clever way?... use HAL API to send in one shot...
char* pStringLocal;

    pStringLocal = pString;
    while((*pStringLocal)!='\0')
    {
        UART_SendData(Uart_ID, *pStringLocal);
        pStringLocal++;
    }


}

/*******************************************************************************
 * @brief  : Send string with XON/XOFF based flow control
 * @param  : Aucun.
 * @retval : Rien.
 ******************************************************************************/
void UART_SendString_SwFlowControl(UartID_t Uart_ID, char* pString)
{

char* pStringLocal;
uint8_t  uart1_rx;

// FIXME !!!!
// Routine takes Uart_ID as input parameter but does not care and uses UART1

        pStringLocal = pString;
        while((*pStringLocal)!='\0')
    	{
             //Check for XOFF without waiting for any time out
            HAL_UART_Receive(&huart1, &uart1_rx, 0x1, 0);
            if (uart1_rx == XOFF)  // XOFF detected
            {
	         while (uart1_rx != XON)   // wait for XON to resume
	         {
                     HAL_UART_Receive(&huart1, &uart1_rx, 0x1, 0);
	         }
	    }
	    UART_SendData(UART1, *pStringLocal);
            pStringLocal++;

	}

}


/***************************************************************END OF FILE****/
