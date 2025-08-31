/*
* File Name: lsm6dsl.c
* Name: Enoch Wong
* PI Name: Matthew Stormant
* Class Number: 11645
* Description: LSM write and read functions
*/


/********************************DEPENDENCIES**********************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi.h"
#include "lsm6dsl.h"
#include "lsm6dsl_registers.h"

/*****************************END OF DEPENDENCIES******************************/


/*****************************FUNCTION DEFINITIONS*****************************/
void LSM_write(uint8_t reg_addr, uint8_t data) {
	PORTF.OUTCLR = SS_bm;
	spi_write(reg_addr | LSM6DSL_SPI_WRITE_STROBE_bm);
	spi_write(data);
	PORTF.OUTSET = SS_bm;
}

uint8_t LSM_read(uint8_t reg_addr) {
	PORTF.OUTCLR = SS_bm;
	spi_write(reg_addr | LSM6DSL_SPI_READ_STROBE_bm);
	uint8_t data = spi_read();
	PORTF.OUTSET = SS_bm;
	return data;
}

/***************************END OF FUNCTION DEFINITIONS************************/


