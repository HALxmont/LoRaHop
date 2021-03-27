#include "GPSConfig.h"
#include "GPS.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include"stm32f1xx_hal.h"
#include "LoraApp.h"
#include "fatfs.h"

GPS_t GPS; 
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef dma1;
extern SDStatus sdstat;

volatile uint8_t data_length;

//##################################################################################################################
double convertDegMinToDecDeg (float degMin)
{
  double min = 0.0;
  double decDeg = 0.0;
 
  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);
 
  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );
 
  return decDeg;
}
//##################################################################################################################
void	GPS_Init(void)
{
	GPS.rxIndex=0;
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE); 
	HAL_UART_Receive_DMA(&huart2, (uint8_t*)GPS.rxBuffer, GPSBuffLength); 
		
}
//##################################################################################################################
void	GPS_CallBack(void)
{
	GPS.LastTime=HAL_GetTick();
	if(GPS.rxIndex < sizeof(GPS.rxBuffer)-2)
	{
		GPS.rxBuffer[GPS.rxIndex] = GPS.rxTmp;
		GPS.rxIndex++;
	}	
	//HAL_UART_Receive_IT(&_GPS_USART,&GPS.rxTmp,1);
}
//##################################################################################################################
void	GPS_Process(void)
{

const char delim[2] = ",";
char *token = NULL;
uint8_t tokenIndex = 0, l1;
char t1[20];
char *t2;
int8_t tagetRxBuff[GPSBuffLength];

memset(tagetRxBuff, 0, GPSBuffLength);
GPS_Delimit(tagetRxBuff, GPS.rxBuffer, "$GPGGA");

	if(  (GPS.rxIndex>0))
	{
    	if(!strncmp(tagetRxBuff, "$GPGGA", 6)){
        	/* get the first token */
        	token = strtok(tagetRxBuff, delim);      
    		/* walk through other tokens */
    		while( token != NULL ) {
        		
				token = strtok(NULL, delim);
        		tokenIndex++;

        		switch(tokenIndex){
            		case 1: t1[0] = token[0];
                    		t1[1] = token[1];
							t1[2] = '\0';
							GPS.GPGGA.UTC_Hour = atoi(t1);
							t1[0] = token[2];
							t1[1] = token[3];
							t1[2] = '\0';
							GPS.GPGGA.UTC_Min = atoi(t1);
                    		t1[0] = token[4];
                    		t1[1] = token[5];
                    		t1[2] = '\0';
                    		GPS.GPGGA.UTC_Sec = atoi(t1);
                    break; 

		            case 2: t2 = strchr(token, '.');
						    t2 = &token[t2-token+1];
							GPS.GPGGA.Latitude  = atoi(token);
						    GPS.GPGGA.LatitudeDecimal = atoi(t2);
            		break;
	    
					case 3: GPS.GPGGA.NS_Indicator = (char) token;
					break;

					case 4: t2 = strchr(token, '.');
							t2 = &token[t2-token+1];
							GPS.GPGGA.Longitude  = atoi(token);
							GPS.GPGGA.LongitudeDecimal = atoi(t2);
					break;
				
					case 5: GPS.GPGGA.EW_Indicator = (char) token;
					break;

					case 6: GPS.GPGGA.PositionFixIndicator = atoi(token);
					break;
				
					case 7: GPS.GPGGA.SatellitesUsed = atoi(token);
					break;
	
					case 8: t2 = strchr(token, '.');
							t2 = &token[t2-token+1];
							GPS.GPGGA.HDOP = atoi(token);
							GPS.GPGGA.HDOPDecimal = atoi(t2);
					break;
	   
					case 9: t2 = strchr(token, '.');
							t2 = &token[t2-token+1];
							GPS.GPGGA.MSL_Altitude  = atoi(token);
							GPS.GPGGA.MSL_AltitudeDecimal = atoi(t2);
					break;
	
	
					case 10: GPS.GPGGA.MSL_Units = (char) token;
					break;

					case 11: t2 = strchr(token, '.');
							t2 = &token[t2-token+1];
							GPS.GPGGA.Geoid_Separation  = atoi(token);
							GPS.GPGGA.Geoid_SeparationDecimal = atoi(t2);
					break;
				
				
					case 12: GPS.GPGGA.Geoid_Units = (char) token;
	    			break;
	
					case 13: t2 = strchr(token, '.');
						     t2 = &token[t2-token+1];
							 GPS.GPGGA.AgeofDiffCorr  = atoi(token);
							 GPS.GPGGA.AgeofDiffCorrDecimal = atoi(t2);
					break;
					case 14: t2 = strchr(token, '*');
							 l1 = strlen(token) - (t2-token);
					 		 memcpy(t1, token, l1+1);
							 GPS.GPGGA.DiffRefStationID = atoi(t1);
					break;
   	     		} //endcase
			} //endwhile
		//memset(GPS.rxBuffer,0,sizeof(GPS.rxBuffer));
		GPS.rxIndex=0;		
		} // endif	
	memset(tagetRxBuff, 0, GPSBuffLength);
	GPS_Delimit(tagetRxBuff, GPS.rxBuffer, "$GPRMC");	
	if(!strncmp(tagetRxBuff, "$GPRMC", 6)){
        	/* get the first token */
        	token = strtok(tagetRxBuff, delim);      
    		/* walk through other tokens */
    		while( token != NULL ) {
        		
				token = strtok(NULL, delim);
        		tokenIndex++;

        		switch(tokenIndex){
            		case 1: 
                    break;

		            case 2: 
            		break;
	    
					case 3:
					break;

					case 4: 
					break;
				
					case 5: 
					break;

					case 6: 
					break;
				
					case 7: 
					break;
	   
					case 8: t1[0] = token[0];
                    		t1[1] = token[1];
							t1[2] = '\0';
							GPS.GPGGA.UTC_Day = atoi(t1);
							t1[0] = token[2];
							t1[1] = token[3];
							t1[2] = '\0';
							GPS.GPGGA.UTC_Mon = atoi(t1);
                    		t1[0] = token[4];
                    		t1[1] = token[5];
                    		t1[2] = '\0';
                    		GPS.GPGGA.UTC_Year = atoi(t1);
					break;
   	     		} //endcase
			} //endwhile
		GPS.rxIndex=0;		
		} // endif	
   	}  //endif
}

void GPS_Delimit(int8_t *dst, uint8_t *src, const char *ref ){

 uint8_t *start, *stop, len;

	start = (uint8_t*) strstr(src, ref);
	stop  = (uint8_t*) strchr(start, '*');
	len = stop-start;
	if(len>0){
		memcpy(dst, (uint8_t*) start, len);
		dst[len] = 0;
		GPS.rxIndex = len;
	}
}

void GPS_UART_IDLECallback(UART_HandleTypeDef *huart)
{
	//Stop this DMA transmission
    HAL_UART_DMAStop(huart);  
    //Calculate the length of the received data
    data_length  = GPSBuffLength - __HAL_DMA_GET_COUNTER(&dma1);   
	//HAL_UART_Transmit(&huart1,GPS.rxBuffer,data_length,0x200); 
	GPS_Process();
	DMStoDecimal();
	if(GPS.GPGGA.SatellitesUsed > 3)
	 	__GPSLED_Toogle();
	GPS.rxIndex = data_length;        
    data_length = 0;
    //Restart to start DMA transmission of 255 bytes of data at a time
    HAL_UART_Receive_DMA(huart, (uint8_t*)GPS.rxBuffer, GPSBuffLength);                    
}


void GPS_MakeString(){

 char *out;
 static FIL fil; 		//File handle
 FRESULT fres; //Result after operations
 UINT bytesWrote,len;
 static uint8_t cnt;
 static uint8_t postfix;
 char filename[20];   

 out  = (int8_t*) malloc(data_length+3);
 sprintf(filename,"GPS-%d.txt", postfix);
 cnt = (cnt + 1) % 100;
 if (cnt == 0)
  	postfix++;
	
 sprintf(out,"20%d-%d-%dT%d:%d:%d,%d.%d,%d.%d,%d.%d\n",GPS.GPGGA.UTC_Year, GPS.GPGGA.UTC_Mon, GPS.GPGGA.UTC_Day, GPS.GPGGA.UTC_Hour, GPS.GPGGA.UTC_Min,GPS.GPGGA.UTC_Sec, GPS.GPGGA.Lat, GPS.GPGGA.LatDec, GPS.GPGGA.Lon, GPS.GPGGA.LonDec, GPS.GPGGA.MSL_Altitude, GPS.GPGGA.MSL_AltitudeDecimal);
 myprintf("GPS: %s", out); 
 len = strlen(out);
//	HAL_UART_Transmit(&huart1,out,len,0x700);
 if ((sdstat != 0) && (GPS.GPGGA.SatellitesUsed != 0) ){
 __disable_irq();
    fres = f_open(&fil, filename, FA_WRITE | FA_OPEN_ALWAYS );
    if(fres == FR_OK) {
  		if(DEBUG)
	  		myprintf("I was able to open file for writing\r\n");
    } else {
	if(DEBUG)
	  	myprintf("f_open error (%i)\r\n", fres);
    }
      
    fres = f_lseek(&fil, f_size(&fil)) ;
	if(fres != FR_OK) {
  		if(DEBUG)
	  		myprintf("Fseek failed!\r\n");
    } 
	
    fres = f_write(&fil, out, len, &bytesWrote);
    //f_printf(&fil, "%s", out);
	if(fres == FR_OK) {
		if(DEBUG)
	  		myprintf("Wrote %i bytes to file \r\n", bytesWrote);
    } 
	else {
		if(DEBUG)
	  		myprintf("f_write error (%i)\r\n");
	}
	f_close(&fil);
 __enable_irq();
 }		
  free(out);
}

void DMStoDecimal(void){

float flat,  min;
unsigned short  clat, clatd;
char  ns = 'S';
double latdec, latdecd;

latdecd = (double) GPS.GPGGA.LatitudeDecimal/100000;
//latdecd = (double) 54700/100000;
flat  = (float) GPS.GPGGA.Latitude/100;
///flat  = (float) 6209/100;
clat  = (unsigned short) GPS.GPGGA.Latitude/100;
//clat  = (unsigned short) 6209/100;
min  =  (flat - clat)*100;

latdec = (double) (clat + (min + latdecd)/60);

GPS.GPGGA.Lat =  (short) -latdec;

latdecd = (double) (latdec - clat)*100000;
GPS.GPGGA.LatDec = (unsigned short) latdecd;

myprintf(" Lat: %d, Latdec: %d\n",  clat, clatd);

latdecd = (double) GPS.GPGGA.LongitudeDecimal/100000;
//latdecd = (double) 24700/100000;
flat  = (float) GPS.GPGGA.Longitude/100;
//flat  = (float) 5828/100;
clat  = (unsigned short) GPS.GPGGA.Longitude/100;
//clat  = (unsigned short) 5828/100;
min  =  (flat - clat)*100;

latdec = (double) (clat + (min + latdecd)/60);

GPS.GPGGA.Lon = (short) -latdec;

latdecd = (double) (latdec - clat)*100000;
GPS.GPGGA.LonDec = (unsigned short) latdecd;

myprintf("Lon: %d, %d\n",  clat, clatd);
myprintf("Sat: %d\n", GPS.GPGGA.SatellitesUsed);

}

//##################################################################################################################
