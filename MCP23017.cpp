/*
MCP23017.cpp 

16 Bit GPIO Expander (I2C version)

EnergyBoard

 */

#include "MCP23017.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <sstream>
#include <bitset>
#include <iomanip>
using namespace std;

namespace energyBoard {

//From Table 1-3 for the MCP23017 Data sheet
#define IODIRA   0x00   //< Input/Output direction Port A Register
#define IODIRB   0x01   //< Input/Output direction Port B Register
#define IPOLA    0x02   //< Set the polarity of Port A (invert inputs)
#define IPOLB    0x03   //< Set the polarity of Port B (invert inputs)
#define GPINTENA 0x04   //< Interrupt on change control reg. Port A
#define GPINTENB 0x05   //< Port B -- must have DEFVAL and INTCON set
#define DEFVALA  0x06   //< Default compare register
#define DEFVALB  0x07   //< Default compare register
#define INTCONA  0x08   //< Interrupt control register to choose whether int
#define INTCONB  0x09   //< is on change (1) or on compare with DEFVAL (0)
#define IOCON    0x0A   //< IOCON is shared with IOCONA and IOCONB
#define IOCONA   0x0A   //< Configuration and control register
#define IOCONB   0x0B   //< Configuration and control register
#define GPPUA    0x0C   //< Input pull-up resistor config for Port A
#define GPPUB    0x0D   //< Input pull-up resistor config for Port B
#define INTFA    0x0E   //< Interrupt flag register (reflects int cond.)
#define INTFB    0x0F   //< Interrupt flag register Port B
#define INTCAPA  0x10   //< Captures Port A values when interrupt occurs
#define INTCAPB  0x11   //< Captures Port B values when interrupt occurs
#define GPIOA    0x12   //< GPIO input register (writing affects OLATx)
#define GPIOB    0x13   //< GPIO input register Port B
#define OLATA    0x14   //< Output latch for setting outputs on Port A
#define OLATB    0x15   //< Output latch for setting outputs on Port B

#define DEFAULT_INIT_STATE  0b00111010  



int MCP23017::writeDevice(unsigned char address, unsigned char value){

   return this->i2cDevice->writeRegister(address, value);
}

unsigned char MCP23017::readDevice(unsigned char address){

   return this->i2cDevice->readRegister(address);
}

/**
 * The constructor for the MCP23017 object. It passes the bus number and the
 * device address to the constructor of I2CDevice. All of the states
 * are initialized and the registers are updated.
 * @param I2CBus The bus number that the MCP23017 is on (default 1)
 * @param I2CAddress The address of the MCP23017 device (default 0x40)
 */

MCP23017::MCP23017(I2CDevice *i2cDevice) {
   this->i2cDevice = i2cDevice;
   this->updateConfigRegister(DEFAULT_INIT_STATE);
}

int MCP23017::updateConfigRegister(unsigned char value){
   this->configRegister = value;
   //IOCON is mirrored over IOCONA and IOCONB
   this->writeDevice(IOCONA, configRegister);
   return this->writeDevice(IOCONB, configRegister);
}

int MCP23017::setGPIODirections(MCP23017::PORT port, unsigned char value) {
   if(port==PORTA){
      return this->writeDevice(IODIRA, value);
   }
   return this->writeDevice(IODIRB, value);
}

int MCP23017::setGPIODirections(unsigned short value){
   this->setGPIODirections(PORTA, value & 0xff);
   return this->setGPIODirections(PORTB, value >> 8);
}

unsigned char MCP23017::getOutputValues(MCP23017::PORT port) {
   if(port==PORTA){
      return this->readDevice(OLATA);
   }
   return this->readDevice(OLATB);
}

unsigned short MCP23017::getOutputValues(){
   short value = getOutputValues(PORTB);
   return (value<<8) | getOutputValues(PORTA);
}

std::string MCP23017::getOutputValuesStr(){
   stringstream ss;
   ss << std::bitset<16>(getOutputValues());
   return ss.str();
}

int MCP23017::setOutputValues(MCP23017::PORT port, unsigned char value) {
   if(port==PORTA){
      return this->writeDevice(OLATA, value);
   }
   return this->writeDevice(OLATB, value);
}

int MCP23017::setOutputValues(unsigned short value){
   setOutputValues(PORTA, value & 0xff);
   return setOutputValues(PORTB, value >> 8);
}

unsigned char MCP23017::getInputValues(MCP23017::PORT port) {
   if(port==PORTA){
      return this->readDevice(GPIOA);
   }
   return this->readDevice(GPIOB);
}

unsigned short MCP23017::getInputValues(){
   short value = getInputValues(PORTB);
   return (value<<8) | getInputValues(PORTA);
}

std::string MCP23017::getInputValuesStr(){
   stringstream ss;
   ss << std::bitset<16>(getInputValues());
   return ss.str();
}

int MCP23017::setInputPolarity(MCP23017::PORT port, unsigned char value){
   if(port==PORTA){
      return this->writeDevice(IPOLA, value);
   }
   return this->writeDevice(IPOLB, value);
}

int MCP23017::setInputPolarity(unsigned short value){
   setInputPolarity(PORTA, value & 0xff);
   return setInputPolarity(PORTB, value >> 8);
}

int MCP23017::setGPIOPullUps(MCP23017::PORT port, unsigned char value){
   if(port==PORTA){
      return this->writeDevice(GPPUA, value);
   }
   return this->writeDevice(GPPUB, value);
}

int MCP23017::setGPIOPullUps(unsigned short value){
   setGPIOPullUps(PORTA, value & 0xff);
   return setGPIOPullUps(PORTB, value >> 8);
}

// Sets the bits to trigger the interrupts (A and B output pins) when changed
int MCP23017::setInterruptOnChange(MCP23017::PORT port, unsigned char value){
   if(port==PORTA){
      return this->writeDevice(GPINTENA, value);
   }
   return this->writeDevice(GPINTENB, value);
}

int MCP23017::setInterruptOnChange(unsigned short value){
   setInterruptOnChange(PORTA, value & 0xff);
   return setInterruptOnChange(PORTB, value >> 8);
}

// Get the interrupt capture register values -- reflects the value on the port
// when the interrupt occurs. Cleared after it or GPIO state is read.
unsigned char MCP23017::getInterruptCaptureState(MCP23017::PORT port){
   if(port==PORTA){
      return this->readDevice(INTCAPA);
   }
   return this->readDevice(INTCAPB);
}

unsigned short MCP23017::getInterruptCaptureState(){
   short value = getInterruptCaptureState(PORTB);
   return (value<<8) | getInterruptCaptureState(PORTA);
}

std::string MCP23017::getInterruptCaptureStateStr(){
   stringstream ss;
   ss << std::bitset<16>(getInterruptCaptureState());
   return ss.str();
}

int MCP23017::setInterruptControl(MCP23017::PORT port, unsigned char value){
   if(port==PORTA){
      return this->writeDevice(INTCONA, value);
   }
   return this->writeDevice(INTCONB, value);
}
int MCP23017::setInterruptControl(unsigned short value){
   setInterruptControl(PORTA, value & 0xff);
   setInterruptControl(PORTB, value >> 8);
}

int MCP23017::setDefaultCompareValue(MCP23017::PORT port, unsigned char value){
   if(port==PORTA){
      return this->writeDevice(DEFVALA, value);
   }
   return this->writeDevice(DEFVALB, value);
}
int MCP23017::setDefaultCompareValue(unsigned short value){
   setDefaultCompareValue(PORTA, value & 0xff);
   return setDefaultCompareValue(PORTB, value >> 8);
}

unsigned char MCP23017::getInterruptFlagState(MCP23017::PORT port){
   if(port==PORTA){
      return this->readDevice(INTFA);
   }
   return this->readDevice(INTFB);
}

unsigned short MCP23017::getInterruptFlagState(){
   short value = getInterruptFlagState(PORTB);
   return (value<<8) | getInterruptFlagState(PORTA);
}

std::string MCP23017::getInterruptFlagStateStr(){
   stringstream ss;
   ss << std::bitset<16>(getInterruptFlagState());
   return ss.str();
}

void MCP23017::dumpRegisters(){
  const string registers[] = {"IODIRA  ","IPOLA   ","GPINTENA","DEFVALA ","INTCONA ",
      "IOCONA  ", "GPPUA   ","INTFA   ","INTAPA  ","GPIOA   ","OLATA   "};
  cout << "Register Dump:" << endl;
  for(int i=0; i<11; i++){
     char a = this->readDevice(i*2);
     char b = this->readDevice((i*2)+1);
     cout << "Register " << registers[i] << ": " << setw(5) << (int)a << "  B: " << (int)b << endl;
  }
  
void MCP23017::flashLED(){
	// Flash D1 LED 5 times 
	
	for(int i=0; i<6; i++){
		this->writeDevice();
		
		
	}
	
}  
  
}

} /* namespace energyBoard */
