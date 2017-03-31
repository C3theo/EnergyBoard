/* 
	gpio_i2ctest.cpp
 Flash D2 repeatedly from user input
*/

#include <iostream>
#include <sstream>
#include "gpioExpander.h"

using namespace std;


int main(){
	
   unsigned char value;
   I2CDevice *i2cDevice = new I2CDevice(1,0x21);

   i2cDevice->open();  // Open for the following comands -- close afterwards
   i2cDevice->writeRegister(0x05, 0b10111010); //set up the MCP23017
   
   cout << "Set up the Control Register - IOCON (0x05)" << endl;
   
   // LED Control 

   cin >> count >> endl;
   
   
   
   i2cDevice->writeRegister(0x01, 0b00000000); //set all of PORTA as outputs
   cout << "Set up PORTB (GPIOB_OO) as outputs using - IODIRB (0x00)" << endl;

 

   value = i2cDevice->readRegister(0x05); // check that IOCON was set 
   cout << "At IOCON Received [" << (int)value << "]" << endl;

   value = i2cDevice->readRegister(0x09); // check that IOCON was set above
   cout << "At GPIOA Received [" << (int)value << "]" << endl;

   value = i2cDevice->readRegister(0x13); // check that IOCON was set above
   cout << "At OLATB Received [" << (int)value << "]" << endl;

   i2cDevice->close();
}
