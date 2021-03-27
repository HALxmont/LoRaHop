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

  SDStatus sdstat;

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


  
  
    GPS_Init();
    //LoRa_Init();
    
    tickl = uwTick;  //take initial time

    while (1){
    tickh = uwTick;  //take last time 

      if(tickh-tickl > MainLoopDly){  //every MainLoopDly we will do something
      
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

        tickl = uwTick;
      }
    }
  }





