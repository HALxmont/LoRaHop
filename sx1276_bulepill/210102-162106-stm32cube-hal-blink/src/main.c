  #include <string.h>
  #include <stdlib.h>
  #include <stdarg.h>
  #include "stm32f1xx_hal.h"
  #include "stm32f1xx_hal_dma.h"
  #include "LoraApp.h"
  #include "GPS.h"
  #include "HardwareInit.h"
  #include "fatfs.h"
  #include "ff.h"
  #include "main.h"
  #include "SX1278.h"
  #include "SX1278_hw.h"

  SDStatus sdstat;




//--------------- for sx1276/77/78 --------------- //
extern SPI_HandleTypeDef  hspi2;
uint8_t master = 0;  //Set Slave (0) or Master (1)
uint8_t ret;
char buffer[512];
int message;
int message_length;

//--------------- END for sx1276/77/78 --------------- //
  
  int main(void)
  {
    FATFS FatFs; 	
    FRESULT fres; 
    DWORD free_clusters, free_sectors, total_sectors;
    FATFS* getFreeFs;
    uint32_t tickh, tickl;  //counters
    
    SystemClock_Config();
    HardwareInit();
    MX_FATFS_Init();
    HAL_Delay(1);
    sdstat = 1;
  
    myprintf("Hi I am %s\r\n", MNAME);


  /*
    fres = f_mount(&FatFs, "", 1); 
    if (fres != FR_OK) {
      if(DEBUG)
        myprintf("f_mount error (%i)\r\n", fres);  
      sdstat = 0;
    }
    fres = f_getfree("", &free_clusters, &getFreeFs);
    if (fres != FR_OK) {
      if(DEBUG)
        myprintf("f_getfree error (%i)\r\n", fres);
        sdstat = 0;
    }
    else{
      total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
      free_sectors = free_clusters * getFreeFs->csize;
      myprintf("SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);
      }

    if(sdstat == 0){
      myprintf("SD card error\n\r");
      __LoRaLED_On();
      __GPSLED_On();
      while(1); //infinite loop
    }  

  */

SX1278_hw_t SX1278_hw;
SX1278_t SX1278;


	SX1278_hw.dio0.port = DIO0_GPIO_Port;
	SX1278_hw.dio0.pin = DIO0_Pin;
	SX1278_hw.nss.port = NSS_GPIO_Port;
	SX1278_hw.nss.pin = NSS_Pin;
	SX1278_hw.reset.port = RESET_GPIO_Port;
	SX1278_hw.reset.pin = RESET_Pin;
	SX1278_hw.spi = &hspi2;

SX1278.hw = &SX1278_hw;

SX1278_init(&SX1278, 915000000, SX1278_POWER_17DBM, SX1278_LORA_SF_7,
	SX1278_LORA_BW_125KHZ, SX1278_LORA_CR_4_5, SX1278_LORA_CRC_EN, 10);

	if (master == 1) {
		ret = SX1278_LoRaEntryTx(&SX1278, 16, 2000);
		//HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	} else {
		ret = SX1278_LoRaEntryRx(&SX1278, 16, 2000);
		//HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	}

    //GPS_Init();
    //LoRa_Init();
    
    tickl = uwTick;  //take initial time

    while (1){
    tickh = uwTick;  //take last time 

      //if(tickh-tickl > MainLoopDly){  //every MainLoopDly we will do something
      
      //**** my code test ****

      //send_string("I'm awake!\r\n");
      //SendFixMessage();

      //**** end  my code test ****



      /* kh function
        if(GPS.GPGGA.SatellitesUsed > 3)
            GPS_MakeString();
            MakeLoraFrame();
        __MCULED_Toogle();
      */


      
    //--------------- BEGING TEST SX1276/77/78 --------------- //

    		if (master == 1) {
			myprintf("Master ...\r\n");
			HAL_Delay(1000);
			myprintf("Sending package...\r\n");

			message_length = sprintf(buffer, "PING!\r\n");
			ret = SX1278_LoRaEntryTx(&SX1278, message_length, 2000);
			//myprintf("Entry: %d\r\n", ret);

			myprintf("Sending %s\r\n", buffer);
			ret = SX1278_LoRaTxPacket(&SX1278, (uint8_t*) buffer,
					message_length, 2000);
			message += 1;

			//myprintf("Transmission: %d\r\n", ret);
			//myprintf("Package sent...\r\n");

		} else {
			//myprintf("Slave ...\r\n");
			HAL_Delay(800);
			myprintf("Receiving package...\r\n");

			ret = SX1278_LoRaRxPacket(&SX1278);
			//myprintf("Received: %d\r\n", ret);
			if (ret > 0) {
				SX1278_read(&SX1278, (uint8_t*) buffer, ret);
				myprintf("Content (%d): %s, snr: %d, rssi: %d, crssi: %d\r\n", ret, buffer,SX1278_LastSNR(&SX1278),SX1278_LastRSSI(&SX1278), SX1278_RSSI_LoRa(&SX1278));
			}
			//myprintf("Package received ...\r\n");

		}
HAL_Delay(5000);
    //--------------- END TEST SX1276/77/78 --------------- //





        tickl = uwTick;
      //}
    }
  }





