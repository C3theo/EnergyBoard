/*
AD7998.cpp
 
12-Bit ADC
*/

#include "AD7998.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
using namespace std;

namespace energyBoard{
	
	


// From Table 8 of AD7998 Datasheet
//  Register Addresses

#define CONVERSION 	0x00 //Conversion Result  (R)
#define ALERT 		0x01 //Alert Status  (R/W)
#define CONFIG   	0x02 //Configuration  (R/W)
#define CYCLE    	0x03 //Cycle Timer  (R/W)
#define CH1_LOW  	0x04 //Activates ALERT if value less than limit CH1 (R/W)
#define CH1_HIGH 	0x05 //Activates ALERT if value greater than limit  CH1 (R/W)
#define CH1_HYST 	0x06 //Determines reset point for ALERT  CH1 (R/W)
#define CH2_LOW  	0x07 //Activates ALERT if value less than limit  CH2 (R/W)
#define CH2_HIGH 	0x08 //Activates ALERT if value greater than limit  CH2 (R/W)
#define CH2_HYST 	0x09 //Determines reset point for ALERT  CH2 (R/W)
#define CH3_LOW  	0x0A //Activates ALERT if value less than limit  CH3 (R/W)
#define CH3_HIGH 	0x0B //Activates ALERT if value greater than limit  CH3 (R/W)
#define CH3_HYST 	0x0C //Determines reset point for ALERT  CH3 (R/W)
#define CH4_LOW 	0x0D //Activates ALERT if value less than limit  CH4 (R/W)
#define CH4_HIGH 	0x0E //Activates ALERT if value greater than limit  CH4 (R/W)
#define CH4_HYST 	0x0F //Determines reset point for ALERT  CH4 (R/W)


AD7998::AD7998(unsigned int I2CBus, unsigned int I2CAddress):I2CDevice(I2CBus, I2CAddress){
	this->I2CAddress = I2CAddress;
	this->I2CBus = I2CBus;
}
	
short AD7998::combineValues(unsigned char upper, unsigned char lower){
	return ((short)upper<<8)|(short)lower;
}
	
void AD7998::convertValues(){
	
}

void AD7998::selectChannel(){
	
}
	
	
}


  
}