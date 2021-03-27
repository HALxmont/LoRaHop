

// uint8_t init()
// {

//     uint8_t ok = 0;

//     // first get the module data (must be called first for some odd reason

//     ok = ReadModelData();

//     if (!ok)
//     {
//         return 1;
//     }
//     // now get parameters to put unit defaults into the class variables

//     ok = ReadParameters();
//     if (!ok)
//     {
//         return 1;
//     }

//     return 0;
// }

// /*
// Utility method to wait until module is doen tranmitting
// a timeout is provided to avoid an infinite loop
// */

// /*
// method to set the mode (program, normal, etc.)
// */

// void SetMode(uint8_t mode)
// {

//     // data sheet claims module needs some extra time after mode setting (2ms)
//     // most of my projects uses 10 ms, but 40ms is safer

//     delay(PIN_RECOVER);

//     if (mode == MODE_NORMAL)
//     {

//         digitalWrite(_M0, LOW);
//         digitalWrite(_M1, LOW);
//     }
//     else if (mode == MODE_WAKEUP)
//     {

//         digitalWrite(_M0, HIGH);
//         digitalWrite(_M1, LOW);
//     }
//     else if (mode == MODE_POWERDOWN)
//     {
//         digitalWrite(_M0, LOW);
//         digitalWrite(_M1, HIGH);
//     }
//     else if (mode == MODE_PROGRAM)
//     {
//         digitalWrite(_M0, HIGH);
//         digitalWrite(_M1, HIGH);
//     }

//     // data sheet says 2ms later control is returned, let's give just a bit more time
//     // these modules can take time to activate pins
//     delay(PIN_RECOVER);

//     // clear out any junk
//     // added rev 5
//     // i've had some issues where after programming, the returned model is 0, and all settings appear to be corrupt
//     // i imagine the issue is due to the internal buffer full of junk, hence clearing
//     // Reset() *MAY* work but this seems better.
//     ClearBuffer();

//     // wait until aux pin goes back low
//     CompleteTask(4000);
// }

// // i've asked EBYTE what's supposed to happen--got an unclear answer
// // but my testing indicates it clears buffer
// // I use this when needing to restart the EBYTE after programming while data is still streaming in
// // to let the unit start reading from a cleared internal buffer

// // it does NOT return the ebyte back to factory defaults
// // if your unit gets corrupt or you need to restore values, you will have to do brute force
// // example for and E44-915
// // look at the data sheet for default values
// //  Trans.SetAddressH(0);
// //  Trans.SetAddressL(0);
// //  Trans.SetSpeed(0b00011100);
// //  Trans.SetChannel(1);
// //  Trans.SetOptions(0b01000100);
// //  Trans.SaveParameters(PERMANENT);

// void Reset()
// {

//     SetMode(MODE_PROGRAM);

//     delay(50); // not sure you need this

//     _s->write(0xC4);
//     _s->write(0xC4);
//     _s->write(0xC4);

//     CompleteTask(4000);

//     SetMode(MODE_NORMAL);
// }

// void SetSpeed(uint8_t val)
// {
//     _Speed = val;
// }
// void SetOptions(uint8_t val)
// {
//     _Options = val;
// }
// uint8_t GetSpeed()
// {
//     return _Speed;
// }
// uint8_t GetOptions()
// {
//     return _Options;
// }

// /*
// method to set the high bit of the address
// */

// void SetAddressH(uint8_t val)
// {
//     _AddressHigh = val;
// }

// uint8_t GetAddressH()
// {
//     return _AddressHigh;
// }

// /*
// method to set the lo bit of the address
// */

// void SetAddressL(uint8_t val)
// {
//     _AddressLow = val;
// }

// uint8_t GetAddressL()
// {
//     return _AddressLow;
// }

// /*
// method to set the channel
// */

// void SetChannel(uint8_t val)
// {
//     _Channel = val;
// }
// uint8_t GetChannel()
// {
//     return _Channel;
// }

// /*
// method to set the air data rate
// */

// void SetAirDataRate(uint8_t val)
// {

//     _AirDataRate = val;
//     BuildSpeedByte();
// }

// uint8_t GetAirDataRate()
// {
//     return _AirDataRate;
// }

// /*
// method to set the parity bit
// */

// void SetParityBit(uint8_t val)
// {
//     _ParityBit = val;
//     BuildSpeedByte();
// }
// uint8_t GetParityBit()
// {
//     return _ParityBit;
// }

// /*
// method to set the options bits
// */

// void SetTransmissionMode(uint8_t val)
// {
//     _OptionTrans = val;
//     BuildOptionByte();
// }
// uint8_t GetTransmissionMode()
// {
//     return _OptionTrans;
// }

// void SetPullupMode(uint8_t val)
// {
//     _OptionPullup = val;
//     BuildOptionByte();
// }
// uint8_t GetPullupMode()
// {
//     return _OptionPullup;
// }

// void SetWORTIming(uint8_t val)
// {
//     _OptionWakeup = val;
//     BuildOptionByte();
// }
// uint8_t GetWORTIming()
// {
//     return _OptionWakeup;
// }

// void SetFECMode(uint8_t val)
// {
//     _OptionFEC = val;
//     BuildOptionByte();
// }
// uint8_t GetFECMode()
// {
//     return _OptionFEC;
// }

// void SetTransmitPower(uint8_t val)
// {

//     _OptionPower = val;
//     BuildOptionByte();
// }

// uint8_t GetTransmitPower()
// {
//     return _OptionPower;
// }

// /*
// method to compute the address based on high and low bits
// */

// void SetAddress(uint16_t Val)
// {
//     _AddressHigh = ((Val & 0xFFFF) >> 8);
//     _AddressLow = (Val & 0xFF);
// }

// /*
// method to get the address which is a collection of hi and lo bytes
// */

// uint16_t GetAddress()
// {
//     return (_AddressHigh << 8) | (_AddressLow);
// }

// /*
// set the UART baud rate
// */

// void SetUARTBaudRate(uint8_t val)
// {
//     _UARTDataRate = val;
//     BuildSpeedByte();
// }

// uint8_t GetUARTBaudRate()
// {
//     return _UARTDataRate;
// }

// /*
// method to build the byte for programming (notice it's a collection of a few variables)
// */
// void BuildSpeedByte()
// {
//     _Speed = 0;
//     _Speed = ((_ParityBit & 0xFF) << 6) | ((_UARTDataRate & 0xFF) << 3) | (_AirDataRate & 0xFF);
// }

// /*
// method to build the option byte for programming (notice it's a collection of a few variables)
// */

// void BuildOptionByte()
// {
//     _Options = 0;
//     _Options = ((_OptionTrans & 0xFF) << 7) | ((_OptionPullup & 0xFF) << 6) | ((_OptionWakeup & 0xFF) << 3) | ((_OptionFEC & 0xFF) << 2) | (_OptionPower & 0b11);
// }

// uint8_t GetAux()
// {
//     return digitalRead(_AUX);
// }

// /*
// method to save parameters to the module
// */

// void SaveParameters(uint8_t val)
// {

//     SetMode(MODE_PROGRAM);

//     // ClearBuffer();

//     // here you can save permanenly or temp
//     delay(5);

//     _s->write(val);
//     _s->write(_AddressHigh);
//     _s->write(_AddressLow);
//     _s->write(_Speed);
//     _s->write(_Channel);
//     _s->write(_Options);

//     delay(50);

//     CompleteTask(4000);

//     SetMode(MODE_NORMAL);
// }

// /*
// method to print parameters, this can be called anytime after init(), because init gets parameters
// and any method updates the variables
// */

// void PrintParameters()
// {

//     _ParityBit = (_Speed & 0XC0) >> 6;
//     _UARTDataRate = (_Speed & 0X38) >> 3;
//     _AirDataRate = _Speed & 0X07;

//     _OptionTrans = (_Options & 0X80) >> 7;
//     _OptionPullup = (_Options & 0X40) >> 6;
//     _OptionWakeup = (_Options & 0X38) >> 3;
//     _OptionFEC = (_Options & 0X07) >> 2;
//     _OptionPower = (_Options & 0X03);
// }

// /*
// method to read parameters, 
// */

// uint8_t ReadParameters()
// {

//     _Params[0] = 0;
//     _Params[1] = 0;
//     _Params[2] = 0;
//     _Params[3] = 0;
//     _Params[4] = 0;
//     _Params[5] = 0;

//     SetMode(MODE_PROGRAM);

//     _s->write(0xC1);

//     _s->write(0xC1);

//     _s->write(0xC1);

//     _s->readBytes((uint8_t *)&_Params, (uint8_t)sizeof(_Params));

//     _Save = _Params[0];
//     _AddressHigh = _Params[1];
//     _AddressLow = _Params[2];
//     _Speed = _Params[3];
//     _Channel = _Params[4];
//     _Options = _Params[5];

//     _Address = (_AddressHigh << 8) | (_AddressLow);
//     _ParityBit = (_Speed & 0XC0) >> 6;
//     _UARTDataRate = (_Speed & 0X38) >> 3;
//     _AirDataRate = _Speed & 0X07;

//     _OptionTrans = (_Options & 0X80) >> 7;
//     _OptionPullup = (_Options & 0X40) >> 6;
//     _OptionWakeup = (_Options & 0X38) >> 3;
//     _OptionFEC = (_Options & 0X07) >> 2;
//     _OptionPower = (_Options & 0X03);

//     SetMode(MODE_NORMAL);

//     if (0xC0 != _Params[0])
//     {

//         return 1;
//     }

//     return 0;
// }

// uint8_t ReadModelData()
// {

//     _Params[0] = 0;
//     _Params[1] = 0;
//     _Params[2] = 0;
//     _Params[3] = 0;
//     _Params[4] = 0;
//     _Params[5] = 0;

//     uint8_t found = 1;
//     int i = 0;

//     SetMode(MODE_PROGRAM);

//     _s->write(0xC3);

//     _s->write(0xC3);

//     _s->write(0xC3);

//     _s->readBytes((uint8_t *)&_Params, (uint8_t)sizeof(_Params));

//     _Save = _Params[0];
//     _Model = _Params[1];
//     _Version = _Params[2];
//     _Features = _Params[3];

//     if (0xC3 != _Params[0])
//     {

//         for (i = 0; i < 5; i++)
//         {
//             Serial.print("trying: ");
//             Serial.println(i);
//             _Params[0] = 0;
//             _Params[1] = 0;
//             _Params[2] = 0;
//             _Params[3] = 0;
//             _Params[4] = 0;
//             _Params[5] = 0;
//             delay(100);

//             _s->write(0xC3);

//             _s->write(0xC3);

//             _s->write(0xC3);

//             _s->readBytes((uint8_t *)&_Params, (uint8_t)sizeof(_Params));

//             if (0xC3 == _Params[0])
//             {
//                 found = 0;
//                 break;
//             }
//         }
//     }
//     else
//     {
//         found = 0;
//     }

//     SetMode(MODE_NORMAL);

//     CompleteTask(1000);

//     return found;
// }

// /*
// method to get module model and E50-TTL-100 will return 50
// */

// uint8_t GetModel()
// {

//     return _Model;
// }

// /*
// method to get module version (undocumented as to the value)
// */

// uint8_t GetVersion()
// {

//     return _Version;
// }

// /*
// method to get module version (undocumented as to the value)
// */

// uint8_t GetFeatures()
// {

//     return _Features;
// }

// /*
// method to clear the serial buffer

// without clearing the buffer, i find getting the parameters very unreliable after programming.
// i suspect stuff in the buffer affects rogramming 
// hence, let's clean it out
// this is called as part of the setmode

// */
// void ClearBuffer()
// {

//     unsigned long amt = millis();

//     while (_s->available())
//     {
//         _s->read();
//         if ((millis() - amt) > 5000)
//         {
//             Serial.println("runaway");
//             break;
//         }
//     }
// }
