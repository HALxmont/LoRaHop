#ifndef LORAE32_H_
#define LORAE32_H_

#include<stm32f1xx.h>

#define PIN_RECOVER 15 

// modes NORMAL send and recieve for example
#define MODE_NORMAL 0			// can send and recieve
#define MODE_WAKEUP 1			// sends a preamble to waken receiver
#define MODE_POWERDOWN 2		// can't transmit but receive works only in wake up mode
#define MODE_PROGRAM 3			// for programming

// options to save change permanently or temp (power down and restart will restore settings to last saved options
#define PERMANENT 0xC0
#define TEMPORARY 0xC2

// parity bit options (must be the same for transmitter and reveiver)
#define PB_8N1 0b00			// default
#define PB_8O1 0b01
#define PB_8E1 0b11

//UART data rates
// (can be different for transmitter and reveiver)
#define UDR_1200 0b000		// 1200 baud
#define UDR_2400 0b001		// 2400 baud
#define UDR_4800 0b010		// 4800 baud
#define UDR_9600 0b011		// 9600 baud default
#define UDR_19200 0b100		// 19200 baud
#define UDR_38400 0b101		// 34800 baud
#define UDR_57600 0b110		// 57600 baud
#define UDR_115200 0b111	// 115200 baud

// air data rates (certian types of modules)
// (must be the same for transmitter and reveiver)
#define ADR_300 0b000		// 300 baud
#define ADR_1200 0b001		// 1200 baud
#define ADR_2400 0b010		// 2400 baud
#define ADR_4800 0b011		// 4800 baud
#define ADR_9600 0b100		// 9600 baud
#define ADR_19200 0b101		// 19200 baud

// air data rates (other types of modules)
#define ADR_1K 0b000		// 1k baud
#define ADR_2K 0b001		// 2K baud
#define ADR_5K 0b010		// 4K baud
#define ADR_8K 0b011		// 8K baud
#define ADR_10K 0b100		// 10K baud
#define ADR_15K 0b101		// 15K baud
#define ADR_20K 0b110		// 20K baud
#define ADR_25K 0b111		// 25K baud

// various options
// (can be different for transmitter and reveiver)
#define OPT_FMDISABLE 0b0	//default
#define OPT_FMENABLE 0b1
#define OPT_IOOPENDRAIN 0b0	 
#define OPT_IOPUSHPULL  0b1
#define OPT_WAKEUP250  0b000 
#define OPT_WAKEUP500  0b001
#define OPT_WAKEUP750  0b010
#define OPT_WAKEUP1000 0b011
#define OPT_WAKEUP1250 0b100
#define OPT_WAKEUP1500 0b101
#define OPT_WAKEUP1750 0b110
#define OPT_WAKEUP2000 0b111
#define OPT_FECDISABLE  0b0
#define OPT_FECENABLE 0b1	

// constants or 100 mW units
#define OPT_TP20 0b00		// 20 db
#define OPT_TP17 0b01		// 17 db
#define OPT_TP14 0b10		// 14 db
#define OPT_TP11 0b11		// 10 db
#define OPT_TP10 0b11		// 10 db


typedef  struct{

	// pin variables
	int8_t _M0;
	int8_t _M1;
	int8_t _AUX;

	// variable for the 6 bytes that are sent to the module to program it
	// or bytes received to indicate modules programmed settings
	uint8_t _Params[6];

	// indicidual variables for each of the 6 bytes
	// _Params could be used as the main variable storage, but since some bytes
	// are a collection of several options, let's just make storage consistent
	// also Param[1] is different data depending on the _Save variable
	uint8_t _Save;
	uint8_t _AddressHigh;
	uint8_t _AddressLow;
	uint8_t _Speed;
	uint8_t _Channel;
	uint8_t _Options;
	
	// individual variables for all the options
	uint8_t _ParityBit;
	uint8_t _UARTDataRate;
	uint8_t _AirDataRate;
	uint8_t _OptionTrans;
	uint8_t _OptionPullup;
	uint8_t _OptionWakeup;
	uint8_t _OptionFEC;
	uint8_t _OptionPower;
	uint16_t _Address;
	uint8_t _Model;
	uint8_t _Version;
	uint8_t _Features;
	uint8_t _buf;

} LoraTypeDef;




uint8_t init();
void SetMode(uint8_t mode = MODE_NORMAL);
void SetAddress(uint16_t val = 0);
void SetAddressH(uint8_t val = 0);
void SetAddressL(uint8_t val = 0);
void SetAirDataRate(uint8_t val);
void SetUARTBaudRate(uint8_t val);
void SetSpeed(uint8_t val);
void SetOptions(uint8_t val);
void SetChannel(uint8_t val);
void SetParityBit(uint8_t val);
	
//functions to set the options
void SetTransmissionMode(uint8_t val);
void SetPullupMode(uint8_t val);
void SetWORTIming(uint8_t val);
void SetFECMode(uint8_t val);
void SetTransmitPower(uint8_t val);
uint8_t GetAux();
uint8_t available();
void flush();
// methods to get some operating parameters
uint16_t GetAddress();
// methods to get module data
uint8_t GetModel();
uint8_t GetVersion();
uint8_t GetFeatures();
uint8_t GetAddressH();
uint8_t GetAddressL();
uint8_t GetAirDataRate();
uint8_t GetUARTBaudRate();
uint8_t GetChannel();
uint8_t GetParityBit();
uint8_t GetTransmissionMode();
uint8_t GetPullupMode();
uint8_t GetWORTIming();
uint8_t GetFECMode();
uint8_t GetTransmitPower();
uint8_t GetOptions();
uint8_t GetSpeed();
// methods to get data from sending unit
uint8_t GetByte();
uint8_t GetStruct(const void *TheStructure, uint16_t size_);
// method to send to data to receiving unit
void SendByte(uint8_t TheByte);
uint8_t SendStruct(const void *TheStructure, uint16_t size_);
// mehod to print parameters
void PrintParameters();
// parameters are set above but NOT saved, here's how you save parameters
// notion here is you can set several but save once as opposed to saving on each parameter change
// you can save permanently (retained at start up, or temp which is ideal for dynamically changing the address or frequency
void SaveParameters(uint8_t val = PERMANENT);
// MFG is not clear on what Reset does, but my testing indicates it clears buffer
// I use this when needing to restart the EBYTE after programming while data is still streaming in
// it does NOT return the ebyte back to factory defaults
void Reset();
// function to read modules parameters
uint8_t ReadParameters();
// method to let method know of module is busy doing something (timeout provided to avoid lockups)
void CompleteTask(unsigned long timeout = 0);
// utility funciton to build the "speed byte" which is a collection of a few different parameters
void BuildSpeedByte();
// utility funciton to build the "options byte" which is a collection of a few different parameters
void BuildOptionByte();
uint8_t ReadModelData();
void ClearBuffer();
// variable for the serial stream

typedef  struct{

	// pin variables
	int8_t _M0;
	int8_t _M1;
	int8_t _AUX;

	// variable for the 6 bytes that are sent to the module to program it
	// or bytes received to indicate modules programmed settings
	uint8_t _Params[6];

	// indicidual variables for each of the 6 bytes
	// _Params could be used as the main variable storage, but since some bytes
	// are a collection of several options, let's just make storage consistent
	// also Param[1] is different data depending on the _Save variable
	uint8_t _Save;
	uint8_t _AddressHigh;
	uint8_t _AddressLow;
	uint8_t _Speed;
	uint8_t _Channel;
	uint8_t _Options;
	
	// individual variables for all the options
	uint8_t _ParityBit;
	uint8_t _UARTDataRate;
	uint8_t _AirDataRate;
	uint8_t _OptionTrans;
	uint8_t _OptionPullup;
	uint8_t _OptionWakeup;
	uint8_t _OptionFEC;
	uint8_t _OptionPower;
	uint16_t _Address;
	uint8_t _Model;
	uint8_t _Version;
	uint8_t _Features;
	uint8_t _buf;

} LoraTypeDef;

#endif