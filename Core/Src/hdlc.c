#include "hdlc.h"
#include "main.h"


extern UART_HandleTypeDef huart2;

interfaceHDLC_TX masterTX;
interfaceHDLC_RX masterRX;

//-------------------------------------------------------------
// ��������� ��������� ����� � ����� �������� � �������  
//-------------------------------------------------------------
unsigned add_data_tx_buff (unsigned char data, unsigned int index)
{
	if(data == 0x7e)
	{  
		masterTX.master_buff_tx[index] = 0x7d;
		masterTX.master_buff_tx[index+1] = 0x5e; // ���� ������� ���� 0x7e ������ �� ��� 0x7d 0x5e
		return 2;
	}
	else
	{
		if(data == 0x7d)
		{
			masterTX.master_buff_tx[index] = 0x7d;
			masterTX.master_buff_tx[index+1] = 0x5d;// ���� ������� ���� 0x7d ������ �� ��� 0x7d 0x5d
			return 2;
		}
		else
		{
			masterTX.master_buff_tx[index] = data; // ����� ���������� ��� ����
			return 1;
		}			
	}		
}

//-------------------------------------------------------------
// ��������� ���������� ������ ��� �������� ��������� ������� � ����� �������
//-------------------------------------------------------------
unsigned int send_Master_HDLC (struct_HDLC_Header MasterHDLC,char Command, char* data, int count_data)
{

	unsigned int count_master_TX;				      // ������ �����, ���� ������ ������
	uint16_t crcSend = CRC16((uint8_t*)data,count_data);

	count_master_TX = 0;
	masterTX.master_buff_tx[count_master_TX++] = 0x7e;	              // ������ ������ � ����� �������

	count_master_TX += add_data_tx_buff(MasterHDLC.ADR_DST,count_master_TX); // �������� ����� ���������� �������
	count_master_TX += add_data_tx_buff(MasterHDLC.ADR_SRC,count_master_TX); // �������� ����� �����������
	count_master_TX += add_data_tx_buff(Command,count_master_TX); // �������� ����� �����������
	
	for (unsigned int index = 0; index < count_data; index++)
	{
		count_master_TX += add_data_tx_buff(data[index],count_master_TX); // ������ �������
	}
	// �������� ��������� �������� CRC
	count_master_TX += add_data_tx_buff((uint8_t)(crcSend & 0x00FF),count_master_TX); // ������ �������
	count_master_TX += add_data_tx_buff((uint8_t)((crcSend & 0xFF00)>>8),count_master_TX); // ������ �������
	
	masterTX.master_buff_tx[count_master_TX++] = 0x7e;	              // ������ ������ � ����� �������
	
	HAL_UART_Transmit(&huart2, masterTX.master_buff_tx, count_master_TX, 4000);	
	return count_master_TX;
}

//-------------------------------------------------------------
//  ��������� ��������� �����
//-------------------------------------------------------------
void recHDLC (uint8_t data)
{
   uint16_t CRC_in, CRC_calk;
	 switch (data)
   {
      case 0x7E:

          if (masterRX.RX_StartFlag == 1)
          {
              masterRX.RX_7EFlag = 0;
              // ������� ��� �������, ���������
              if (masterRX.count_RX > 3)
              {
                  CRC_in = (masterRX.master_buff_rx[masterRX.count_RX - 2] + masterRX.master_buff_rx[masterRX.count_RX - 1] * 256);
                  CRC_calk = CRC16(masterRX.master_buff_rx,masterRX.count_RX-2);
									CRC_in = 0;
									CRC_calk = 0;
                  if (CRC_in == CRC_calk)
                  {
                      masterRX.ADR_DSC = masterRX.master_buff_rx[0];
                      masterRX.ADR_SRC = masterRX.master_buff_rx[1];
                      masterRX.Command = masterRX.master_buff_rx[2];
											masterRX.argCount = masterRX.count_RX - 5;
                      for (int i = 0; i < masterRX.argCount; i++)
                        masterRX.arg[i] = masterRX.master_buff_rx[i + 3];
                  }
									masterRX.RX_StartFlag = 0;
									masterRX.RX_7DFlag = 0;
									masterRX.RX_7EFlag = 0;
									masterRX.count_RX = 0;
									 	

               }

            }
            else
            {
                masterRX.RX_StartFlag = 1;
                masterRX.count_RX = 0;


            }
            break;

        case 0x7D:
            masterRX.RX_7DFlag = 1;
            break;

        case 0x5E:

            if (masterRX.RX_StartFlag == 1)
            {
                if (masterRX.RX_7DFlag == 1)
                {
                    masterRX.master_buff_rx[masterRX.count_RX++] = 0x7E;
									  masterRX.RX_7DFlag = 0;
                }
                else
                {
                    masterRX.master_buff_rx[masterRX.count_RX++] = 0x5E;
                }
            }
            break;
        case 0x5D:

            if (masterRX.RX_StartFlag == 1)
            {
                if (masterRX.RX_7DFlag == 1)
                {
                    masterRX.master_buff_rx[masterRX.count_RX++] = 0x7D;
									  masterRX.RX_7DFlag = 0;
                }
                else
                {
                    masterRX.master_buff_rx[masterRX.count_RX++] = 0x5D;
                }
            }
            break;

        default:

            if(masterRX.RX_StartFlag == 1)
                masterRX.master_buff_rx[masterRX.count_RX++] = data;

            if (masterRX.count_RX > MAX_HDLC_RX_BUFFER)
            {
                masterRX.RX_StartFlag = 0;
                masterRX.RX_7DFlag = 0;
                masterRX.count_RX = 0;
            }
            break;


    }
}

//-------------------------------------------------------------
// ������� CRC16
//-------------------------------------------------------------
uint16_t CRC16(uint8_t * data, int count)
{
		uint16_t crc = 0xFFFF;
		int i, index;

		for (index = 0; index < count; index++)
		{
				crc ^= (data[index] << 8);

				for (i = 0; i < 8; i++)
						crc = (crc & 0x8000) == 0x8000 ? ((crc << 1) ^ 0x1021) : (crc << 1);
		}

		return crc;
}
