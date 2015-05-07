/*******************************************************************************
 * Carroussel_Demo.c
 *
 * Private function of main()
 * 
 * Author: La BlueFrog, 2015
 *
 *******************************************************************************/

#include "LBF_Global.h"
#include "User_Configuration.h"

#include "string.h"

//boolean_t 	DemoChangeRequest(void);
boolean_t 	Detect_VL6180X_Covered(void);

static boolean_t  DecodeBMPFile(char* FileName);

static uint32_t  little_endian_32b (uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4);
static uint16_t  little_endian_16b (uint8_t data1, uint8_t data2);



void 	Carroussel_Demo(void)
{

//FIL MyFile;
DIR  dir;
FILINFO  FileInfoStruct;
char listBMPfiles[50][13]; //to store up to 50 file names of 13 char each
uint8_t fnameLength;

boolean_t FatFS_Ok = TRUE;

uint8_t ImgIndex = 0;
uint8_t TotalBMPImg = 0;

boolean_t NewPictureRequired( uint8_t* pDir );
uint8_t   direction;


/* -- Open root directory  ------------------------------ */

    FatFS_Ok &= (f_opendir( &dir, "/") == FR_OK);  

    Led_StopNBlinkOnFalse (FatFS_Ok);   //in case of FatFS error


/* -- Search all BMP file  ------------------------------ */

    FatFS_Ok &= ( f_readdir( &dir, &FileInfoStruct) == FR_OK);   // 


    while (FileInfoStruct.fname[0]!=0)   //as long as item is found
    {
	fnameLength = strlen(FileInfoStruct.fname);

        if ( FileInfoStruct.fname[fnameLength-3]=='B' 
          && FileInfoStruct.fname[fnameLength-2]=='M' 
          && FileInfoStruct.fname[fnameLength-1]=='P' )
	{	
            // copy found file name to current entry of table of BMP file names
	    strcpy( &listBMPfiles[ImgIndex][0] , FileInfoStruct.fname);  
	    ImgIndex++;
	}

        //next item  (break on error) :
        if (f_readdir( &dir, &FileInfoStruct) != FR_OK) break;

    }
    TotalBMPImg = ImgIndex;

    f_closedir(&dir) ;   


/* --  Decode BMP files with switch control   -------------------------------- */
 

    DecodeBMPFile("FROG.SPE"); 
   // Initial picture 

    ImgIndex = 0;
    while(1)
    {

//      if ( Detect_VL6180X_Covered() )  // demo change requested
      if ( State_Switch1_IsOn() || State_Switch2_IsOn() )  // demo change requested
	 return;
      else
      {
          if ( NewPictureRequired( &direction ) )
          {
            if ( direction == 0) 
            {
	        Stm32_Led_ON();
                if (ImgIndex++ == (TotalBMPImg-1) ) 
                {
                     ImgIndex = 0;   // roll-over
                }
	        DecodeBMPFile(&listBMPfiles[ImgIndex][0]);

	        // Wait for switch release before continuing
	        //while( State_Switch1_IsOn() );  

	        Stm32_Led_OFF();
            }

            if ( direction == 1) 
            {
	        Stm32_Led_ON();
                if (ImgIndex-- == 0) 
                {
                    ImgIndex = TotalBMPImg - 1;   // roll-over
                }
	        DecodeBMPFile(&listBMPfiles[ImgIndex][0]);

    	        // Wait for switch release before continuing
	        //while( State_Switch2_IsOn() );  

	        Stm32_Led_OFF();
            }
          }
      }

    }


}

/*  ---  Private functions  ----------- */

boolean_t NewPictureRequired( uint8_t* pDir )
{
boolean_t  ret = FALSE ;
int8_t     Y_Accel_HighByte;


  if ( (I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x1E) & 0x01) == 0x01)
  // XLDA bit set i.e. new accel data available
  {
        Y_Accel_HighByte =  (int8_t) I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x2B);   

        if (Y_Accel_HighByte > 12 ) 
        {
            ret = TRUE;
            *pDir = 0;
        }
        if (Y_Accel_HighByte < -12 )
        {
            ret = TRUE;
            *pDir = 1;
        }
   }

   return ret;
  
}

/* -------------------------------------- */

boolean_t  DecodeBMPFile(char* FileName)
{

FIL MyFile;

boolean_t  FatFS_Ok = TRUE;
boolean_t  ValidBMP = TRUE;
boolean_t TopToBottomBMP;

BYTE BMPFileHeader[14];
BYTE BMPImageHeader[40];
BYTE BMPLineBuffer[3*160];

UINT rbytes_count; /* File read counts */

uint32_t  OffsetHead;
uint32_t  ImgWidth, ImgHeight;
int32_t  ImgHeight_signed;

uint8_t   R, G, B;
uint16_t  PixelLine565[160];

uint16_t  i,j;


    /* --- Open File and Read File Hedader + Image Header  ------------------- */

    FatFS_Ok &= ( f_open(&MyFile, FileName, FA_OPEN_EXISTING|FA_READ) == FR_OK );
   
    FatFS_Ok &= ( f_read( &MyFile, BMPFileHeader, sizeof(BMPFileHeader), &rbytes_count ) == FR_OK);

    FatFS_Ok &= ( f_read( &MyFile, BMPImageHeader, sizeof(BMPImageHeader), &rbytes_count) == FR_OK);
 
    // DEBUG- Stop here and blink fast LED if FatFS failure
    Led_StopNBlinkOnFalse (FatFS_Ok);

    /* --- Extract and Check Information from Header Contents  ---------------- */

    // Check first 2 bytes are B,M - Bytes 0,1 in File Header
    ValidBMP &= ( (BMPFileHeader[0] == 0x42) && (BMPFileHeader[1] == 0x4D) );

    // Get Offset to Start of Pixel Data - Bytes 10-13 of File Header
    OffsetHead = little_endian_32b( BMPFileHeader[10], 
				    BMPFileHeader[11],
				    BMPFileHeader[12],
				    BMPFileHeader[13] );

    // Check picture is 24bpp - Bytes 14-15 of Image Header
    ValidBMP &= ( little_endian_16b( BMPImageHeader[14], BMPImageHeader[15] ) == 24 );

    // Get picture width in pix - Bytes 4-7 of Image Header
    ImgWidth = little_endian_32b(   BMPImageHeader[4], 
				    BMPImageHeader[5],
				    BMPImageHeader[6],
				    BMPImageHeader[7] );

    // Get picture height in pix - Bytes 8-11 of Image Header
    ImgHeight_signed = little_endian_32b(  BMPImageHeader[8], 
				    	   BMPImageHeader[9],
			   	   	   BMPImageHeader[10],
				   	   BMPImageHeader[11] );
   //Negative Height in BMP indicated Top to Bottom image coding
   if (ImgHeight_signed<0)
   {     TopToBottomBMP = TRUE;
	 ImgHeight = (uint32_t)(-ImgHeight_signed);
   }
   else
   {     TopToBottomBMP = FALSE;
	 ImgHeight = (uint32_t)(ImgHeight_signed);
   }

    // Check picture not greater than 160x128
    ValidBMP &= (  (ImgWidth <= 160) && (ImgHeight <= 128) );



    /* --- Now Read and Display Picture line by line  ------------------------ */
    /* --  unless BMP had issues                      ------------------------ */

    if (!ValidBMP)
    {
        OLED_Fill(0,0,160,128, BLUE);
    }
    else
    {

    // THIS EXECUTES SLOWLY AND WOULD NEED TO BE OPTIMIZED
    // Need to investigate what is the bottleneck

    // Move Read Pointer to start of pixel data 
    FatFS_Ok  &=  ( f_lseek( &MyFile, (DWORD)(OffsetHead)) == FR_OK );


    for(i=0; i<ImgHeight; i++)
    {
         FatFS_Ok  &=  ( f_read( &MyFile, BMPLineBuffer, 3*ImgWidth,&rbytes_count) == FR_OK);

	 // Create RGB565 buffer for OLED
	 for (j=0; j<ImgWidth;j++)
	 {
	     B = BMPLineBuffer[j*3] >> 3;      // truncate 3 LSBs
	     G = BMPLineBuffer[j*3 + 1] >> 2;  // truncate 2 LSBs
	     R = BMPLineBuffer[j*3 + 2] >> 3;  // truncate 3 LSBs
	     PixelLine565[j] =  (uint16_t)((R << 11) | (G << 5) | B);
	 }
	 if (TopToBottomBMP)
	 {
             OLED_DisplayBuffer( 0, i, (uint8_t)(ImgWidth), 1, PixelLine565 );
         }   
	 else
	 {
             OLED_DisplayBuffer( 0, (uint8_t)(ImgHeight-1-i),(uint8_t)(ImgWidth), 1, PixelLine565 );
         }   
     }
     }

     FatFS_Ok  &=  (f_close(&MyFile) == FR_OK);

     return (FatFS_Ok & ValidBMP);
}




/* ---------------------------------------------------------------
 * @brief  : 32-bit little-endian to big-endian conversion
 * @param  : little endian data
 * @return : Permuted data on 4 bytes
   --------------------------------------------------------------- */
uint32_t little_endian_32b (uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4)
{
    return (uint32_t)(data1 | (data2 << 8) | (data3 << 16) | (data4 << 24));
}



/* ---------------------------------------------------------------
 * @brief  : 16-bit little-endian to big-endian conversion
 * @param  : little endian data
 * @return : Permuted data on 2 bytes
   --------------------------------------------------------------- */
uint16_t little_endian_16b (uint8_t data1, uint8_t data2)
{
    return (uint16_t)(data1 | (data2 << 8));
}




