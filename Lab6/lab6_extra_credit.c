/*
* File Name: lab6_extra_credit.c
* Name: Enoch Wong
* PI Name: Matthew Stormant
* Class Number: 11645
* Description: Plots gyroscope data
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi.h"
#include "lsm6dsl.h"
#include "lsm6dsl_registers.h"
#include "usart.h"

volatile uint8_t gyro_flag = 0;

void LSM_init(void){
	// IMU software reset
	LSM_write(CTRL3_C, 0b00000001);
	
	// set IMU interrupt activation level as active low and enable address incrementation during multiple byte access
	LSM_write(CTRL3_C, 0b00100100);
	
	// set output data rate as 208Hz, full scale selection as default
	LSM_write(CTRL2_G, 0b01010000);
	
	// enable data in XYZ axis
	LSM_write(CTRL9_XL, 0b11100000);
	
	// make gyroscope data ready on INT1
	LSM_write(INT1_CTRL, 0b00000010);
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
	gyro_flag = 1;
}

int main(void) {
	// initializations
	ISR_init();
	spi_init();
	LSM_init();
	usartd0_init();
	
	// wake up the IMU by reading the XYZ registers
	LSM_read(OUTX_L_G);
	LSM_read(OUTY_L_G);
	LSM_read(OUTZ_L_G);
	
	while(1){
		if(gyro_flag){
			// transmit data via uart
			usartd0_out_char(LSM_read(OUTX_L_G));
			usartd0_out_char(LSM_read(OUTX_H_G));
			usartd0_out_char(LSM_read(OUTY_L_G));
			usartd0_out_char(LSM_read(OUTY_H_G));
			usartd0_out_char(LSM_read(OUTZ_L_G));
			usartd0_out_char(LSM_read(OUTZ_H_G));
			// reset acceleration flag
			gyro_flag = 0;
			// re-enable interrupt once read is complete
			PORTC.INTCTRL = PORT_INT1LVL_LO_gc;
		}
	}
	return 0;
}