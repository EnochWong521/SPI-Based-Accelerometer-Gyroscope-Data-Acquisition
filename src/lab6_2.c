/*
* File Name: lab6_2.c
* Name: Enoch Wong
* PI Name: Matthew Stormant
* Class Number: 11645
* Description: Continuously transmits 0x96 to SPI 
*/

/********************************DEPENDENCIES**********************************/

#include <avr/io.h>
#include "spi.h"

/*****************************END OF DEPENDENCIES******************************/

/*Start program*/

int main(void) {
	spi_init();
	while(1) {
		/* set PF1 to low to simulate IMU student device being activated */
		PORTF.OUTCLR = SS_bm;
		/* write then read the value 0x96 */
		spi_write(0x96);
		spi_read();
		PORTF.OUTSET = SS_bm;
		/* set PF1 to high to simulate IMU student device being disabled */
	}
	return 0;
}