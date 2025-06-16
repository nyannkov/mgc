/*****************************************************************************
* | File      	:   LCD_1in3.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master
*                and enhance portability
*----------------
* |	This version:   V1.0
* | Date        :   2020-05-20
* | Info        :   Basic version
*
******************************************************************************/

/*****************************************************************************
* This software is based on and modified from the sample code provided by Waveshare.
*
* Reference:
* https://www.waveshare.com/wiki/Pico-LCD-1.3
******************************************************************************/

#include "ST7789.h"
#include "ports/ST7789_port.h"

#define LCD_1IN3_HEIGHT     MGC_DRIVERS_ST7789_HEIGHT
#define LCD_1IN3_WIDTH      MGC_DRIVERS_ST7789_WIDTH

typedef struct{
    UWORD WIDTH;
    UWORD HEIGHT;
    UBYTE SCAN_DIR;
}LCD_1IN3_ATTRIBUTES;

static LCD_1IN3_ATTRIBUTES LCD_1IN3;

#define HORIZONTAL 0
#define VERTICAL   1

#ifndef MGC_DRIVERS_ST7789_SCAN_DIR
#define MGC_DRIVERS_ST7789_SCAN_DIR    HORIZONTAL
#endif/*MGC_DRIVERS_ST7789_SCAN_DIR*/

#define LCD_SCAN_DIR            MGC_DRIVERS_ST7789_SCAN_DIR

/******************************************************************************
function :	Hardware reset
parameter:
******************************************************************************/
static inline void LCD_1IN3_Reset(void) {
    ST7789_PORT__Digital_Write(LCD_RST_PIN, 1);
    ST7789_PORT__Delay_ms(100);
    ST7789_PORT__Digital_Write(LCD_RST_PIN, 0);
    ST7789_PORT__Delay_ms(100);
    ST7789_PORT__Digital_Write(LCD_RST_PIN, 1);
    ST7789_PORT__Delay_ms(100);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void LCD_1IN3_SendCommand(UBYTE Reg) {
    ST7789_PORT__Digital_Write(LCD_DC_PIN, 0);
    ST7789_PORT__Digital_Write(LCD_CS_PIN, 0);
    ST7789_PORT__SPI_WriteByte(Reg);
    ST7789_PORT__Digital_Write(LCD_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static inline void LCD_1IN3_SendData_8Bit(UBYTE Data) {
    ST7789_PORT__Digital_Write(LCD_DC_PIN, 1);
    ST7789_PORT__Digital_Write(LCD_CS_PIN, 0);
    ST7789_PORT__SPI_WriteByte(Data);
    ST7789_PORT__Digital_Write(LCD_CS_PIN, 1);
}

/******************************************************************************
function :	Initialize the lcd register
parameter:
******************************************************************************/
static void LCD_1IN3_InitReg(void) {
    LCD_1IN3_SendCommand(0x3A);
    LCD_1IN3_SendData_8Bit(0x05);

    LCD_1IN3_SendCommand(0xB2);
    LCD_1IN3_SendData_8Bit(0x0C);
    LCD_1IN3_SendData_8Bit(0x0C);
    LCD_1IN3_SendData_8Bit(0x00);
    LCD_1IN3_SendData_8Bit(0x33);
    LCD_1IN3_SendData_8Bit(0x33);

    LCD_1IN3_SendCommand(0xB7);  //Gate Control
    LCD_1IN3_SendData_8Bit(0x35);

    LCD_1IN3_SendCommand(0xBB);  //VCOM Setting
    LCD_1IN3_SendData_8Bit(0x19);

    LCD_1IN3_SendCommand(0xC0); //LCM Control     
    LCD_1IN3_SendData_8Bit(0x2C);

    LCD_1IN3_SendCommand(0xC2);  //VDV and VRH Command Enable
    LCD_1IN3_SendData_8Bit(0x01);
    LCD_1IN3_SendCommand(0xC3);  //VRH Set
    LCD_1IN3_SendData_8Bit(0x12);
    LCD_1IN3_SendCommand(0xC4);  //VDV Set
    LCD_1IN3_SendData_8Bit(0x20);

    LCD_1IN3_SendCommand(0xC6);  //Frame Rate Control in Normal Mode
    LCD_1IN3_SendData_8Bit(0x0F);
    
    LCD_1IN3_SendCommand(0xD0);  // Power Control 1
    LCD_1IN3_SendData_8Bit(0xA4);
    LCD_1IN3_SendData_8Bit(0xA1);

    LCD_1IN3_SendCommand(0xE0);  //Positive Voltage Gamma Control
    LCD_1IN3_SendData_8Bit(0xD0);
    LCD_1IN3_SendData_8Bit(0x04);
    LCD_1IN3_SendData_8Bit(0x0D);
    LCD_1IN3_SendData_8Bit(0x11);
    LCD_1IN3_SendData_8Bit(0x13);
    LCD_1IN3_SendData_8Bit(0x2B);
    LCD_1IN3_SendData_8Bit(0x3F);
    LCD_1IN3_SendData_8Bit(0x54);
    LCD_1IN3_SendData_8Bit(0x4C);
    LCD_1IN3_SendData_8Bit(0x18);
    LCD_1IN3_SendData_8Bit(0x0D);
    LCD_1IN3_SendData_8Bit(0x0B);
    LCD_1IN3_SendData_8Bit(0x1F);
    LCD_1IN3_SendData_8Bit(0x23);

    LCD_1IN3_SendCommand(0xE1);  //Negative Voltage Gamma Control
    LCD_1IN3_SendData_8Bit(0xD0);
    LCD_1IN3_SendData_8Bit(0x04);
    LCD_1IN3_SendData_8Bit(0x0C);
    LCD_1IN3_SendData_8Bit(0x11);
    LCD_1IN3_SendData_8Bit(0x13);
    LCD_1IN3_SendData_8Bit(0x2C);
    LCD_1IN3_SendData_8Bit(0x3F);
    LCD_1IN3_SendData_8Bit(0x44);
    LCD_1IN3_SendData_8Bit(0x51);
    LCD_1IN3_SendData_8Bit(0x2F);
    LCD_1IN3_SendData_8Bit(0x1F);
    LCD_1IN3_SendData_8Bit(0x1F);
    LCD_1IN3_SendData_8Bit(0x20);
    LCD_1IN3_SendData_8Bit(0x23);

    LCD_1IN3_SendCommand(0x21);  //Display Inversion On

    LCD_1IN3_SendCommand(0x11);  //Sleep Out

    LCD_1IN3_SendCommand(0x29);  //Display On
}

/********************************************************************************
function:	Set the resolution and scanning method of the screen
parameter:
		Scan_dir:   Scan direction
********************************************************************************/
static void LCD_1IN3_SetAttributes(UBYTE Scan_dir) {
    //Get the screen scan direction
    LCD_1IN3.SCAN_DIR = Scan_dir;
    UBYTE MemoryAccessReg = 0x00;

    //Get GRAM and LCD width and height
    if(Scan_dir == HORIZONTAL) {
        LCD_1IN3.HEIGHT	= LCD_1IN3_WIDTH;
        LCD_1IN3.WIDTH   = LCD_1IN3_HEIGHT;
        MemoryAccessReg = 0X70;
    } else {
        LCD_1IN3.HEIGHT	= LCD_1IN3_HEIGHT;       
        LCD_1IN3.WIDTH   = LCD_1IN3_WIDTH;
        MemoryAccessReg = 0X00;
    }

    // Set the read / write scan direction of the frame memory
    LCD_1IN3_SendCommand(0x36); //MX, MY, RGB mode
    LCD_1IN3_SendData_8Bit(MemoryAccessReg);	//0x08 set RGB
}

/********************************************************************************
function:	Sets the start position and size of the display area
parameter:
		Xstart 	:   X direction Start coordinates
		Ystart  :   Y direction Start coordinates
		Xend    :   X direction end coordinates
		Yend    :   Y direction end coordinates
********************************************************************************/
static void LCD_1IN3_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend) {
    //set the X coordinates
    LCD_1IN3_SendCommand(0x2A);
    LCD_1IN3_SendData_8Bit(0x00);
    LCD_1IN3_SendData_8Bit(Xstart);
	LCD_1IN3_SendData_8Bit(0x00);
    LCD_1IN3_SendData_8Bit(Xend);

    //set the Y coordinates
    LCD_1IN3_SendCommand(0x2B);
    LCD_1IN3_SendData_8Bit(0x00);
	LCD_1IN3_SendData_8Bit(Ystart);
	LCD_1IN3_SendData_8Bit(0x00);
    LCD_1IN3_SendData_8Bit(Yend);

    LCD_1IN3_SendCommand(0X2C);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
static void LCD_1IN3_Clear(UWORD Color) {
    UWORD j,i;
    UWORD Image[LCD_1IN3.WIDTH*LCD_1IN3.HEIGHT];
    
    Color = ((Color<<8)&0xff00)|(Color>>8);
   
    for (j = 0; j < LCD_1IN3.HEIGHT*LCD_1IN3.WIDTH; j++) {
        Image[j] = Color;
    }
    
    LCD_1IN3_SetWindows(0, 0, LCD_1IN3.WIDTH, LCD_1IN3.HEIGHT);
    ST7789_PORT__Digital_Write(LCD_DC_PIN, 1);
    ST7789_PORT__Digital_Write(LCD_CS_PIN, 0);
    for(j = 0; j < LCD_1IN3.HEIGHT; j++){
        ST7789_PORT__SPI_Write_nByte((uint8_t *)&Image[j*LCD_1IN3.WIDTH], LCD_1IN3.WIDTH*2);
        
    }
    ST7789_PORT__Digital_Write(LCD_CS_PIN, 1);
}

/********************************************************************************
function :	Initialize the lcd
parameter:
********************************************************************************/
static void LCD_1IN3_Init(UBYTE Scan_dir) {

    ST7789_PORT__SET_PWM(90);
    //Hardware reset
    LCD_1IN3_Reset();

    //Set the resolution and scanning method of the screen
    LCD_1IN3_SetAttributes(Scan_dir);
    
    //Set the initialization register
    LCD_1IN3_InitReg();
}

/******************************************************************************
function :	Sends the image buffer in RAM to displays
parameter:
******************************************************************************/
static void LCD_1IN3_DisplayWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD *Image) {
    UWORD j;
    UWORD DX, DY, len;
    DX = Xend - Xstart + 1;
    DY = Yend - Ystart + 1;
    len = DX * 2;

    LCD_1IN3_SetWindows(Xstart, Ystart, Xend, Yend);
    ST7789_PORT__Digital_Write(LCD_DC_PIN, 1);
    ST7789_PORT__Digital_Write(LCD_CS_PIN, 0);
    for (j = 0; j < DY; j++ ) {
        ST7789_PORT__SPI_Write_nByte((uint8_t *)&Image[j * DX], len);
    }
    ST7789_PORT__Digital_Write(LCD_CS_PIN, 1);
}

void ST7789_init(uint32_t spi_clock_rate) {

    ST7789_PORT__Init(spi_clock_rate);

    // LCD Initialization
    LCD_1IN3_Init(LCD_SCAN_DIR);
    LCD_1IN3_Clear(0x0);
}

void ST7789_transfer_region(uint8_t *buffer, size_t len, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    LCD_1IN3_DisplayWindows(x0, y0, x1, y1, (UWORD*)buffer);
}

void ST7789_reset(void) {
    LCD_1IN3_Reset();
}
