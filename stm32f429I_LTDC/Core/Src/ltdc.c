#include "ltdc.h"
#include "main.h"

#define swap(a,b) {int16_t t=a;a=b;b=t;}

extern LTDC_HandleTypeDef hltdc;
extern SPI_HandleTypeDef hspi5;
extern DMA2D_HandleTypeDef hdma2d;

uint16_t X_SIZE = 240, Y_SIZE = 320;

LCD_DrawPropTypeDef lcdProp;

const uint8_t *ch;

void LCD_PowerOn(void)
{
	LCD_WriteCommand(0xCA);
	LCD_WriteData(0xC3);
	LCD_WriteData(0x08);
	LCD_WriteData(0x50);
	LCD_WriteCommand(LCD_POWERB);
	LCD_WriteData(0x00);
	LCD_WriteData(0xC1);
	LCD_WriteData(0x30);
	LCD_WriteCommand(LCD_POWER_SEQ);
	LCD_WriteData(0x64);
	LCD_WriteData(0x03);
	LCD_WriteData(0x12);
	LCD_WriteData(0x81);
	LCD_WriteCommand(LCD_DTCA);
	LCD_WriteData(0x85);
	LCD_WriteData(0x00);
	LCD_WriteData(0x78);
	LCD_WriteCommand(LCD_POWERA);
	LCD_WriteData(0x39);
	LCD_WriteData(0x2C);
	LCD_WriteData(0x00);
	LCD_WriteData(0x34);
	LCD_WriteData(0x02);
	LCD_WriteCommand(LCD_PRC);
	LCD_WriteData(0x20);
	LCD_WriteCommand(LCD_DTCB);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteCommand(LCD_FRC);
	LCD_WriteData(0x00);
	LCD_WriteData(0x1B);
	LCD_WriteCommand(LCD_DFC);
	LCD_WriteData(0x0A);
	LCD_WriteData(0xA2);
	LCD_WriteCommand(LCD_POWER1);
	LCD_WriteData(0x10);
	LCD_WriteCommand(LCD_POWER2);
	LCD_WriteData(0x10);
	LCD_WriteCommand(LCD_VCOM1);
	LCD_WriteData(0x45);
	LCD_WriteData(0x15);
	LCD_WriteCommand(LCD_VCOM2);
	LCD_WriteData(0x90);
	LCD_WriteCommand(LCD_MAC);
	LCD_WriteData(0xC8);
	LCD_WriteCommand(LCD_3GAMMA_EN);
	LCD_WriteData(0x00);
	LCD_WriteCommand(LCD_RGB_INTERFACE);
	LCD_WriteData(0xC2);
	LCD_WriteCommand(LCD_DFC);
	LCD_WriteData(0x0A);
	LCD_WriteData(0xA7);
	LCD_WriteData(0x27);
	LCD_WriteData(0x04);

	/* colomn address set */
	LCD_WriteCommand(LCD_COLUMN_ADDR);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0xEF);
	/* Page Address Set */
	LCD_WriteCommand(LCD_PAGE_ADDR);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0x3F);
	LCD_WriteCommand(LCD_INTERFACE);
	LCD_WriteData(0x01);
	LCD_WriteData(0x00);
	LCD_WriteData(0x06);

	LCD_WriteCommand(LCD_GRAM);
	HAL_Delay(100);

	LCD_WriteCommand(LCD_GAMMA);
	LCD_WriteData(0x01);

	LCD_WriteCommand(LCD_PGAMMA);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x29);
	LCD_WriteData(0x24);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x09);
	LCD_WriteData(0x4E);
	LCD_WriteData(0x78);
	LCD_WriteData(0x3C);
	LCD_WriteData(0x09);
	LCD_WriteData(0x13);
	LCD_WriteData(0x05);
	LCD_WriteData(0x17);
	LCD_WriteData(0x11);
	LCD_WriteData(0x00);
	LCD_WriteCommand(LCD_NGAMMA);
	LCD_WriteData(0x00);
	LCD_WriteData(0x16);
	LCD_WriteData(0x1B);
	LCD_WriteData(0x04);
	LCD_WriteData(0x11);
	LCD_WriteData(0x07);
	LCD_WriteData(0x31);
	LCD_WriteData(0x33);
	LCD_WriteData(0x42);
	LCD_WriteData(0x05);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x0A);
	LCD_WriteData(0x28);
	LCD_WriteData(0x2F);
	LCD_WriteData(0x0F);

	LCD_WriteCommand(LCD_SLEEP_OUT);
	HAL_Delay(100);
	LCD_WriteCommand(LCD_DISPLAY_ON);
	/* GRAM start writing */
	LCD_WriteCommand(LCD_GRAM);
}

void LCD_WriteCommand(uint8_t com)
{
	uint8_t buf[2] = {com, 0};
	HAL_GPIO_WritePin(LTDC_WRX_GPIO_Port, LTDC_WRX_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LTDC_NCS_GPIO_Port, LTDC_NCS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi5, buf, 1, 100);
	HAL_GPIO_WritePin(LTDC_NCS_GPIO_Port, LTDC_NCS_Pin, GPIO_PIN_SET);
}

void LCD_WriteData(uint8_t data)
{
	uint8_t buf[2] = {data, 0};
	HAL_GPIO_WritePin(LTDC_WRX_GPIO_Port, LTDC_WRX_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LTDC_NCS_GPIO_Port, LTDC_NCS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi5, buf, 1, 100);
	HAL_GPIO_WritePin(LTDC_NCS_GPIO_Port, LTDC_NCS_Pin, GPIO_PIN_SET);
}

void LCD_FillScreen(uint32_t color)
{
	hdma2d.Init.Mode = DMA2D_R2M;
	hdma2d.Init.OutputOffset = 0;
	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK)
	{
		if (HAL_DMA2D_Start(&hdma2d, color, hltdc.LayerCfg[0].FBStartAdress,
				hltdc.LayerCfg[0].ImageWidth, hltdc.LayerCfg[0].ImageHeight) == HAL_OK)
		{
			HAL_DMA2D_PollForTransfer(&hdma2d, 10);
		}
	}/*
	uint32_t n = hltdc.LayerCfg[0].ImageHeight*hltdc.LayerCfg[0].ImageWidth;
	for (uint32_t i = 0; i < (n); i++)
	{
		*(__IO uint32_t*) (hltdc.LayerCfg[0].FBStartAdress + (i*3)) = color;
	}*/
}

void LCD_Test(void)
{
	uint32_t color = 0x00;
	uint32_t n = hltdc.LayerCfg[0].ImageHeight*hltdc.LayerCfg[0].ImageWidth;
	for (uint32_t i = 0; i < (n); i++)
	{
		*(__IO uint32_t*) (hltdc.LayerCfg[0].FBStartAdress + (i*3)) = color;
		color++;
		if (color == 0xFFFFFF)
		{
			color = 0x00;
		}
	}
}

void LCD_FillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
	if(x1 > x2) swap(x1, x2);
	if(y1 > y2) swap(y1, y2);

	uint32_t addr = 0;
	addr = (hltdc.LayerCfg[0].FBStartAdress) + 3*(y1*hltdc.LayerCfg[0].ImageWidth + x1);
	hdma2d.Init.Mode = DMA2D_R2M;
	hdma2d.Init.OutputOffset = hltdc.LayerCfg[0].ImageWidth-(x2-x1);

	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK)
	{
		if (HAL_DMA2D_Start(&hdma2d, color, addr, x2-x1, y2-y1) == HAL_OK)
		{
			HAL_DMA2D_PollForTransfer(&hdma2d, 10);
		}
	}/*
	uint32_t xPos, yPos;

	if(x1 > x2) swap(x1, x2);
	if(y1 > y2) swap(y1, y2);

	for (yPos = y1; yPos <= y2; yPos++)
	{
		for (xPos = x1; xPos <= x2; xPos++)
		{
			*(__IO uint32_t*) (hltdc.LayerCfg[0].FBStartAdress + (3*(yPos*hltdc.LayerCfg[0].ImageWidth + xPos))) = color;
		}
	}*/
}

void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t radius, uint32_t color)
{
    int x = -radius, y = 0, err = 2-2*radius, e2;
    do {/*
    	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK)
    	{
    		if (HAL_DMA2D_Start(&hdma2d, color, hltdc.LayerCfg[0].FBStartAdress + (2*((Xpos-x) + 240*(Ypos+y)))
    				, 1, 1) == HAL_OK)
    		{
    			HAL_DMA2D_PollForTransfer(&hdma2d, 10);
    		}
    	}
    	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK)
    	{
    		if (HAL_DMA2D_Start(&hdma2d, color, hltdc.LayerCfg[0].FBStartAdress + (2*((Xpos+x) + 240*(Ypos+y)))
   				, 1, 1) == HAL_OK)
    	    {
      			HAL_DMA2D_PollForTransfer(&hdma2d, 10);
    		}
    	}
    	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK)
    	{
    		if (HAL_DMA2D_Start(&hdma2d, color, hltdc.LayerCfg[0].FBStartAdress + (2*((Xpos+x) + 240*(Ypos-y)))
    				, 1, 1) == HAL_OK)
    		{
    			HAL_DMA2D_PollForTransfer(&hdma2d, 10);
    		}
    	}
    	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK)
    	{
    		if (HAL_DMA2D_Start(&hdma2d, color, hltdc.LayerCfg[0].FBStartAdress + (2*((Xpos-x) + 240*(Ypos-y)))
   				, 1, 1) == HAL_OK)
    	    {
      			HAL_DMA2D_PollForTransfer(&hdma2d, 10);
    		}
    	}*/
        *(__IO uint16_t*) (hltdc.LayerCfg[0].FBStartAdress + (2*((Xpos-x) + 240*(Ypos+y)))) = color;
        *(__IO uint16_t*) (hltdc.LayerCfg[0].FBStartAdress + (2*((Xpos+x) + 240*(Ypos+y)))) = color;
        *(__IO uint16_t*) (hltdc.LayerCfg[0].FBStartAdress + (2*((Xpos+x) + 240*(Ypos-y)))) = color;
        *(__IO uint16_t*) (hltdc.LayerCfg[0].FBStartAdress + (2*((Xpos-x) + 240*(Ypos-y)))) = color;

        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    }
    while (x <= 0);
}

void LCD_DrawPixel(uint32_t x, uint32_t y, uint32_t color)
{
	hdma2d.Init.Mode = DMA2D_R2M;
	hdma2d.Init.OutputOffset = 0;
	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK)
	{
		if (HAL_DMA2D_Start(&hdma2d, color, hltdc.LayerCfg[0].FBStartAdress + (3*(y*hltdc.LayerCfg[0].ImageWidth + x))
				, 1, 1) == HAL_OK)
		{
			HAL_DMA2D_PollForTransfer(&hdma2d, 10);
		}
	}
	//*(__IO uint32_t*) (hltdc.LayerCfg[0].FBStartAdress + (3*(y*hltdc.LayerCfg[0].ImageWidth + x))) = color;
}

void LCD_DrawCross(uint32_t x, uint32_t y, uint32_t color)
{
	LCD_DrawPixel(x, y, color);
	for (uint8_t i = 1; i < 4; i++)
		LCD_DrawPixel(x+i, y, color);
	for (uint8_t i = 1; i < 4; i++)
		LCD_DrawPixel(x, y+i, color);
	for (uint8_t i = 1; i < 4; i++)
		LCD_DrawPixel(x-i, y, color);
	for (uint8_t i = 1; i < 4; i++)
		LCD_DrawPixel(x, y-i, color);
}

void LCD_DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
	int steep = abs(y2-y1)>abs(x2-x1);
	if (steep)
	{
		swap(x1,y1);
		swap(x2,y2);
	}
	if (x1>x2)
	{
		swap(x1,x2);
		swap(y1,y2);
	}
		int dx,dy;
		dx=x2-x1;
		dy=abs(y2-y1);
		int err=dx/2;
		int ystep;
		if(y1<y2) ystep = 1;
	else  ystep = -1;
	for (;x1<=x2;x1++)
	{
		if (steep) LCD_DrawPixel(y1,x1,color);
		else LCD_DrawPixel(x1,y1,color);
		err-=dy;
		if (err<0)
		{
			y1 += ystep;
			err+=dx;
		}
	}
}

void LCD_FontsInit(void)
{
	lcdProp.BackColor = LCD_COLOR_BLACK;
	lcdProp.TextColor = LCD_COLOR_GREEN;
	lcdProp.pFont = &Font16;
}

void LCD_SetFont(sFONT *fonts)
{
	lcdProp.pFont = fonts;
}

void LCD_SetTextColor(uint32_t color)
{
	lcdProp.TextColor = color;
}

void LCD_SetBackColor(uint32_t color)
{
	lcdProp.BackColor = color;
}

void LCD_DrawChar(uint16_t x, uint16_t y, const uint8_t c)
{
  uint16_t height, width;
  uint8_t offset;
  uint8_t *pchar;
  uint32_t line;

  ch = &lcdProp.pFont->table[(c-' ') * lcdProp.pFont->Height * ((lcdProp.pFont->Width + 7) / 8)];
  height = lcdProp.pFont->Height;
  width = lcdProp.pFont->Width;
  offset = 8*((width + 7)/8) - width;

  for (uint32_t i = 0; i < height; i++)
  {
	  pchar = ((uint8_t *)ch + (width + 7)/8 * i);
	  switch ((width + 7)/8)
	  {
	  	  case 1:
	  		  line = pchar[0];
	  		  break;
	  	  case 2:
	  		  line = (pchar[0] << 8) | pchar[1];
	  		  break;
	  	  case 3:
	  	  default:
	  		  line = (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
	  		  break;
	  }

	  for (uint32_t j = 0; j < width; j++)
	  {
		  if (line & (1 << (width- j + offset- 1)))
		  {
			  LCD_DrawPixel((x + j), y, lcdProp.TextColor);
		  }
		  else
		  {
			  LCD_DrawPixel((x + j), y, lcdProp.BackColor);
		  }
	  }
	  y++;
  }
}

void LCD_DrawString(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode)
{
	uint16_t ref_column = 1, i = 0;
	uint32_t size = 0, xsize = 0;
	uint8_t *ptr = Text;
	while (*ptr++) size++;
	xsize = (X_SIZE/lcdProp.pFont->Width);

	switch (Mode)
	{
		case CENTER_MODE:
			ref_column = Xpos + ((xsize - size) * lcdProp.pFont->Width) / 2;
			break;
		case RIGHT_MODE:
			ref_column = - Xpos + ((xsize - size) * lcdProp.pFont->Width);
			break;
		case LEFT_MODE:
		default:
			ref_column = Xpos;
			break;
	}

	if ((ref_column < 1) || (ref_column >= 0x8000))
	{
		ref_column = 1;
	}
	while ((*Text != 0) & (((X_SIZE - (i*lcdProp.pFont->Width)) & 0xFFFF) >= lcdProp.pFont->Width))
	{
		LCD_DrawChar(ref_column, Ypos, *Text);
		ref_column += lcdProp.pFont->Width;
		Text++;
		i++;
	}
}


void LCD_DrawImage(uint16_t x_pos, uint16_t y_pos, uint16_t W, uint16_t H, const uint32_t* bitmap) {

    uint16_t xp = 0;
    uint16_t yp = 0;
    uint32_t index = 0;
    for (xp = 0; xp < H; xp++) {
        for (yp = 0; yp < W; yp++) {
        	LCD_DrawPixel(xp, yp, bitmap[index++]);
        }
    }
}
