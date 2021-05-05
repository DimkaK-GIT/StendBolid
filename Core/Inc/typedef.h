#ifndef __TYPEDEF_H
#define __TYPEDEF_H

#include "main.h"
typedef struct
			{
				uint8_t count;		// счетчик PWM
				uint8_t countMax;   // период PWM, максимальный счетчик 
				uint8_t enable;     // разрешение
				uint8_t state; 	    // состояние
				uint8_t value;		// значение PWM
			}pwmStateHeader;


#endif /* __TYPEDEF_H */
