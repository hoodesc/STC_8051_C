#ifndef	__NRF24L01__TX_H__
#define __NRF24L01__TX_H__

extern uchar SPI_RW(uchar dat);
extern uchar SPI_Read(uchar reg);
extern uchar SPI_RW_Reg(uchar reg,uchar value);
extern uchar SPI_Write_Buf(uchar reg,uchar *pBuf,uchar bytes);
extern void NRF24L01_TxPacket();
extern void NRF24L01_TX_Mode(void);

// SPIλ����
sbit SCK  = P1^7;		// SPIʱ��
sbit MISO = P1^6;		// ��������
sbit MOSI = P1^5;		// ����ӳ�
sbit CSN  = P1^4;		// оƬƬѡ��Ϊ��оƬ����
sbit IRQ  = P1^3;		// �ж��ź�
sbit CE   = P1^2;		// оƬģʽ������


// NRF24L01�Ĵ�����ַ
#define CONFIG 		0x00		// ���üĴ���
#define EN_AA 		0x01		// �Զ�Ӧ��
#define EN_RXADDR 	0x02		// ���յ�ַ����
#define SETUP_AW 	0x03		// ���õ�ַ��ȣ���������ͨ����
#define SETUP_RETR 	0x04		// �Զ��ط�
#define RF_CH 		0x05		// ��Ƶͨ��
#define RF_SETUP 	0x06		// ��Ƶ�Ĵ���
#define STATUS 		0x07		// ״̬�Ĵ���
#define OBSERVE_TX 	0x08		// ���ͼ��Ĵ���
#define CD 			0x09		// �ز����
#define RX_ADDR_P0 0x0A		// ����ͨ��0~5���յ�ַ
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F
#define TX_ADDR  0x10		// ���͵�ַ
#define RX_PW_P0 0x11		// ����ͨ��0~5�������ݳ���
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17		// FIFO״̬�Ĵ���

// NRF24L01����ָ��
#define READ_REG 	0x00		// ���Ĵ���ָ��
#define WRITE_REG 	0x20		// д�Ĵ���ָ��
#define RD_TX_PLOAD 0x61		// ��ȡ��������ָ��
#define WR_TX_PLOAD 0xA0		// д����������ָ��
#define FLUSE_TX 0xE1		 	// ���TX FIFO�Ĵ���
#define FLUSE_RX 0xE2			// ���RX FIFO�Ĵ���
#define REUSE_RX_PL 0xE3		// Ӧ�÷��䣬���·�����һ����Ч����
#define NOP 0xFF				// �ղ��� �����ڶ��Ĵ���״̬

#define RX_ADR_WIDTH 5		// �յ�ַ5���ֽڿ��
#define TX_ADR_WIDTH 5		// ����ַ5���ֽڿ��
#define RX_PLOAD_WIDTH 4	// ������32���ֽ�
#define TX_PLOAD_WIDTH 4	// ������32���ֽ�
#endif