/*
 * txSerial.c
 */
 
#include <avr/io.h>
#include <avr/interrupt.h>
 
#define F_CPU   16000000
#define BUAD    9600
#define BRC     ((F_CPU/16/BUAD) - 1)
#define TX_BUFFER_SIZE  128
 
#include <util/delay.h>
 
char serialBuffer[TX_BUFFER_SIZE];// creating a character array
uint8_t serialReadPos = 0;//Starting read position
uint8_t serialWritePos = 0;//Writr position
 
void appendSerial(char c);//just to concatenate the string
void serialWrite(char  c[]);//Writing on terminal
 
int main(void)
{   
    UBRR0H = (BRC >> 8);
    UBRR0L =  BRC;
     
    UCSR0B = (1 << TXEN0)  | (1 << TXCIE0);//Tx enable on Bit3
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);//From the table 19-7 I needed 8 bit character size so setting UCSzn1 and UCSZ0 to be 1.
     
    sei();                                 //Enabling the interrupt
     
    serialWrite("HELLO\n\r");              //Sending Character
     
    _delay_ms(1500);
     
    serialWrite("WORLD\n\r");
     
    while(1)
    {
    }
}
 
void appendSerial(char c)
{
    serialBuffer[serialWritePos] = c;//Write the char C in the right position
    serialWritePos++;
     
    if(serialWritePos >= TX_BUFFER_SIZE)//If we are at the end of the Array
    {
        serialWritePos = 0;//Write from beginning
    }
}
 
void serialWrite(char c[])//Take entire character like a string-send message
{
    for(uint8_t i = 0; i < strlen(c); i++)//loop for whole array
    {
        appendSerial(c[i]);//concatenating the char
    }
     
    if(UCSR0A & (1 << UDRE0))
    {
        UDR0 = 0;
    }
}
 
ISR(USART_TX_vect)//Tells that Transmission is complete
{
    if(serialReadPos != serialWritePos)// If read posittion and write pos are not equal
    {
        UDR0 = serialBuffer[serialReadPos];//Shit out Data
        serialReadPos++;//increment
         
        if(serialReadPos >= TX_BUFFER_SIZE)//if array is ended
        {
            serialReadPos++;//increment
        }
    }
}