/**
 ******************************************************************************
 * @file    x_nucleo_ikp01a1_display.c
 * @author  
 * @version v1.0
 * @date    10-Dec-2014
 * @brief   This file provides a set of functions needed to manage the display on X-nucleo-ikp01a expansion board.
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
#include "x_nucleo_ikp01a_display.h"

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "x_nucleo_ikp01a1.h"

#define n_segments 8        // 7 segment digits, plus dot
#define n_digits 4          // 4 digits display
#define n_numbers 11        // 0-9, dot
#define n_digit_combos 6    //

/* define a structure for the µC output to drive the display */
typedef struct
{
  uint32_t Pin;      
  GPIO_TypeDef* GPIOx;  
  int state;
}DISPLAY_GPIO;

//define structures per segment and digit with PIN and GPIO_port 
DISPLAY_GPIO digit1= {GPIO_PIN_0, GPIOB, 0};
DISPLAY_GPIO digit2= {GPIO_PIN_4, GPIOA, 0};
DISPLAY_GPIO digit3= {GPIO_PIN_1, GPIOA, 0};
DISPLAY_GPIO digit4= {GPIO_PIN_0, GPIOA, 0};


DISPLAY_GPIO segment1= {GPIO_PIN_6,  GPIOB, 0};
DISPLAY_GPIO segment2= {GPIO_PIN_7,  GPIOC, 0};
DISPLAY_GPIO segment3= {GPIO_PIN_9,  GPIOA, 0};
DISPLAY_GPIO segment4= {GPIO_PIN_8,  GPIOA, 0};
DISPLAY_GPIO segment5= {GPIO_PIN_10, GPIOB, 0};
DISPLAY_GPIO segment6= {GPIO_PIN_4,  GPIOB, 0};
DISPLAY_GPIO segment7= {GPIO_PIN_5,  GPIOB, 0};
DISPLAY_GPIO segment8= {GPIO_PIN_3,  GPIOB, 0};

/* define table of GPIO for segments and digit */
DISPLAY_GPIO led[n_segments]; // = {segment1,segment2,segment3,segment4,segment5,segment6,segment7,segment8};
DISPLAY_GPIO seg[n_digits]; //  = {digit1,digit2,digit3,digit4};


///////////////////////////////////////////////////////////////////////////////////////////////
// number define for each Segment
///////////////////////////////////////////////////////////////////////////////////////////////
int number[n_numbers][n_segments]={
    {0,0,0,0,0,0,1,1},          //zero
    {1,0,0,1,1,1,1,1},          //one
    {0,0,1,0,0,1,0,1},           //two
    {0,0,0,0,1,1,0,1},          //three
    {1,0,0,1,1,0,0,1},       //four
    {0,1,0,0,1,0,0,1},          //five
    {0,1,0,0,0,0,0,1},          //six
    {0,0,0,1,1,1,1,1},          //seven
    {0,0,0,0,0,0,0,1},           //eight
    {0,0,0,0,1,0,0,1},             //nine
    {1,1,1,1,1,1,0,1}          //dot
};

// Define to display letter type
enum iLetter{
    letter_E,
    letter_F,
    letter_r,
    letter_b,
    letter_o,
    letter_n,
	  letter_H,
    n_letters
};

int letters[n_letters][n_segments]={
    {0,1,1,0,0,0,0,1},           //E
    {0,1,1,1,0,0,0,1},           //F
    {1,1,1,1,0,1,0,1},           //r
    {1,1,0,0,0,0,0,1},           //b
    {1,1,0,0,0,1,0,1},           //o
    {1,1,0,1,0,1,0,1},           //n
	  {1,0,0,1,0,0,0,1},           //H
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// pick a digit to display
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int digit_select[n_digit_combos][n_digits]=
{
    {0,1,1,1},          //0
    {1,0,1,1},          //1
    {1,1,0,1},          //2
    {1,1,1,0},          //3
    {1,1,1,1},          //all off
    {0,0,0,0}           //all on
};

void init_display()
{
  led[0] = segment1;
  led[1] = segment2;
  led[2] = segment3;
  led[3] = segment4;
  led[4] = segment5;
  led[5] = segment6;
  led[6] = segment7;
  led[7] = segment8;
  seg[0] = digit1;
  seg[1] = digit2;
  seg[2] = digit3;
  seg[3] = digit4;
  
}

void clear_display()
{
    for (int i=0; i<4; i++){
     HAL_GPIO_WritePin(seg[i].GPIOx, seg[i].Pin, (GPIO_PinState)digit_select[4][i]);
    }
}

void _display_letter(int index, int digit)
{
    for (int i=0; i<8; i++)
    {
     HAL_GPIO_WritePin(led[i].GPIOx, led[i].Pin, (GPIO_PinState)letters[index][i]);
    }
    for (int i=0; i<4; i++){
        HAL_GPIO_WritePin(seg[i].GPIOx, seg[i].Pin, (GPIO_PinState)digit_select[digit][i]);
    }
    HAL_Delay(1);
}

void _display_digit(int index, int digit)
{   
    for (int i=0; i<8; i++)
    {
        HAL_GPIO_WritePin(led[i].GPIOx, led[i].Pin, (GPIO_PinState)number[index][i]);
    }
    for (int i=0; i<4; i++){
        HAL_GPIO_WritePin(seg[i].GPIOx, seg[i].Pin, (GPIO_PinState)digit_select[digit][i]);
    }
    HAL_Delay(1);
}

void display_err(int duration_ms)
{
	   _display_digit(10, 0);
     _display_digit(10, 1);
     _display_digit(10, 2);
     _display_digit(10, 3);			
   
   HAL_Delay(duration_ms);
   clear_display();
}



void display_number(int range,int duration_ms) {
  int range_1000,range_100,range_10,range_1;

  
  range_1000 = (range/1000)%10;
  range_100 = (range/100)%10;
  range_10 = (range/10)%10;
  range_1 = range%10;
  
	if ( range_1000 > 0)
    duration_ms /= 4;
  else if ( range_100 > 0)
    duration_ms /=3;
  else if ( range_100 > 0)
    duration_ms /= 2;
  
  if (range>9999){
    range_1000=10;
    range_100=10;
    range_10=10;
    range_1=10;
  }
  
  if (range<0){
    range_1000=0;
    range_100=0;
    range_10=0;
    range_1=0;
  }
 
 if (range>999) {
        _display_digit(range_1000, 0);
        WaitMilliSec(duration_ms);
    }

    if (range>99){
        _display_digit(range_100, 1);
        WaitMilliSec(duration_ms);
    }

    if (range>9){
        _display_digit(range_10, 2);
        WaitMilliSec(duration_ms);
    }

    _display_digit(range_1, 3);
    WaitMilliSec(duration_ms);


}

void display_8888(int duration_ms)
{
 while (duration_ms) 
  { 
  display_number(8888,1);
	duration_ms--;
	}

  clear_display();
}






