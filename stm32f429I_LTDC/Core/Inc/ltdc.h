#ifndef LTDC_H
#define LTDC_H

#include "stm32f4xx_hal.h"
#include "fonts.h"
#include <string.h>
#include <stdlib.h>

/* Colors */

#define LCD_COLOR_BLUE ((uint32_t)0x0000FF)
#define LCD_COLOR_GREEN ((uint32_t)0x00FF00)
#define LCD_COLOR_RED ((uint32_t)0xFF0000)
#define LCD_COLOR_CYAN ((uint32_t)0x00FFFF)
#define LCD_COLOR_MAGENTA ((uint32_t)0xFF00FF)
#define LCD_COLOR_YELLOW ((uint32_t)0xFFFF00)
#define LCD_COLOR_LIGHTBLUE ((uint32_t)0x8080FF)
#define LCD_COLOR_LIGHTGREEN ((uint32_t)0x80FF80)
#define LCD_COLOR_LIGHTRED ((uint32_t)0xFF8080)
#define LCD_COLOR_LIGHTCYAN ((uint32_t)0x80FFFF)
#define LCD_COLOR_LIGHTMAGENTA ((uint32_t)0xFF80FF)
#define LCD_COLOR_LIGHTYELLOW ((uint32_t)0xFFFF80)
#define LCD_COLOR_DARKBLUE ((uint32_t)0x000080)
#define LCD_COLOR_DARKGREEN ((uint32_t)0x008000)
#define LCD_COLOR_DARKRED ((uint32_t)0x800000)
#define LCD_COLOR_DARKCYAN ((uint32_t)0x008080)
#define LCD_COLOR_DARKMAGENTA ((uint32_t)0x800080)
#define LCD_COLOR_DARKYELLOW ((uint32_t)0x808000)
#define LCD_COLOR_WHITE ((uint32_t)0xFFFFFF)
#define LCD_COLOR_LIGHTGRAY ((uint32_t)0xD3D3D3)
#define LCD_COLOR_GRAY ((uint32_t)0x808080)
#define LCD_COLOR_DARKGRAY ((uint32_t)0x404040)
#define LCD_COLOR_BLACK ((uint32_t)0x000000)
#define LCD_COLOR_BROWN ((uint32_t)0xA52A2A)
#define LCD_COLOR_ORANGE ((uint32_t)0xFFA500)
#define LCD_COLOR_TRANSPARENT ((uint32_t)0x000000)

/* Registers */
#define LCD_SLEEP_OUT            0x11   /* Sleep out register */
#define LCD_GAMMA                0x26   /* Gamma register */
#define LCD_DISPLAY_OFF          0x28   /* Display off register */
#define LCD_DISPLAY_ON           0x29   /* Display on register */
#define LCD_COLUMN_ADDR          0x2A   /* Colomn address register */
#define LCD_PAGE_ADDR            0x2B   /* Page address register */
#define LCD_GRAM                 0x2C   /* GRAM register */
#define LCD_MAC                  0x36   /* Memory Access Control register*/
#define LCD_PIXEL_FORMAT         0x3A   /* Pixel Format register */
#define LCD_WDB                  0x51   /* Write Brightness Display register */
#define LCD_WCD                  0x53   /* Write Control Display register*/
#define LCD_RGB_INTERFACE        0xB0   /* RGB Interface Signal Control */
#define LCD_FRC                  0xB1   /* Frame Rate Control register */
#define LCD_BPC                  0xB5   /* Blanking Porch Control register*/
#define LCD_DFC                  0xB6   /* Display Function Control register*/
#define LCD_POWER1               0xC0   /* Power Control 1 register */
#define LCD_POWER2               0xC1   /* Power Control 2 register */
#define LCD_VCOM1                0xC5   /* VCOM Control 1 register */
#define LCD_VCOM2                0xC7   /* VCOM Control 2 register */
#define LCD_POWERA               0xCB   /* Power control A register */
#define LCD_POWERB               0xCF   /* Power control B register */
#define LCD_PGAMMA               0xE0   /* Positive Gamma Correction register*/
#define LCD_NGAMMA               0xE1   /* Negative Gamma Correction register*/
#define LCD_DTCA                 0xE8   /* Driver timing control A */
#define LCD_DTCB                 0xEA   /* Driver timing control B */
#define LCD_POWER_SEQ            0xED   /* Power on sequence register */
#define LCD_3GAMMA_EN            0xF2   /* 3 Gamma enable register */
#define LCD_INTERFACE            0xF6   /* Interface control register */
#define LCD_PRC                  0xF7   /* Pump ratio control register */

typedef struct
{
  uint32_t TextColor;
  uint32_t BackColor;
  sFONT *pFont;
}LCD_DrawPropTypeDef;

typedef enum
{
  CENTER_MODE = 0x01, /* Center mode */
  RIGHT_MODE = 0x02, /* Right mode */
  LEFT_MODE = 0x03 /* Left mode */
}Text_AlignModeTypdef;


void LCD_PowerOn(void);
void LCD_WriteCommand(uint8_t com);
void LCD_WriteData(uint8_t data);

void LCD_Test(void);

/* Primitive graphics */
void LCD_FillScreen(uint32_t color);
void LCD_FillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
void LCD_DrawPixel(uint32_t x, uint32_t y, uint32_t color);
void LCD_DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color);

/* Text */
void LCD_FontsInit(void);
void LCD_SetFont(sFONT *fonts);
void LCD_SetTextColor(uint32_t color);
void LCD_SetBackColor(uint32_t color);
void LCD_DrawChar(uint16_t x, uint16_t y, const uint8_t c);
void LCD_DrawString(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode);
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t radius, uint32_t color);
void LCD_DrawCross(uint32_t x, uint32_t y, uint32_t color);
void LCD_DrawImage(uint16_t x_pos, uint16_t y_pos, uint16_t W, uint16_t H, const uint32_t* bitmap);
#endif /* LTDC_H */
