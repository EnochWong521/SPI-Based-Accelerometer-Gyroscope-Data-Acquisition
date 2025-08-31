/*
* File Name: lab6_5.c
* Name: Enoch Wong
* PI Name: Matthew Stormant
* Class Number: 11645
* Description: Plots accelerometer data, with IMU initialization
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi.h"
#include "lsm6dsl.h"
#include "lsm6dsl_registers.h"
#include "usart.h"

volatile uint8_t accel_flag = 0;

void LSM_init(void){
	// IMU software reset
	LSM_write(CTRL3_C, 0b00000001);
	
	// set IMU interrupt activation level as active low and enable address incrementation during multiple byte access
	LSM_write(CTRL3_C, 0b00100100);
	
	// enable data in XYZ axis
	LSM_write(CTRL9_XL, 0b11100000);
	
	// set output data rate as 208Hz, full scale selection as 2g
	LSM_write(CTRL1_XL, 0b01010000);
	
	// make accelerometer data ready on INT1
	LSM_write(INT1_CTRL, 0b00000001);
}

void ISR_init(void){
	// set PC6 as input
	PORTC.DIRCLR = PIN6_bm;
	// initialize IO interrupt signals
	PORTC.INT1MASK = PIN6_bm;
	// set external interrupt PC6 as a low level interrupt
	PORTC.PIN6CTRL = PORT_ISC_FALLING_gc | PORT_OPC_TOTEM_gc;
	PORTC.INTCTRL = PORT_INT1LVL_LO_gc;
	// enable low level interrupts
	PMIC_CTRL = PMIC_LOLVLEX_bm;
	// enable global interrupt
	sei();
}

ISR(PORTC_INT1_vect){
	// disable interrupt
	PORTC.INTCTRL = 0;
	// set acceleration global flag to 1
	accel_flag = 1;
}

int main(void) {
	// initializations
	ISR_init();
	spi_init();
	LSM_init();
	usartd0_init();
	
	// wake up the IMU by reading the XYZ registers
	LSM_read(OUTX_L_XL);
	LSM_read(OUTY_L_XL);
	LSM_read(OUTZ_L_XL);
	
	while(1){
		if(accel_flag){
			// transmit data via uart
			usartd0_out_char(LSM_read(OUTX_L_XL));
			usartd0_out_char(LSM_read(OUTX_H_XL));
			usartd0_out_char(LSM_read(OUTY_L_XL));
			usartd0_out_char(LSM_read(OUTY_H_XL));
			usartd0_out_char(LSM_read(OUTZ_L_XL));
			usartd0_out_char(LSM_read(OUTZ_H_XL));
			// reset acceleration flag
			accel_flag = 0;
			// re-enable interrupt once read is complete
			PORTC.INTCTRL = PORT_INT1LVL_LO_gc;
		}
	}
	return 0;
}