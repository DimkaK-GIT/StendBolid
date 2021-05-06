#ifndef __BUTTON_H
#define __BUTTON_H
#include "main.h"

#define noPress 0
#define shortPress 1
#define longPress 2

typedef struct
{
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
	uint16_t counter;
	uint16_t counterShort;
	uint16_t counterLong;
	uint8_t state;
	
}buttonStructHeader;

void KeyInit(buttonStructHeader *Keys, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint16_t shortDelay, uint16_t longDelay);
uint8_t ReadKey(buttonStructHeader *Keys);
void ProcessKey (buttonStructHeader *Keys);

#endif // __BUTTON_H
