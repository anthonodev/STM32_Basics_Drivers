//PC0 - PC7 = D0 - D7
//PB5 = RS
//PB6 = R/W
//PB7 = EN

#include "stm32f4xx.h"

#define GPIOBEN (1U<<1)
#define GPIOCEN (1U<<2)

#define RS (1U<<5) //0x20 // 0010 0000b
#define RW (1U<<6) //0x40 // 0100 0000b
#define EN (1U<<7) //0x80 // 1000 0000b

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
		/*
		 * According to page 10 of the datasheet, we can see that the character mapping inside
		 * display device respect ascii character table. So, by putting the chars in parameter,
		 * of LCD_data function, the device will recognise the ascii code of characters and
		 * display it.
		 */
		LCD_data('H');
		LCD_data('e');
		LCD_data('l');
		LCD_data('l');
		LCD_data('o');

		//LCD_command(0x01);//clear the screen
		delayMS(500); // For blinking 'Hello'
	}
}
void LCD_Init(void)
{
	GPIO_Init();
	/*Init sequence*/
	delayMS(3);
	LCD_command(0x30);
	delayMS(3);
	LCD_command(0x30);
	delayMS(3);
	LCD_command(0x30);
	/*Set 8-bit data mode, 2-line, 5x7 font*/
	delayMS(3);
	LCD_command(0x38);
	/*Move cursor to the right side*/
	delayMS(3);
	LCD_command(0x06);
	/*Clear screen*/
	delayMS(3);
	LCD_command(0x01);
	/*Turn display, blink cursor*/
	delayMS(3);
	LCD_command(0x0F);
}

void GPIO_Init(void)
{
	/*Clock the GPIOB & GPIOC*/
	RCC->AHB1ENR |= (GPIOBEN | GPIOCEN);
	/*Mode register*/
	/*PB5 -> : General purpose output mode*/
	GPIOB->MODER |=(1U<<10);
	GPIOB->MODER &=~(1U<<11);
	/*PB6 -> : General purpose output mode*/
	GPIOB->MODER |=(1U<<12);
	GPIOB->MODER &=~(1U<<13);
	/*PB7 -> : General purpose output mode*/
	GPIOB->MODER |=(1U<<14);
	GPIOB->MODER &=~(1U<<15);
	/*Set EN and RW low*/
	GPIOB -> BSRR = 0x00C;
	/*Set PC0 - -PC7 as output*/
	GPIOC->MODER |= 0x00005555; // 0000 0000 0000 0000 0101 0101 0101 0101
}

void LCD_command(unsigned char command)
{
	GPIOB->BSRR = (RS|RW)<<16; //RS = 0 (command register), RW = 0 (writing) (RESET the corresponding ODR bits)
	GPIOC->ODR = command;
	GPIOB->BSRR = EN; //Set EN => According the datasheet (page 4), enable cycle time shall be minimum 1000ns = 1us.
	delayMS(1);//Here, we set 1ms for the half cycle, but for better performance, we should prefer putting 500 ns.
	GPIOB->BSRR = EN<<16; //Clear EN
	delayMS(1);//Here, we set 1ms for the half cycle, but for better performance, we should prefer putting 500 ns.
}

void LCD_data(char data)
{
	GPIOB->BSRR = RS;     //RS = 1 (data register)
	GPIOB->BSRR = RW<<16; //RW = 0 (writing)
	GPIOC->ODR = data;
	GPIOB->BSRR = EN; //Set EN => According the datasheet (page 4), enable cycle time shall be minimum 1000ns = 1us.
	delayMS(1);//Here, we set 1ms for the half cycle, but for better performance, we should prefer putting 500 ns.
	GPIOB->BSRR = EN<<16; //Clear EN
	delayMS(1);//Here, we set 1ms for the half cycle, but for better performance, we should prefer putting 500 ns.
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
