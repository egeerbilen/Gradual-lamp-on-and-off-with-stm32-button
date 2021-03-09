#include "stm32f10x.h"

int val = 0;
int Counter = 0;
void TIM1_UP_IRQHandler()
{
	if (TIM1->SR &= ~1)
	{
		TIM1->SR &= ~1;
		if (Counter <= 2000)
		{
			if (Counter % 200 == 0)
			{
				val = val + 360;
				TIM1->CCR2 = val;
			}
			Counter++;
		}
		else if ((Counter > 2000) && (Counter <= 4000))
		{
			if (Counter % 200 == 0)
			{
				val = val - 360;
				TIM1->CCR2 = val;
			}
			Counter++;
		}
		else
			Counter = 1;
	}
}
int main()
{
	RCC->APB2ENR |= (1 << 2) | 1;
	GPIOA->CRH &= 0XFFFFFF0F;
	GPIOA->CRH |= 0X000000B0;

	RCC->APB2ENR |= (1 << 11); //timer aktif etme
	TIM1->CCMR1 |= (6 << 12);  //PA_9 için pwm mode 1
	TIM1->CCER |= (1 << 4);	   //çikis ayarlandi (kanal 1 için)
	TIM1->BDTR |= (1 << 15);
	TIM1->ARR = 3600; //20kh
	TIM1->PSC = 9;	  //(ARR*(PSC+1))/72000000
	TIM1->RCR = 0;
	TIM1->CCR1 = val; //parlaklik
	TIM1->DIER |= 1;
	NVIC->ISER[0] |= (1 << 25); //interupt metodu aktif etti
	TIM1->CR1 |= 1;				//timer aktif
	while (1)
	{
	}
}
