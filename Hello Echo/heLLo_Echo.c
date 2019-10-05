//
//
// hello.ftdi.44.echo.c
//
// 115200 baud FTDI character echo, with flash string
//
// set lfuse to 0x5E for 20 MHz xtal
//
// Neil Gershenfeld
// 12/8/10
//
// (c) Massachusetts Institute of Technology 2010
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//
/* There are additonal comments added to the code for more readability aand understanding by Ahmad Ali.
 I's written for Attiny 44 if you wish to change processor the make sure to change the associated make file
 Which converts this Written C code to hex files.
 First you have to run the make file then Setup the fuses then Load the program. 
*/
/* 
*/

#include <avr/io.h>                                //including header io files fro AVR Library
#include <util/delay.h>                            //
#include <avr/pgmspace.h>                          //To use proram space utility library of avrLibc including this header file

//macros or shorthand to save typos
#define output(directions,pin) (directions |= pin) // set port direction for output 
                                                   /*can also be written as (directions = direction | pin) bit ise OR operation with Dir and Pin to set both of them as output*/
#define set(port,pin) (port |= pin)                // set port pin /*Setting up the port as Pins*/
#define clear(port,pin) (port &= (~pin))           // clear port pin 
                                                   /*(port = port & (~pin)) And operation with complementary port to clear port pin    */
#define pin_test(pins,pin) (pins & pin)            // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit))     // test for bit set by using Left Shift (<<)- 
                                                   /*Left shift operator is a binary operator which shift the some number of bits, in the given bit pattern, to the left and append 0 at the end. */
#define bit_delay_time 8.5                         // bit delay for 115200 with overhead
                                                   /*Baud rate, as mentioned above, is just the measure of how many bits can be sent/received in 1 second. 
                                                   This determines the period of each bit (i.e. the time interval between each bit being sent/received) and the period is equal to 1/Baud.
                                                   1/115200 = 8.6 uSec approx so delay time of 8.5 uSec*/
#define bit_delay() _delay_us(bit_delay_time)       // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define char_delay() _delay_ms(10)                  // char delay

#define serial_port PORTA                       `  // Defining Serial PortA
#define serial_direction DDRA                    ` //Defining Data Direction Registers of Port A 
#define serial_pins PINA                          `//Defining PINA of Port A
#define serial_pin_in (1 << PA0)                   //1 << PA0 will shift 1 by PA0 to the left. Since PA0 is 0, there is no shift its input Serial PIN
#define serial_pin_out (1 << PA1)                  //1 << PA1 will shift 1 by PA1 to the left. Since PA1 is 0, there is no shift its Output Serial PIN

#define max_buffer 25                              //Aray size of 25 characters 

void get_char(volatile unsigned char *pins, unsigned char pin, char *rxbyte) //deining function of get_chard with some arguments
// star  rx byte ki value globally
/*arguments are address of pins, char pin, char rxbyte address 
The purpose of using pointer is that the value should change globally of the pin and rxbyte/receiving bits*/
 {
   //
   // read character into rxbyte on pins pin
   //    assumes line driver (inverts bits)
   //
   *rxbyte = 0;
   while (pin_test(*pins,pin))// loop for pin test on recieving | Library routine that gets charcter in
      //
      // wait for start bit
      //
      ;
   //
   // delay to middle of first data bit
   //
   half_bit_delay();
   bit_delay();
   //
   // unrolled loop to read data bits
   //
   if pin_test(*pins,pin)  //checking the address 
      *rxbyte |= (1 << 0); //Equates to shifting 1 left by zero bits, leaving an OR with the value 1 to set the first bit.
   else
      *rxbyte |= (0 << 0);//Equates to shifting 0 left by zero bits, leaving an OR with the value 0 i.e making it 0
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 1);
   else
      *rxbyte |= (0 << 1);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 2);
   else
      *rxbyte |= (0 << 2);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 3);
   else
      *rxbyte |= (0 << 3);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 4);
   else
      *rxbyte |= (0 << 4);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 5);
   else
      *rxbyte |= (0 << 5);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 6);
   else
      *rxbyte |= (0 << 6);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 7);
   else
      *rxbyte |= (0 << 7);
   //
   // wait for stop bit
   //
   bit_delay();
   half_bit_delay();
   }

void put_char(volatile unsigned char *port, unsigned char pin, char txchar)
   // Library routine that gets charcter OUT on port pin notice the If used is fro 0 to 7 i.e 8 bits/ 1 byte
 {
   //
   // send character in txchar on port pin
   //    assumes line driver (inverts bits)
   //
   // start bit
   //
   clear(*port,pin);
   bit_delay();
   //
   // unrolled loop to write data bits
   //
   if bit_test(txchar,0)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,1)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,2)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,3)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,4)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,5)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,6)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,7)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   //
   // stop bit
   //
   set(*port,pin);
   bit_delay();
   //
   // char delay
   //
   bit_delay();
   }

void put_string(volatile unsigned char *port, unsigned char pin, char *str) {
   //
   // print a null-terminated string
   //
   static int index;
   index = 0;
   do {
      put_char(port, pin, str[index]);
      ++index;
      } while (str[index] != 0);
   }

int main(void) 
{ 
   //
   // main function that runs 
   //
   static char chr;
   static char buffer[max_buffer] = {0}; /*Initialising a charcter, a buffer array to store the character and */
   static int index;                     /*An int type index variable to store the index number of buffer array/
   //
   // set clock divider to /1  --This is where we are telling how fast it is
   //So one processesor cycle for each clock
   //For understanding Chek Page 32 of Datasheet- 
      /*
      To avoid unintentional changes of clock frequency, a special write procedure must be followed
      to change the CLKPS bits:
      1. Write the Clock Prescaler Change Enable (CLKPCE) bit to one and all other bits in
      CLKPR to zero.
      2. Within four cycles, write the desired value to CLKPS while writing a zero to CLKPCE. 
      */
   CLKPR = (1 << CLKPCE);                                                               // setting to 1
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);              // setting all other to 0  -From Datasheet
   //
   // initialize output pins
   //
   set(serial_port, serial_pin_out);                                                  //Setting the port and pin
   output(serial_direction, serial_pin_out);                                          //Setting as output
   //
   // main loop
   //
   index = 0; //Initializing the index number
   while (1) 
   //While loop that runs infinite 
   {
      get_char(&serial_pins, serial_pin_in, &chr);                                    //Wait for the character to come in
      put_string(&serial_port, serial_pin_out, "hello.ftdi.44.echo.c: you typed \"");//Puts out a message that says that user has typed some string
      buffer[index++] = chr;                                                         //Saves the character in the array by incrementing index num
      if (index == (max_buffer-1))                                                  //checking for filling up the array and 
         index = 0;                                                                 //Settting index to 0 to start sending out character from beginning
      put_string(&serial_port, serial_pin_out, buffer);                             //Puts out the character from the buffer
      put_char(&serial_port, serial_pin_out, '\"');                                 //It starts a new line
      put_char(&serial_port, serial_pin_out, 10);    
      }
   }