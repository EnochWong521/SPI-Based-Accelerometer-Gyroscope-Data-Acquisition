/*
* File Name: lab6_3.c
* Name: Enoch Wong
* PI Name: Matthew Stormant
* Class Number: 11645
* Description: Reads from the WHO_AM_I register
*/

#include <avr/io.h>
#include "spi.h"
#include "lsm6dsl.h"
#include "lsm6dsl_registers.h"

int main(void) {
	uint8_t data;
	spi_init();
	while(1) {
		/* read value from WHO_AM_I register */
		data = LSM_read(WHO_AM_I);
	}
	return 0;
}