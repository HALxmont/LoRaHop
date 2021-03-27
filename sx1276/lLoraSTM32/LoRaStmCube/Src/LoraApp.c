    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include "LoraApp.h"
    #include "stm32f1xx.h"
    #include "stm32f1xx_hal.h"
    #include "GPS.h"
    #include "HardwareInit.h"
    #include "fatfs.h"

    char        LoRaRxBuff[LRBuffSize];
    uint8_t     LoRaTxBuff[LoRaTxBufferSize];
    bool DID_FARO = false;
    extern UART_HandleTypeDef huart1;
    extern UART_HandleTypeDef huart2;
    extern UART_HandleTypeDef huart3;
    extern DMA_HandleTypeDef dma2;
    ModuleStatus LoRaStatus = Normal;
    LoRaModuleConf modulecfg;
    extern SDStatus sdstat;
    volatile uint8_t data_length;
    extern GPS_t GPS;
    LoraRx loraRx;
    LoraRxUAV loraRxUAV;


    void LORA_UART_IDLECallback(UART_HandleTypeDef *huart)
    {
        //Stop this DMA transmission
        HAL_UART_DMAStop(huart);         
    
        //Calculate the length of the received data
        data_length  = LRBuffSize - __HAL_DMA_GET_COUNTER(&dma2); 

        if(LoRaStatus == Normal){
            LoRaRxBuff[data_length+1] = 0x00;
            //HAL_UART_Transmit(&huart1,(uint8_t*) LoRaRxBuff,(uint16_t)data_length,0x700);    
            LoRaUAV();
            DecodeLoraFrame();
            SaveLoraFrame();
            
            
        }
        if(LoRaStatus == Program){
            modulecfg.addr = (uint16_t) ( (LoRaRxBuff[1] << 8) + LoRaRxBuff[2]);
            modulecfg.conf1 = LoRaRxBuff[3];
            modulecfg.chan = LoRaRxBuff[4];
            modulecfg.conf2 = LoRaRxBuff[5];
            myprintf("Adr = %d,Ch %d \n\r", modulecfg.addr, modulecfg.chan);
            LoRaStatus = Normal;
        }
        
        __LoRaLED_Toogle();
        data_length = 0;
        //Restart to start DMA transmission of LRBuffSize bytes of data at a time
        HAL_UART_Receive_DMA(huart, (uint8_t*)LoRaRxBuff, LRBuffSize);                    
    }

    uint8_t genByte(void){

        uint8_t res;
            res = rand()%255;
        return res;
    }

    void genPayload(void){

        uint8_t length;
        txbufferLoRa = NULL;
        length = rand()%112;
    // txbufferLoRa = (uint8_t*) malloc(length);
        if(txbufferLoRa != NULL){
            for(uint8_t i=2; i<length;i++)
                txbufferLoRa[i] = genByte();
        }
    }

    void sendFrameLoRa(void){

        static uint16_t counter;
        uint8_t tmp;
        genPayload();
        tmp = (uint8_t) counter >> 8;
        txbufferLoRa[0] = tmp;
        tmp = (uint8_t) counter;
        txbufferLoRa[1] = tmp;
        if(txbufferLoRa != NULL){
            HAL_UART_Transmit(&huart3, txbufferLoRa, sizeof(txbufferLoRa), 0x700);     
            counter++;
    //       free(txbufferLoRa);
        }
    }


    void SendFixMessage(void){
        uint8_t parameter_config[] = {0x00, DSTADDR, CHANNEL};
        char message[20] = ",Hi I'm LoRa!";

        
        //volatile uint8_t id_src = SRCADDR;
        volatile char node_src_message[20];
        sprintf(node_src_message, "%u", SRCADDR);
        strcat(node_src_message, message);
        

        HAL_UART_Transmit(&huart3, parameter_config, sizeof(parameter_config), 0x800);
        HAL_UART_Transmit(&huart3, node_src_message, sizeof(node_src_message), 0x800);   

    }



void LoRaFixMsg(const char *fmt, uint8_t CHNL, uint8_t HBDtarget, uint8_t LBDtarget) {
  static char buffer[256];
  __va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  int len = strlen(buffer);
  char config_send[] = {HBDtarget, LBDtarget, CHNL};
  
  HAL_UART_Transmit(&huart1, (char*)config_send, 3, -1);
  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, len, -1);
}



    void  LoRa_Init(void){
        __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE); 
        HAL_UART_Receive_DMA(&huart3, (uint8_t*)LoRaRxBuff, (uint16_t)LRBuffSize); 
    }

    void GetLoRaConfig(void){

    uint8_t cmd[] = {0xc1, 0xc1, 0xc1};
        M0_HIGH();
        M1_HIGH();
        LoRaStatus = Program;
        HAL_Delay(1);
        HAL_UART_Transmit(&huart3, cmd, 3 , 0x700);     
        HAL_Delay(1);
        M0_LOW();
        M1_LOW();
    }

    void send_string(char* s){
        HAL_UART_Transmit(&huart1, (uint8_t*)s, strlen(s), 1000);
    }

    void MakeLoraFrame(void){

    uint16_t dstaddr = DSTADDR;
    uint16_t srcaddr = SRCADDR;
    uint8_t  chan  = CHANNEL;
    static uint16_t index;
    uint8_t buffind = 2;
    uint8_t sbuff[58];
    uint8_t len;
    LoRaTxBuff[0] =  (uint8_t) (dstaddr >> 8);
    LoRaTxBuff[1] =  (uint8_t) (dstaddr);

    memcpy(&LoRaTxBuff[buffind], &chan, 1);
    buffind +=1; 
    memcpy(&LoRaTxBuff[buffind], &srcaddr, 2);
    buffind +=2; 
    memcpy(&LoRaTxBuff[buffind], &dstaddr, 2);
    buffind +=2; 
    memcpy(&LoRaTxBuff[buffind], &index, 2);
    buffind +=2; 
    memcpy(&LoRaTxBuff[buffind], &GPS.GPGGA.UTC_Year, 1);
    buffind +=1; 
    memcpy(&LoRaTxBuff[buffind], &GPS.GPGGA.UTC_Mon, 1);
    buffind +=1; 
    memcpy(&LoRaTxBuff[buffind], &GPS.GPGGA.UTC_Day, 1);
    buffind +=1; 
    memcpy(&LoRaTxBuff[buffind], &GPS.GPGGA.UTC_Hour, 1);
    buffind +=1; 
    memcpy(&LoRaTxBuff[buffind], &GPS.GPGGA.UTC_Min, 1);
    buffind +=1; 
    memcpy(&LoRaTxBuff[buffind], &GPS.GPGGA.UTC_Sec, 1);
    buffind +=1; 
    memcpy(&LoRaTxBuff[buffind], &GPS.GPGGA.Lat, 2);
    buffind +=2; 
    memcpy(&LoRaTxBuff[buffind], &GPS.GPGGA.LatDec, 4);
    buffind +=4; 
    memcpy(&LoRaTxBuff[buffind], &GPS.GPGGA.Lon, 2);
    buffind +=2; 
    memcpy(&LoRaTxBuff[buffind], &GPS.GPGGA.LonDec, 4);
    buffind +=4; 
    memcpy(&LoRaTxBuff[buffind], &GPS.GPGGA.MSL_Altitude, 4);
    buffind +=4; 
    memcpy(&LoRaTxBuff[buffind], &GPS.GPGGA.MSL_AltitudeDecimal, 4);
    buffind +=4; 
    memcpy(&LoRaTxBuff[buffind], &GPS.GPGGA.SatellitesUsed, 1);

    //HAL_UART_Transmit(&huart3,(uint8_t*) LoRaTxBuff,LoRaTxBufferSize,0x700);       
    index++; 

    len = sprintf(sbuff,"ddcg,%d,%d:%d:%d,%d.%d,%d.%d,%d.%d,%d\n\r", index, GPS.GPGGA.UTC_Hour, GPS.GPGGA.UTC_Min, GPS.GPGGA.UTC_Sec, GPS.GPGGA.Lat, GPS.GPGGA.LatDec, GPS.GPGGA.Lon, GPS.GPGGA.LonDec, GPS.GPGGA.MSL_Altitude, GPS.GPGGA.MSL_AltitudeDecimal, GPS.GPGGA.SatellitesUsed );
    sbuff[0] = 0x00;
    sbuff[1] = 0x08;
    sbuff[2] = 0x0f;
    HAL_UART_Transmit(&huart3,(uint8_t*) sbuff,len,0x700);       
    }

   void LoRaUAV(void){
        char msg[250];
        char string_msg[250];
        uint8_t msg_string_size = 0;

        //src, past_src, dst, mode


// ------- READ DATAFRAME CODES AND CONFIGURATIONS ----------
        loraRxUAV.past_src = LoRaRxBuff[0]; 
        loraRxUAV.src = LoRaRxBuff[1];      
        loraRxUAV.dst = LoRaRxBuff[2];
        loraRxUAV.dst_to_hop = LoRaRxBuff[3];
        loraRxUAV.mode = LoRaRxBuff[4];
// ------- --------------------------------------- ----------


// ------- READ INCOMING MESG ----------

for (uint8_t i = 5; i < 250; i++)
        {
            if (LoRaRxBuff[i] != '\000' && LoRaRxBuff[i] != '\n' && LoRaRxBuff[i] != '\r')  //read LoRaRxBuff. i>=4 because the data saved on dataframe can be 0 (someone of the four bytes)
            {
                msg[i-5] = LoRaRxBuff[i];       //save just msj  
                msg_string_size++;              //up msj size 
            }
            
            if (i == 249)   //end case
                msg[i] = '\0';
            
        }

                msg[msg_string_size] = '\r';
                msg[msg_string_size+1] = '\n';
                msg[msg_string_size+2] = '\0';
                msg_string_size = msg_string_size + 3;
                strcpy(string_msg, msg);  //char to string

            if (string_msg[msg_string_size-3] != '\r')
           {
               string_msg[msg_string_size-3] = ' ';
            }
           
// ------- -------------------------- ----------


void delay( uint32_t my_delay ){

    uint32_t tickL;
    uint32_t tickH;

    tickL = uwTick;  //take initial time

    while (tickH-tickL < my_delay){
        tickH = uwTick;  //take last time 
    }

}

// ------- COMUNNICATION MODE DETECTION AND ACTION ----------//


    //----------- HOP RESPONSE MODE --------------------//
if (loraRxUAV.mode == 1) {  //not hop mode. Just response

    //two possible casses: FARO NODE MODE or END NODE MODE

    //**** FARO MODE CASE *****//
    if (SRCADDR != loraRxUAV.dst_to_hop) {

/*
        //we need to chek if the string was parsed or not
        if (loraRxUAV.src == 0 && loraRxUAV.past_src == 0)
        {
            DID_FARO = false;  //REST DID_FARO
        } 
        if (!DID_FARO)
        {
                msg[msg_string_size+1] = '\r';
                msg[msg_string_size+2] = '\n';
                msg[msg_string_size+3] = '\0';
                msg_string_size = msg_string_size + 3;
                strcpy(string_msg, msg);  //char to string    
            if (string_msg[msg_string_size-3] != '\r')
           {
               string_msg[msg_string_size-3] = ' ';
           }    
        }

  */
        
        //fill structure
        loraRxUAV.past_src = LoRaRxBuff[0];
        loraRxUAV.src = LoRaRxBuff[2];  //update src(faro SRC)
        loraRxUAV.dst = LoRaRxBuff[3];
        loraRxUAV.dst_to_hop = LoRaRxBuff[3];//don't care
        loraRxUAV.mode = LoRaRxBuff[4];


        //transmit (HOP!)
        
        //HAL_Delay(1000); //basic delay
        uint8_t parameters_config[] = {0x00, loraRxUAV.dst, CHANNEL}; //set 
        uint8_t parameters_to_share[] = {loraRxUAV.past_src, loraRxUAV.src, loraRxUAV.dst, loraRxUAV.dst_to_hop, loraRxUAV.mode};
        uint8_t out[256];


        sprintf(out,"FARO NODE: %d,%d,%d,%d,%d,", loraRxUAV.past_src, loraRxUAV.src, loraRxUAV.dst, loraRxUAV.dst_to_hop, loraRxUAV.mode);

       // delay(3000);

        HAL_UART_Transmit(&huart3, parameters_config, sizeof(parameters_config), 0x800); //send basic config transmission
        //HAL_UART_Transmit(&huart3, out, 7, 0x800); //send info
        HAL_UART_Transmit(&huart3, parameters_to_share, sizeof(parameters_to_share) , 0x800); 
        HAL_UART_Transmit(&huart3, string_msg, msg_string_size , 0x800); //send info (in this case the same recived info)
        DID_FARO = true;

        myprintf("%s", out);
        //HAL_Delay(1);
        myprintf("%s", string_msg);
        //HAL_Delay(1);
    }
}
 
    //**** ENDNODE MODE CASE *****//
    if (SRCADDR == loraRxUAV.dst_to_hop)
    {

        //So we need to return a message to pst_source (initial node).
        //fill structure
        loraRxUAV.past_src = LoRaRxBuff[1]; //&src 
        loraRxUAV.src = LoRaRxBuff[2]; //= &DST = SRCADDR;  //update src (SRC)  
        loraRxUAV.dst = LoRaRxBuff[1];  //&src.  The last source was a faro, thats it's beceause we are the ENDNODE
        loraRxUAV.dst_to_hop = LoRaRxBuff[0];   //we want to return a message to a original source (pst_src)
        loraRxUAV.mode = LoRaRxBuff[4];


        //transmit (HOP!)
        //HAL_Delay(1000); //basic delay
        uint8_t parameters_config[] = {0x00, loraRxUAV.dst, CHANNEL}; //set 
        uint8_t parameters_to_share[] = {loraRxUAV.past_src, loraRxUAV.src, loraRxUAV.dst, loraRxUAV.dst_to_hop, loraRxUAV.mode};
        uint8_t out[256];


        sprintf(out,"ENDNODE: %d,%d,%d,%d,%d,", loraRxUAV.past_src, loraRxUAV.src, loraRxUAV.dst, loraRxUAV.dst_to_hop, loraRxUAV.mode);

        //delay(3000);
        HAL_UART_Transmit(&huart3, parameters_config, sizeof(parameters_config), 0x800); //send basic config transmission
        //HAL_UART_Transmit(&huart3, out, 7, 0x800); //send info
        HAL_UART_Transmit(&huart3, parameters_to_share, sizeof(parameters_to_share) , 0x800); 
        HAL_UART_Transmit(&huart3, string_msg, msg_string_size , 0x800); //send info (in this case the same recived info)
        //HAL_UART_Transmit(&huart3, '\r', 1 , 0x800); //send info (in this case the same recived info)
        //HAL_UART_Transmit(&huart3, '\n', 1 , 0x800); //send info (in this case the same recived info)

        myprintf("%s", out);
        myprintf("%s", string_msg);

    }        
    //----------- END HOP MODE RESPONSE --------------------//  




    //----------- NOT HOP RESPONSE MODE --------------------//
        if (loraRxUAV.mode == 2)  //not hop mode. Just response
        {
            uint8_t out[256];
            char new_line[] = {'\r', '\n'};
            char new_line_str[2];
            strcpy(new_line_str, new_line);  //char to string

            uint8_t parameters_config[] = {0x00, loraRxUAV.past_src, CHANNEL};
            uint8_t parameters_to_share[] = {loraRxUAV.past_src, loraRxUAV.src, loraRxUAV.dst, loraRxUAV.dst_to_hop, loraRxUAV.mode};
            sprintf(out,"%d,%d,%d,%d,%d,", loraRxUAV.past_src, loraRxUAV.src, loraRxUAV.dst, loraRxUAV.dst_to_hop, loraRxUAV.mode);

            //delay(3000);
            HAL_UART_Transmit(&huart3, parameters_config, sizeof(parameters_config), 0x800); //send basic config transmission
            HAL_UART_Transmit(&huart3, out, 7, 0x800); //send info
            //HAL_UART_Transmit(&huart3, parameters_to_share, sizeof(parameters_to_share) , 0x800); 
            HAL_UART_Transmit(&huart3, string_msg, msg_string_size , 0x800); //send info (in this case the same recived info)
            //HAL_UART_Transmit(&huart3, new_line_str, sizeof(new_line_str), 0x800);

            myprintf("%s", out);
            myprintf("%s", string_msg);

        }
    //----------- END NOT HOP RESPONSE MODE --------------------//  



// ------------- CLEAR COMUNICATION BUFFER ------------- //
        for (uint8_t i = 0; i < 250; i++)
        {
            msg[i] = NULL;
            string_msg[i] = NULL;
            
        }
    }




    void DecodeLoraFrame(void){

    //loraRx.src = LoRaRxBuff[2];
    uint8_t buffind = 0;
    memcpy(&loraRx.src,&LoRaRxBuff[buffind], 2);
    buffind +=2;
    memcpy(&loraRx.dst,&LoRaRxBuff[buffind], 2);
    buffind +=2;
    memcpy(&loraRx.index,&LoRaRxBuff[buffind], 2);
    buffind +=2;
    memcpy(&loraRx.UTC_Year,&LoRaRxBuff[buffind], 1);
    buffind +=1;
    memcpy(&loraRx.UTC_Mon,&LoRaRxBuff[buffind], 1);
    buffind +=1;
    memcpy(&loraRx.UTC_Day,&LoRaRxBuff[buffind], 1);
    buffind +=1;
    memcpy(&loraRx.UTC_Hour,&LoRaRxBuff[buffind], 1);
    buffind +=1;
    memcpy(&loraRx.UTC_Min,&LoRaRxBuff[buffind], 1);
    buffind +=1;
    memcpy(&loraRx.UTC_Sec,&LoRaRxBuff[buffind], 1);
    buffind +=1;
    memcpy(&loraRx.Latitude,&LoRaRxBuff[buffind], 2);
    buffind +=2;
    memcpy(&loraRx.LatitudeDecimal,&LoRaRxBuff[buffind], 4);
    buffind +=4;
    memcpy(&loraRx.Longitude,&LoRaRxBuff[buffind], 2);
    buffind +=2;
    memcpy(&loraRx.LongitudeDecimal,&LoRaRxBuff[buffind], 4);
    buffind +=4;
    memcpy(&loraRx.MSL_Altitude,&LoRaRxBuff[buffind], 4);
    buffind +=4;
    memcpy(&loraRx.MSL_AltitudeDecimal,&LoRaRxBuff[buffind], 4);
    buffind +=4;
    memcpy(&loraRx.SatellitesUsed,&LoRaRxBuff[buffind], 1);

    memset(LoRaRxBuff,0, LRBuffSize);
    //send_string("I'm awake!\r\n");

    }
    void SaveLoraFrame(void){

    static FIL fil; 		
    FRESULT fres; 		
    UINT bytesWrote,len;
    static uint8_t cnt;
    static uint8_t postfix;
    char filename[30];   
    int8_t out[256];
    sprintf(filename,"LoRa-%d.txt", postfix);
    cnt = (cnt + 1) % 100;
    if (cnt == 0)
        postfix++;
    sprintf(out,"%d,%d,%d,20%d-%d-%dT%d:%d:%d,%d.%d,%d.%d,%d.%d,%d\n", loraRx.src,  loraRx.dst, loraRx.index,  loraRx.UTC_Year,  loraRx.UTC_Mon, loraRx.UTC_Day, loraRx.UTC_Hour,   loraRx.UTC_Min,  loraRx.UTC_Sec,  loraRx.Latitude,  loraRx.LatitudeDecimal,  loraRx.Longitude,  loraRx.LongitudeDecimal,  loraRx.MSL_Altitude,  loraRx.MSL_AltitudeDecimal,  loraRx.SatellitesUsed ); 
    //myprintf("LoRa: %s", out);  
    //myprintf("LoRa: %d,%d,%d,20%d-%d-%dT%d:%d:%d,%d.%d,%d.%d,%d.%d,%d\n", loraRx.src,  loraRx.dst, loraRx.index,  loraRx.UTC_Year,  loraRx.UTC_Mon, loraRx.UTC_Day, loraRx.UTC_Hour,   loraRx.UTC_Min,  loraRx.UTC_Sec,  loraRx.Latitude,  loraRx.LatitudeDecimal,  loraRx.Longitude,  loraRx.LongitudeDecimal,  loraRx.MSL_Altitude,  loraRx.MSL_AltitudeDecimal,  loraRx.SatellitesUsed);
    //myprintf("SRC: %d", loraRx.src);

    if(out != NULL){
        len = strlen(out);
        out[len] = 0x00;
        if ((sdstat != 0) && (loraRx.SatellitesUsed)){
            __disable_irq();
            fres = f_open(&fil, filename, FA_WRITE | FA_OPEN_ALWAYS );
            if(fres == FR_OK) {
                if(DEBUG)
                    myprintf("I was able to open file for writing\r\n");
            } 
            else {
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
        }		
    }