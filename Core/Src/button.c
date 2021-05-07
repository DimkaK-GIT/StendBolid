/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : button.c
  * @brief          : One button
  ******************************************************************************
  * @attention
  *
  * 
  * 
  * 
  * 
  *
  ******************************************************************************
  */
#include "main.h"
#include "button.h"

//------------------------------------------------------------------------------------------
//  Инициализация структуры 
//------------------------------------------------------------------------------------------
void KeyInit(buttonStructHeader *Keys, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint16_t shortDelay, uint16_t longDelay)
{
	Keys->GPIOx = GPIOx;
	Keys->GPIO_Pin = GPIO_Pin;
	Keys->counterShort = shortDelay;
	Keys->counterLong = longDelay;
	Keys->counter = 0;
	
}

uint8_t ReadKey(buttonStructHeader *Keys)
{
	uint8_t tmp = Keys->state;	
	if(tmp == shortPress) // shortPress
		Keys->state = 0;
	
	return tmp;
}

void ProcessKey (buttonStructHeader *Keys)
{
	uint8_t pinState = HAL_GPIO_ReadPin(Keys->GPIOx,Keys->GPIO_Pin);
	if(pinState == GPIO_PIN_SET)
	{
		if(Keys->counter > Keys->counterLong)
			Keys->state = noPress;
		else if ((Keys->counter < Keys->counterLong) & (Keys->counter > Keys->counterShort))
			Keys->state = shortPress;
		else
			Keys->state = noPress;
		
		Keys->counter = 0;
	}
	else
	{
		Keys->counter++;
		if(Keys->counter > 65530)
			Keys->counter--;
		
		if(Keys->counter > Keys ->counterLong)
			Keys->state = longPress;
		
	}
}