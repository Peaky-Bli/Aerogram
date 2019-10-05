#include <avr/io.h>					 //including AVR input output header file
  #include <util/delay.h>	//including Header file to specify actual time values rather than cycles per second for Delay
 #include <avr/interrupt.h>				//including Header file for interrupts
#include <avr/pgmspace.h>	//including Header file to access data stored in program space
#define output(directions,pin) (directions |= pin)             // setting the Direction of the Port Pin as Output 
#define set(port,pin) (port |= pin) 			// To set the pin of the Port
#define clear(port,pin) (port &= (~pin)) 			// to clear the pin of the Port
#define pin_test(pins,pin) (pins & pin) 			// test for port pin 
#define bit_test(byte,bit) (byte & (1 << bit))		 // to test whether the bit is set or not
#define bit_delay_time 8.5				 // to get baudrate of approx. 115200
//1MHz/115200=8.5
#define bit_delay() _delay_us(bit_delay_time) 		// RS232 bit delay 	// delay of 8.5us
#define half_bit_delay() _delay_us(bit_delay_time/2)	 // RS232 half bit delay//Review This Line
#define char_delay() _delay_ms(10)		 //char delay//can we remove this bcoz no where used
#define serial_port PORTA		 // defining PORTA as serial_port macro
#define serial_direction DDRA 		//defining DDRA as serial_direction macro
#define serial_pins PINA 		//defining PORT A pins as serial_pins macro
#define serial_pin_in (1 << PA0) 	// defining  value of PA0 as serial_pin_in macro
#define serial_pin_out (1 << PA1)	//defining  value of PA0 as serial_pin_in macro
#define serial_interrupt (1 << PCIE0) 	//defining Interrupt register as serial_interrupt macro
#define serial_interrupt_pin (1 << PCINT0) //defining Interrupt register pin as serial_interrupt_pin macro
#define max_buffer 25 			// defining value 25 as max_buffer macro
void get_char(volatile unsigned char *pins, unsigned char pin, char *rxbyte) 
{ 
// // reads character from rxbyte on pins pin // assumes line driver (inverts bits) //
 *rxbyte = 0; 
while (pin_test(*pins,pin)) ;
// // wait for start bit // // // delay to middle of first data bit // half_bit_delay(); 
bit_delay(); 
// // unrolled loop to read data bits // 
//if data is present on pin of pins then left shift the *rxbyte data by pin no. position
//else if data is not present on pin of pins,then left shift '0' in *rxbyte by pin no. position
if pin_test(*pins,pin)					//Review this and explain it to me
     *rxbyte |= (1 << 0);
else 
     *rxbyte |= (0 << 0); 
bit_delay(); 
if pin_test(*pins,pin) 
     *rxbyte |= (1 << 1); 
else 
     *rxbyte |= (0 << 1); 
bit_delay(); 					//delay of 8.5 microsecond
if pin_test(*pins,pin) 
     *rxbyte |= (1 << 2); 
else 
     *rxbyte |= (0 << 2); 
bit_delay(); 					//delay of 8.5 microsecond
if pin_test(*pins,pin) 
     *rxbyte |= (1 << 3); 
else 
     *rxbyte |= (0 << 3); 
bit_delay(); 					//delay of 8.5 microsecond
if pin_test(*pins,pin) 
     *rxbyte |= (1 << 4); 
else 
     *rxbyte |= (0 << 4); 
bit_delay(); 					//delay of 8.5 microsecond
if pin_test(*pins,pin) 
    *rxbyte |= (1 << 5); 
else 
    *rxbyte |= (0 << 5); 
bit_delay(); 					//delay of 8.5 microsecond
if pin_test(*pins,pin) 
    *rxbyte |= (1 << 6); 
else 
    *rxbyte |= (0 << 6); 
bit_delay(); 					//delay of 8.5 microsecond
if pin_test(*pins,pin) 
    *rxbyte |= (1 << 7); 
else 
    *rxbyte |= (0 << 7); 
// // wait for stop bit // 
bit_delay(); 					//delay of 8.5 microsecond
half_bit_delay(); 	                                      //delay of 8.5/2 microsecond // Please check ones
 } 
void put_char(volatile unsigned char *port, unsigned char pin, char  *txchar)
 { // // send character in txchar on port pin // assumes line driver (inverts bits) // 
// start bit // 
clear(*port,pin); 
bit_delay(); // // unrolled loop to write data bits //
//if AND operation of BIT and DATA TO BE TRANSMITTED is TRUE, then SET the pin of PORT A else CLEAR that pin 
if bit_test(txchar,0) 			
     set(*port,pin); 
else 
     clear(*port,pin); 
bit_delay();					//delay of 8.5 microsecond
if bit_test(txchar,1) 
     set(*port,pin); 
else
     clear(*port,pin); 
bit_delay();					//delay of 8.5 microsecond
if bit_test(txchar,2) 
     set(*port,pin); 
else
     clear(*port,pin);
bit_delay();					//delay of 8.5 microsecond
if bit_test(txchar,3)
     set(*port,pin); 
else
     clear(*port,pin); 
bit_delay();					//delay of 8.5 microsecond
if bit_test(txchar,4) 
     set(*port,pin); 
else 
     clear(*port,pin); 
bit_delay();					//delay of 8.5 microsecond
if bit_test(txchar,5) 
     set(*port,pin); 
else 
     clear(*port,pin); 
bit_delay();					//delay of 8.5 microsecond
if bit_test(txchar,6) 
     set(*port,pin); 
else 
     clear(*port,pin); 
bit_delay();					//delay of 8.5 microsecond
if bit_test(txchar,7) 
     set(*port,pin); 
else 
     clear(*port,pin); 
bit_delay();					//delay of 8.5 microsecond 
// // stop bit //
set(*port,pin); 					//calling set function
bit_delay();					//delay of 8.5 microsecond
 }
void put_string(volatile unsigned char *port, unsigned char pin, char *str)
{
 // // print a null-terminated string // 
static int index; 
index = 0; 
do
{ 
put_char(port, pin, str[index]); 			//calling the put_char function and storing the 
++index;
}
while (str[index] != 0); 
} 
ISR(PCINT0_vect)				//defining the function of INTERRUPT
{
 // // pin change interrupt handler // 
static char chr; 
static char buffer[max_buffer] = {0};
static int index;
get_char(&serial_pins, serial_pin_in, &chr); 	//calling get_char function
put_string(&serial_port, serial_pin_out, "hello.ftdi.44.echo.interrupt.c: you typed \"");		//calling put_string function
buffer[index++] = chr; 
if (index == (max_buffer-1)) 	//if index reaches 24 then set value of index=0
     index = 0; 
put_string(&serial_port, serial_pin_out, buffer);		//calling put_string function
put_char(&serial_port, serial_pin_out, '\"');		//calling put_char function
put_char(&serial_port, serial_pin_out, 10); // new line 
} 
int main(void) 
{
 // // main // // 
set clock divider to /1  
CLKPR = (1 << CLKPCE); 
CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
// // initialize output pins //
set(serial_port, serial_pin_out);				//setting the PA1 in PORT A
output(serial_direction, serial_pin_out); 		//setting the direction of PA1
// set up pin change interrupt on input pin // 
set(GIMSK, serial_interrupt); 		//setting the PCIE0 in Register GIMSK
set(PCMSK0, serial_interrupt_pin);			//setting the PCINT0 in Register PCMSK0
sei(); 							//calling serial enable interrupt function
while(1); 						//infinite loop
}
