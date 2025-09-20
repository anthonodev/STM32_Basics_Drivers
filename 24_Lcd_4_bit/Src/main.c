//PC4 - PC7 : Data lines
//PB5 = RS
//GND : R/W
//PB7 : EN

#include "stm32f4xx.h"

#define RS 0x20 // 0010 0000b
#define EN 0x80 // 1000 0000b

#define GPIOBEN (1U<<1)
#define GPIOCEN (1U<<2)

/*BEGIN Prototype definitions*/
void LCD_Init(void);
void GPIO_Init(void);
void LCD_command(unsigned char command);
void LCD_data(char data);
void delayMS(int delay);
/*END Prototype definitions*/

int main(void)
{
	LCD_Init();
	while(1)
	{
		LCD_data('S');
		LCD_data('a');
		LCD_data('l');
		LCD_data('u');
		LCD_data('t');
		delayMS(500); // For blinking 'Salut'
	}
}

void LCD_Init(void)
{
	GPIO_Init();
	//Init sequence
	delayMS(30);
	//LCD_nibble_write(0x30,0);
	LCD_command(0x30);
	delayMS(3);
	//LCD_nibble_write(0x30,0);
	LCD_command(0x30);
	delayMS(3);
	//LCD_nibble_write(0x30,0);
	LCD_command(0x30);
	delayMS(3);
	LCD_command(0x28); // 4 bit mode, 2-line, 5x7 font
	delayMS(3);
	LCD_command(0x06); //mover cursor
	delayMS(3);
	LCD_command(0x01); //clear screen move cursor home
	delayMS(3);
	LCD_command(0x0F); //turn ON display and blink cursor
}

void GPIO_Init(void)
{
	RCC->AHB1ENR |= (GPIOBEN | GPIOCEN);
	/*Mode register*/
	/*PB5 -> : General purpose output mode*/
	GPIOB->MODER |=(1U<<10);
	GPIOB->MODER &=~(1U<<11);
	/*R/W is already set to GND*/
	/*PB7 -> : General purpose output mode*/
	GPIOB->MODER |=(1U<<14);
	GPIOB->MODER &=~(1U<<15);
	/*Set EN low*/
	GPIOB->BSRR = EN<<16;//Reset EN to 0
	/*Set PC4 -PC7 as output*/
	GPIOC->MODER |= 0x00005500;
}

void LCD_command(unsigned char command)
{
	//GPIOC->BSRR = 0x00F00000;//Clear data bits
	GPIOC->ODR = ((command<<4) & 0xF0);
	GPIOB->BSRR = EN;//Set EN to 1
	//RW already set to GND -> 0
	GPIOB->BSRR = RS<<16;//Reset RS to 0
	delayMS(2);
	GPIOB->BSRR = EN<<16;//Reset EN to 0
	GPIOC->ODR = (command & 0xF0);
	GPIOB->BSRR = EN;//Set EN to 1
	//RW already set to GND -> 0
	GPIOB->BSRR = RS<<16;//Reset RS to 0
	delayMS(2);
	GPIOB->BSRR = EN<<16;//Reset EN to 0
	GPIOC->ODR = 0x0;
}

void LCD_data(char data)
{
	GPIOC->ODR = (data & 0xF0);
	GPIOB->BSRR = EN;//Set EN to 1
	//RW already set to GND -> 0
	GPIOB->BSRR = RS;//Set RS to 1
	delayMS(2);
	GPIOB->BSRR = EN<<16;//Reset EN to 0
	GPIOC->ODR = ((data<<4) & 0xF0);
	GPIOB->BSRR = EN;//Set EN to 1
	//RW already set to GND -> 0
	GPIOB->BSRR = RS;//Set RS to 1
	delayMS(2);
	GPIOB->BSRR = EN<<16;//Reset EN to 0
	GPIOC->ODR = 0x0;
}

void delayMS(int delay)
{
	int i;
	for (;delay>0;delay--)
	{
		for(i=0;i<3195;i++)
		{
		}
	}
}
