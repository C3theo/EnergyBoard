/*
 MCP4728.h
 
 Quad Channel 12-Bit DAC w/ EEPROM
 
 
 EEPROM
 DAC 0 DAC1
 Channels A, B, C, D  pins (6,7,8,9)
 
 On Energy Board
 VOUTA -> DAC0
 VOUTB ->DAC1
 
 
 VREF = VDD (3.3V)
 
 LDAC - updates analog outputs 
 UDAC

 Configuration - 7
 DAC Input - 12 bits

 http://ww1.microchip.com/downloads/en/DeviceDoc/22187E.pdf
 
 */

#ifndef MCP4728_H_
#define MCP4728_H_
#include"I2CDevice.h"

namespace energyBoard {

/**
 * @class MCP4728
 * @brief Specific class for the MCP4725 DAC driver that is a child of the I2CDevice class
 */
class MCP4728:protected I2CDevice{
public:
   enum PD_MODE { DISABLE, GND_1K, GND_100K, GND_500K }; // Power Down Mode

private:
   unsigned int I2CBus, I2CAddress;
   unsigned int lastValue;
   int setOutput(unsigned int value, MCP4728::PD_MODE mode);

public:
   MCP4728(unsigned int I2CBus=1, unsigned int I2CAddress=0x60); //Default Address: 0x60
   virtual int powerDown(MCP4728::PD_MODE mode = GND_500K);
   virtual int wake();
   virtual int setOutput(unsigned int value);
   virtual int setOutput(float percentage);
   virtual int setOutput(unsigned int waveform[], int size, int loops=1);
   virtual unsigned int getLastValue() { return lastValue; }
   virtual ~MCP4728();
};

} /* namespace energyBoard */

#endif /* MCP4728_H_ */
