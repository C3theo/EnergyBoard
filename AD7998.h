/*

AD7998.h
 
(8-Channel 12-Bit ADC) 

16 Data Registers
1 Address Pointer Register

Address Pointer Register
Address pointer doesn't have address

Configuration Register
2-byte write

CH8-1 select analog inputs to be converted


Digital Output = 4096*VIN/VREF

EnergyBoard:

I2CAddress = 0x24

VREF = 2.5V
AS = 3.3V (Address Select = 010 0010)

INPUT:
GPB7 -> BUSY
GPB6 -> CONVST 

OUTPUT:
(J5 header)
0x01 VIN1 -> ADC3 
0x03 VIN3 -> ADC2
0x05 VIN5 -> ADC1
0x07 VIN7 -> ADC0



Conversion Example:
Convert VIN1

write 0x




Datasheet: http://www.analog.com/media/en/technical-documentation/data-sheets/AD7997_7998.pdf

*/

#ifndef AD7998_H_
#define AD7998_H_
#include"I2CDevice.h"


namespace energyBoard {

/**
 * @class AD7998
 * @brief Specific class for the AD7998 ADC that is a child of the I2CDevice class
 */
class AD7998:protected I2CDevice{

private:
   unsigned int I2CBus, I2CAddress;


public:
   AD7998(unsigned int I2CBus=1, unsigned int I2CAddress=0x24); // Bus 2 address 0x24
   virtual 

   virtual ~AD7998();
};

} /* namespace energyBoard */

#endif /* AD7998_H_ */
