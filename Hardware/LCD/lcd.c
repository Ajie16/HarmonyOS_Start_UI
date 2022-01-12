#include "lcd.h"
#include "gpio.h"
#include "spi.h"
#include "cmsis_os.h"

extern osSemaphoreId_t DMA_SemaphoreHandle;


/* USER CODE BEGIN 1 */
/**
 * @brief    SPI �����ֽں���
 * @param    TxData	Ҫ���͵�����
 * @param    size	�������ݵ��ֽڴ�С
 * @return  0:д��ɹ�,����:д��ʧ��
 */
uint8_t SPI_WriteByte(uint8_t *TxData,uint16_t size)
{
	
	osStatus_t result;
////��ȡ�źţ������һ��DMA�������
////�źž��ܻ�ȡ����û�д����������͹���
////�ȵ���������ٻָ�
	result = osSemaphoreAcquire(DMA_SemaphoreHandle,0xFFFF);
//	return HAL_SPI_Transmit(&hspi2,TxData,size,0xFF);
	if(result == osOK)
	{
		//��ȡ�ɹ�
		return HAL_SPI_Transmit_DMA(&hspi2,TxData,size);
	}else
	{
		//��ȡʧ��
		return 1;
	}
}
//DMA ������ɺ����� SPI������ɻص�����
//�ڸú����������ͷ��ź�
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == hspi2.Instance)
		osSemaphoreRelease(DMA_SemaphoreHandle);
}


/**
 * @brief   д���LCD
 * @param   cmd ���� ��Ҫ���͵�����
 * @return  none
 */
static void LCD_Write_Cmd(uint8_t cmd)
{
    LCD_WR_RS(0);
    SPI_WriteByte(&cmd, 1);
}

/**
 * @brief   д���ݵ�LCD
 * @param   dat ���� ��Ҫ���͵�����
 * @return  none
 */
static void LCD_Write_Data(uint8_t dat)
{
    LCD_WR_RS(1);
    SPI_WriteByte(&dat, 1);
}

/**
 * @breif   ��LCD��ʾ����
 * @param   none
 * @return  none
 */
void LCD_DisplayOn(void)
{
    LCD_PWR(1);
}
/**
 * @brief   �ر�LCD��ʾ����
 * @param   none
 * @return  none
 */
void LCD_DisplayOff(void)
{
    LCD_PWR(0);
}

/**
 * @brief   ��������д��LCD�Դ�����
 * @param   x1,y1	���� �������
 * @param   x2,y2	���� �յ�����
 * @return  none
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    /* ָ��X����������� */
    LCD_Write_Cmd(0x2a);
    LCD_Write_Data(x1 >> 8);
    LCD_Write_Data(x1);
    LCD_Write_Data(x2 >> 8);
    LCD_Write_Data(x2);

    /* ָ��Y����������� */
    LCD_Write_Cmd(0x2b);
    LCD_Write_Data(y1 >> 8);
    LCD_Write_Data(y1);
    LCD_Write_Data(y2 >> 8);
    LCD_Write_Data(y2);

    /* ���͸����LCD��ʼ�ȴ������Դ����� */
    LCD_Write_Cmd(0x2C);
}

/**
 * @brief   ��һ����ɫ���LCD��
 * @param   color ���� ������ɫ(16bit)
 * @return  none
 */
void LCD_Clear(uint16_t color)
{
    uint16_t i;
    uint8_t data[2] = {0};  //color��16bit�ģ�ÿ�����ص���Ҫ�����ֽڵ��Դ�

    /* ��16bit��colorֵ�ֿ�Ϊ�����������ֽ� */
    data[0] = color >> 8;
    data[1] = color;
    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);
		LCD_WR_RS(1);
		for(i=0;i<((LCD_Width)*(LCD_Height));i++)
		{
			SPI_WriteByte(data, 2);
		}
}

/**
 * @brief   LCD��ʼ��
 * @param   none
 * @return  none
 */
void LCD_Init(void)
{
		/* ��ʼ����LCDͨ�ŵ����� */
		
		/* ��λLCD */
		LCD_PWR(0);
		LCD_RST(0);
		osDelay(100);
		LCD_RST(1);
		osDelay(120);
    /* �ر�˯��ģʽ */
    LCD_Write_Cmd(0x11);
    osDelay(120);

    /* ��ʼ�����Դ�ɨ��ģʽ�����ݸ�ʽ�� */
    LCD_Write_Cmd(0x36);
    LCD_Write_Data(0x00);
    /* RGB 5-6-5-bit��ʽ  */
    LCD_Write_Cmd(0x3A);
    LCD_Write_Data(0x65);
    /* porch ���� */
    LCD_Write_Cmd(0xB2);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x33);
    LCD_Write_Data(0x33);
    /* VGH���� */
    LCD_Write_Cmd(0xB7);
    LCD_Write_Data(0x72);
    /* VCOM ���� */
    LCD_Write_Cmd(0xBB);
    LCD_Write_Data(0x3D);
    /* LCM ���� */
    LCD_Write_Cmd(0xC0);
    LCD_Write_Data(0x2C);
    /* VDV and VRH ���� */
    LCD_Write_Cmd(0xC2);
    LCD_Write_Data(0x01);
    /* VRH ���� */
    LCD_Write_Cmd(0xC3);
    LCD_Write_Data(0x19);
    /* VDV ���� */
    LCD_Write_Cmd(0xC4);
    LCD_Write_Data(0x20);
    /* ��ͨģʽ���Դ��������� 60Mhz */
    LCD_Write_Cmd(0xC6);
    LCD_Write_Data(0x0F);
    /* ��Դ���� */
    LCD_Write_Cmd(0xD0);
    LCD_Write_Data(0xA4);
    LCD_Write_Data(0xA1);
    /* ��ѹ���� */
    LCD_Write_Cmd(0xE0);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2B);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x54);
    LCD_Write_Data(0x4C);
    LCD_Write_Data(0x18);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x0B);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x23);
    /* ��ѹ���� */
    LCD_Write_Cmd(0xE1);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2C);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x44);
    LCD_Write_Data(0x51);
    LCD_Write_Data(0x2F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x20);
    LCD_Write_Data(0x23);
    /* ��ʾ�� */
    LCD_Write_Cmd(0x21);
    LCD_Write_Cmd(0x29);

    /*����ʾ*/
    LCD_PWR(1);
}


