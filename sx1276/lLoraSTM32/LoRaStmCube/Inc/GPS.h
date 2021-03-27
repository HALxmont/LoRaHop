#ifndef _GPS_H_
#define _GPS_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"

#define GPSBuffLength  256

//##################################################################################################################
typedef struct
{
	uint8_t		UTC_Hour;
	uint8_t		UTC_Min;
	uint8_t		UTC_Sec;
	uint8_t		UTC_Day;
	uint8_t		UTC_Mon;
	uint8_t		UTC_Year;
	
	uint16_t		Latitude;
	uint32_t		LatitudeDecimal;
	int16_t			Lat;
	uint32_t		LatDec;
	char			NS_Indicator;
	uint16_t		Longitude;
	uint32_t		LongitudeDecimal;
	int16_t			Lon;
	uint32_t		LonDec;
	char			EW_Indicator;
	
	uint8_t		PositionFixIndicator;
	uint8_t		SatellitesUsed;
	int			HDOP;
	int 		HDOPDecimal;
	int			MSL_Altitude;
	int			MSL_AltitudeDecimal;
	char		MSL_Units;
	int			Geoid_Separation;
	int			Geoid_SeparationDecimal;
	char		Geoid_Units;
	
	int16_t		AgeofDiffCorr;
	int16_t  	AgeofDiffCorrDecimal;
	uint8_t		DiffRefStationID;
		
	
}GPGGA_t;

typedef struct 
{
	uint8_t	rxBuffer[GPSBuffLength];
	uint16_t	rxIndex;
	uint8_t	rxTmp;	
	uint32_t	LastTime;	
	
	GPGGA_t		GPGGA;
	
}GPS_t;

extern GPS_t GPS;
//##################################################################################################################
void GPS_Init(void);
void GPS_CallBack(void);
void GPS_Process(void);
void GPS_MakeString();
void GPS_Delimit(int8_t *dst, uint8_t *src, const char *ref );
void GPS_UART_IDLECallback(UART_HandleTypeDef *huart);
void DMStoDecimal(void);
//##################################################################################################################
#endif
