/*
 * AMRC.c
 *
 * Created: 06/07/2024 20:13:50
 * Author : Kiru
 */ 

#define F_CPU 16000000UL  // Define CPU clock speed
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>

#define MOTOR1_PWM PD6 // OC0A (Output Compare pin for Timer0, Channel A)
#define MOTOR1_DIR PD2
#define MOTOR2_PWM PD5 // OC0B (Output Compare pin for Timer0, Channel B)
#define MOTOR2_DIR PD3

int pwm1 = 0, pwm2 = 0;

void uart_init(unsigned int ubrr) {
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	// Set frame format: 8 data bits, 1 stop bit
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

unsigned char uart_receive(void) {
	// Wait for data to be received
	while (!(UCSR0A & (1<<RXC0)));
	// Get and return received data from buffer
	return UDR0;
}

void pwm_init() {
	// Set PWM pins as output
	DDRD |= (1<<MOTOR1_PWM) | (1<<MOTOR2_PWM);
	DDRD |= (1<<MOTOR1_DIR) | (1<<MOTOR2_DIR);

	// Set Fast PWM mode, non-inverted
	TCCR0A = (1<<WGM00) | (1<<WGM01) | (1<<COM0A1) | (1<<COM0B1);
	TIMSK0 = (1<<TOIE0);
	
	// Set prescaler to 64 and start PWM
	TCCR0B = (1<<CS01) | (1<<CS00);
}

void set_motor_speed(int motor, int speed) {
	if (motor == 1) {
		if (speed >= 0) {
			PORTD &= ~(1<<MOTOR1_DIR); // Set direction to forward
			OCR0A = speed; // Set PWM duty cycle
			} else {
			PORTD |= (1<<MOTOR1_DIR); // Set direction to backward
			OCR0A = -speed; // Set PWM duty cycle
		}
		} else if (motor == 2) {
		if (speed >= 0) {
			PORTD &= ~(1<<MOTOR2_DIR); // Set direction to forward
			OCR0B = speed; // Set PWM duty cycle
			} else {
			PORTD |= (1<<MOTOR2_DIR); // Set direction to backward
			OCR0B = -speed; // Set PWM duty cycle
		}
	}
}

/*int main(void) {
	char buffer[20];
	int index = 0;
	char c;

	// Initialize UART and PWM
	uart_init(103); // For 9600 baud with 16MHz clock
	pwm_init();

	// Enable global interrupts
	sei();

	while (1) {
		// Read PWM values from the serial input
		c = uart_receive();
		if (c == '\n' || c == '\r') {
			buffer[index] = '\0';

			// Parse the received PWM values
			char* ptr = strtok(buffer, " ");
			if (ptr != NULL) pwm1 = atoi(ptr);
			ptr = strtok(NULL, " ");
			if (ptr != NULL) pwm2 = atoi(ptr);

			// Set motor speeds
			set_motor_speed(1, pwm1);
			set_motor_speed(2, pwm2);

			// Reset the index for the next read
			index = 0;
			} else {
			if (index < sizeof(buffer) - 1) {
				buffer[index++] = c;
			}
		}
	}
}*/
int main(void){
	pwm_init();
	sei();
	while(1){
		set_motor_speed(1, 100);
		set_motor_speed(2, 100);
	}
}

