#ifndef HDLC_H
#define HDLC_H

#include <stdint.h>
#include "string.h"

#define MAX_HDLC_TX_BUFFER 200  				// ������ ������ �� �������� �� �������
#define MAX_HDLC_RX_BUFFER 200  				//������ ������ �� ����� �� �������

typedef struct HDLC_Header
      {
       unsigned char ADR_DST;             		//����� ���������� ������� �� Master
       unsigned char ADR_SRC;             		//����� ����������� ������� �� Master
      }struct_HDLC_Header; 

	  
typedef struct
			{
				uint16_t count_TX_Master;											// ������ ������������� ����� � ���� Master
				uint16_t TX_Master_Max;     									// ���������� ������������ ���� � ���� Master
				uint16_t Master_TX_Flag;                      // ���� �������� ������ � Master
				uint8_t master_buff_tx[MAX_HDLC_TX_BUFFER]; 	// ����� �� �������� Master
			}interfaceHDLC_TX;

typedef struct
			{
				uint16_t count_RX;														// ���������� �������� ����
				uint8_t RX_StartFlag;                         // ���� �������� ������ � Master
				uint8_t RX_7EFlag;														// ���� ������ ����� 7E
				uint8_t RX_7DFlag;														// ���� ������ ����� 7D
				uint8_t ADR_DSC;															// ����� ���������� ���������
				uint8_t ADR_SRC;															// ����� ��������� ���������
				uint8_t Command;															// �������� �������.
				uint8_t arg[MAX_HDLC_RX_BUFFER];						 	// ��������� �������� �������
				uint8_t master_buff_rx[MAX_HDLC_RX_BUFFER]; 	// ����� �� ����� Master
				uint16_t argCount;														// ���������� �������� ����������
			}interfaceHDLC_RX;



// ��������� �������:
void recHDLC (uint8_t data);
unsigned int send_Master_HDLC (struct_HDLC_Header MasterHDLC,char Command, char* data, int count_data);			
uint16_t CRC16(uint8_t * data, int count);
			
#endif // HDLC_H
