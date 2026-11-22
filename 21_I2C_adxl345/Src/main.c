#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adxl345.h"

int16_t x,y,z;
float xg, yg, zg;
extern uint8_t data_rec[6];
/*
 * Be carefull, using debugger with I2C component introduces a glitch random behavior.
 * I2C seems do not like software debbuging. Prefer using an oscilloscope or
 * I2C bus analyzer to do so. Debug by print using uart is usefull here.
 * This driver is not enough smart to carry I2C error communication.
 * May consider to upgrade later using for exemple I2C_CR1 -> SWRST bit
 * when the I2C bus is freezing.
 */

/*
 * PINOUT ADXL345 I2C:
 * PB8 -> SCL
 * PB9 -> SDA
 * VCC -> +5v
 * GND -> GND
 */
int main(void)
{
	uart2_rxtx_init();
	adxl_init();
	while(1)
	{
		adxl_read_values(DATA_START_ADDR);
		x = ((data_rec[1]<<8)|data_rec[0]); //combine x0,x1
		y = ((data_rec[5]<<8)|data_rec[2]); //combine y0,y1
		z = ((data_rec[5]<<8)|data_rec[4]); //combine z0,z1

		//FOUR_G_SCALE_FACT = 0.0078
		xg = (x*0.0078);
		yg = (y*0.0078);
		zg = (z*0.0078);
		printf("ADXL345 Sensor value :\n\r x = %d\n\r y = %d\n\r z = %d\n\r \n\r", x,y,z);
	}
}




