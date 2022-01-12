#include "main.h"

#define	LCD_PWR(n)		(n?\
						HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_SET):\
						HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_RESET))
#define	LCD_WR_RS(n)	(n?\
						HAL_GPIO_WritePin(LCD_WR_RS_GPIO_Port,LCD_WR_RS_Pin,GPIO_PIN_SET):\
						HAL_GPIO_WritePin(LCD_WR_RS_GPIO_Port,LCD_WR_RS_Pin,GPIO_PIN_RESET))
#define	LCD_RST(n)		(n?\
						HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET):\
						HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET))

//LCD��Ļ�ֱ��ʶ���
#define LCD_Width   240
#define LCD_Height  240
//��ɫ����
#define WHITE   0xFFFF	//��ɫ
#define YELLOW  0xFFE0	//��ɫ
#define BRRED   0XFC07  //�غ�ɫ
#define PINK    0XF81F	//��ɫ
#define RED     0xF800	//��ɫ
#define BROWN   0XBC40  //��ɫ
#define GRAY    0X8430  //��ɫ
#define GBLUE   0X07FF	//��ɫ
#define GREEN   0x07E0	//��ɫ
#define BLUE    0x001F  //��ɫ
#define BLACK   0x0000	//��ɫ


uint8_t SPI_WriteByte(uint8_t *TxData,uint16_t size);

static void LCD_Write_Cmd(uint8_t cmd);

static void LCD_Write_Data(uint8_t dat);

void LCD_DisplayOn(void);

void LCD_DisplayOff(void);

void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void LCD_ReFlash(void);

void LCD_Clear(uint16_t color);

void LCD_Init(void);

void LCD_Draw_Point(uint16_t x1, uint16_t y1, uint16_t color);

