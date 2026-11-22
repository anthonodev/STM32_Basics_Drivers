#include "adxl345.h"

/*
 *  MULTY_BYTE_EN 0x40 correspond to MB bit (position 6) (multiple-byte bit, located after the R/W bit in the first byte transfer).
 *  See §figure 37 to 39, page 14/15 of adxl345_Datasheet.pdf file.
 *  MULTY_BYTE_EN -> 1 = ACTIVE, 0 = INACTIVE
 *
 *  READ_OPERATION 0x80 correspond to the R:W bit  (position 7). See §figure 37 to 39, page 14/15 of adxl345_Datasheet.pdf file.
 *  READ_OPERATION -> 1 = READ, 0 = WRITE
 */

#define MULTY_BYTE_EN	0x40
#define READ_OPERATION	0x80

void adxl_read(uint8_t address, uint8_t * rxdata)
{
	/*Set read operation*/
	address |= READ_OPERATION;
	/*Enable multi-byte, place address into buffer*/
	address |= MULTY_BYTE_EN;
	/*Pull cs line low to enable slave*/
	cs_enable();
	/*Send address*/
	spi1_transmit(&address,1);
	/*Read 6 bytes*/
	spi1_receive(rxdata, 6);
	/*Pull cs line low to disable slave*/
	cs_disable();
}

void adxl_write(uint8_t address, char value)
{
	uint8_t data[2];
	/*Enable multi-byte, place address into buffer*/
	data[0] = address|MULTY_BYTE_EN;
	/*Place data into buffer*/
	data[1] = value;
	/*Pull cs line low to enable slave*/
	cs_enable();
	/*Transmit data and address*/
	spi1_transmit(data, 2);
	/*Pull cs line low to disable slave*/
	cs_disable();
}

void adxl_init(void)
{
	/*Enable SPI GPIO*/
	spi_gpio_init();
	/*Config SPI*/
	spi1_config();
	/*Set data format range to +- 4g*/
	adxl_write(DATA_FORMAT_R, FOUR_G);
	/*Reset all bits*/
	adxl_write(POWER_CTL_R, RESET);
	/*Configure power control mesure bit*/
	adxl_write(POWER_CTL_R, SET_MEASURE_B);
}
