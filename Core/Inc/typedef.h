#ifndef __TYPEDEF_H
#define __TYPEDEF_H

#include "main.h"
typedef struct
			{
				uint8_t count;		// ������� PWM
				uint8_t countMax;   // ������ PWM, ������������ ������� 
				uint8_t enable;     // ����������
				uint8_t state; 	    // ���������
				uint8_t value;		// �������� PWM
			}pwmStateHeader;


#endif /* __TYPEDEF_H */
