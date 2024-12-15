#ifndef QSPI_w25q64_H
#define QSPI_w25q64_H

#include "stm32h7xx_hal.h"
#include "usart.h"

/*----------------------------------------------- ���������� -------------------------------------------*/

#define QSPI_W25Qxx_OK           		0		// W25Qxxͨ������
#define W25Qxx_ERROR_INIT         		-1		// ��ʼ������
#define W25Qxx_ERROR_WriteEnable       -2		// дʹ�ܴ���
#define W25Qxx_ERROR_AUTOPOLLING       -3		// ��ѯ�ȴ���������Ӧ
#define W25Qxx_ERROR_Erase         		-4		// ��������
#define W25Qxx_ERROR_TRANSMIT         	-5		// �������
#define W25Qxx_ERROR_MemoryMapped		-6    // �ڴ�ӳ��ģʽ����

#define W25Qxx_CMD_EnableReset  		0x66		// ʹ�ܸ�λ
#define W25Qxx_CMD_ResetDevice   	0x99		// ��λ����
#define W25Qxx_CMD_JedecID 			0x9F		// JEDEC ID  
#define W25Qxx_CMD_WriteEnable		0X06		// дʹ��

#define W25Qxx_CMD_SectorErase 		0x21		// ����������4K�ֽڣ� �ο�����ʱ�� 45ms
#define W25Qxx_CMD_BlockErase_64K 	0xDC		// �������  64K�ֽڣ��ο�����ʱ�� 150ms
#define W25Qxx_CMD_ChipErase 			0xC7		// ��Ƭ�������ο�����ʱ�� 80S

#define W25Qxx_CMD_QuadInputPageProgram  	0x34  		// 1-1-4ģʽ��(1��ָ��1�ߵ�ַ4������)��ҳ���ָ��ο�д��ʱ�� 0.4ms 
#define W25Qxx_CMD_FastReadQuad_IO       	0xEC  		// 1-4-4ģʽ��(1��ָ��4�ߵ�ַ4������)�����ٶ�ȡָ��

#define W25Qxx_CMD_ReadStatus_REG1			0X05			// ��״̬�Ĵ���1
#define W25Qxx_Status_REG1_BUSY  			0x01			// ��״̬�Ĵ���1�ĵ�0λ��ֻ������Busy��־λ�������ڲ���/д������/д����ʱ�ᱻ��1
#define W25Qxx_Status_REG1_WEL  				0x02			// ��״̬�Ĵ���1�ĵ�1λ��ֻ������WELдʹ�ܱ�־λ���ñ�־λΪ1ʱ��������Խ���д����

#define W25Qxx_PageSize       				256			// ҳ��С��256�ֽ�
#define W25Qxx_FlashSize       				0x2000000	// W25Q256��С��32M�ֽ�
#define W25Qxx_FLASH_ID           			0Xef4019    // W25Q256 JEDEC ID
#define W25Qxx_ChipErase_TIMEOUT_MAX		400000U		// ��ʱ�ȴ�ʱ�䣬W25Q256��Ƭ�����������ʱ����400S,
#define W25Qxx_Mem_Addr							0x90000000 	// �ڴ�ӳ��ģʽ�ĵ�ַ




/*----------------------------------------------- �������� ---------------------------------------------------*/

int8_t	QSPI_W25Qxx_Init(void);						// W25Qxx��ʼ��
int8_t 	QSPI_W25Qxx_Reset(void);					// ��λ����
uint32_t QSPI_W25Qxx_ReadID(void);					// ��ȡ����ID


int8_t	QSPI_W25Qxx_Test(void);						//Flash��д����
	
int8_t 	QSPI_W25Qxx_MemoryMappedMode(void);		// �����ڴ�ӳ��ģʽ
	
int8_t 	QSPI_W25Qxx_SectorErase(uint32_t SectorAddress);		// ����������4K�ֽڣ� �ο�����ʱ�� 45ms
int8_t 	QSPI_W25Qxx_BlockErase_64K (uint32_t SectorAddress);	// �������  64K�ֽڣ��ο�����ʱ�� 150ms��ʵ��ʹ�ý���ʹ��64K������������ʱ�����
int8_t 	QSPI_W25Qxx_ChipErase (void);                         // ��Ƭ�������ο�����ʱ�� 20S

int8_t	QSPI_W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);	// ��ҳд�룬���256�ֽ�
int8_t	QSPI_W25Qxx_WriteBuffer(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);				// д�����ݣ�����ܳ���flashоƬ�Ĵ�С
int8_t 	QSPI_W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);	// ��ȡ���ݣ�����ܳ���flashоƬ�Ĵ�С



#endif // QSPI_w25q64_H 





