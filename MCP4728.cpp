/*
	MCP4728.cpp 


  12-bit DAC 

 
 */

#include "MCP4728.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

using namespace std;

namespace energyBoard {

/**
 * The constructor for the MCP4728 object. It passes the bus number and the device address
 * (which is 0x60 by default) to the constructor of I2CDevice. All of the states
 * are initialized and the registers are updated.
 * @param I2CBus The bus number that the MCP4728 device is on - defaults to 1 (H/W I2C-2)
 * @param I2CAddress The address of the MCP4728 device (default 0x60)
 */
MCP4728::MCP4728(unsigned int I2CBus, unsigned int I2CAddress):
   I2CDevice(I2CBus, I2CAddress){   // this member initialisation list calls the parent constructor
   this->I2CAddress = I2CAddress;
   this->I2CBus = I2CBus;
}

/**
 * Use the Power Down Modes (11,10,01) to set the equivalent GND resistor values. See the
 * header file for the values to use. This function calls the private setOutput() function.
 * @return 0 if successful
 */
int MCP4728::powerDown(MCP4728::PD_MODE mode){
   return setOutput(lastValue, mode);
}

/**
 * Wakes the DAC by disabling the power down value
 * @return 0 if successful
 */
int MCP4728::wake(){
   return setOutput(lastValue, DISABLE);
}

/**
 * Set the output as a percentage of Vcc (typically 3.3V)
 * @param percentage the percentage value (0.0 to 100.0)
 * @return 0 if successful
 */
int MCP4728::setOutput(float percentage){
   if((percentage < 0.0f)||(percentage > 100.0f)){
      perror("PWMDriver: Invalid percentage value used");
      return -1;
   }
   return setOutput((unsigned int)roundf(40.95f*percentage));
}

/**
 * Set the output as a 12-bit value
 * @param output the 12-bit unsigned integer value (0-4095)
 * @return 0 if successful
 */
int MCP4728::setOutput(unsigned int output){
// cout << "Setting the DAC to have decimal value: " << output << endl;
   if((output < 0)||(output > 4095)){
      perror("MCP4728: The output is outside range");
      return -1;
   }
   return setOutput(output, DISABLE);
}

/**
 * This method does all the work, but is private in order to abstract it through the
 * methods that are defined above. Keeps the other code straightforward.
 * @param output the 12-bit unsigned integer, already validated
 * @param mode the Power Down mode -- DISABLE for regular operation
 * @return 0 if successful
 */
int MCP4728::setOutput(unsigned int output, MCP4728::PD_MODE mode){
   this->open();
   unsigned char msb = output >> 8;
   unsigned char lsb = output & 0xff;
   if(mode!=DISABLE){
      msb = msb | (mode<<4);     //In fast mode write, power down bits PD1, PD0 are bits 5,4
   }
   int result = this->writeRegister(msb, lsb);
   if (result!=0){
      perror("MCP4728: Failed to write to the device");
      return -1;
   }
// cout << "Wrote decimal equiv. values: " << (int)msb << "," << (int)lsb << endl;
   this->close();
   this->lastValue = output;
   return 0;
}

int MCP4728::setOutput(unsigned int waveform[], int size, int loops){
   //fast write -- no validation or error checking
   this->open();
   for(int loop=0; loop<loops; loop++){
      for(int i=0; i<size; i++){
         this->writeRegister(waveform[i] >> 8, waveform[i] & 0xff);
      }
   }
   this->close();
   return 0;
}

MCP4728::~MCP4728() {}

} /* namespace energyBoard */
