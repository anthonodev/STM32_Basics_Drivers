#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adxl345.h"

int16_t x,y,z;
float xg, yg, zg;
uint8_t data_rec[6];

/*
 * PINOUT ADXL345 SPI:
 * CS  -> PA9 (SS)
 * SDO -> PA6 (MISO)
 * SDA -> PA7 (MOSI)
 * SCL -> PA5 (CLK)
 * VCC -> +5v
 * GND -> GND
 */
int main(void)
{
	uart2_rxtx_init();
	adxl_init();
	while(1)
	{
		adxl_read(DATA_START_ADDR, data_rec);
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




