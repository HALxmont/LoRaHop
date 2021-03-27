#ifndef LORAAPP_H_
#define LORAAPP_H_
#include"stm32f1xx.h"

#define LED_PIN                                GPIO_PIN_13
#define LED_GPIO_PORT                          GPIOC
#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOC_CLK_ENABLE()
#define M1_LOW()                               HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, RESET);
#define M0_LOW()                               HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, RESET);
#define M1_HIGH()                              HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, SET);
#define M0_HIGH()                              HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, SET);
#define __LoRaLED_Toogle()                     HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
#define __LoRaLED_On()                         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
#define __GPSLED_Toogle()                      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
#define __GPSLED_On()                          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
#define __MCULED_Toogle()                      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
#define LRBuffSize 256
#define MNAME   "mxmont"
#define MainLoopDly   5000
#define LoRaTxBufferSize 36
#define DSTADDR           0x00
#define SRCADDR           0x01
#define CHANNEL           0x0f


uint8_t *txbufferLoRa;

typedef enum {appIDLE = 0, AppTransmitting, AppReceiving, AppConfiguration} AppStatus;
typedef enum {Normal  = 0, WakeUp, PowerDown, Program} ModuleStatus;
/*
1. First 2 bytes as upcounter
2. Random number generator
3. Frame length fixed ot not
4. Exceed 58 bytes of payload or no
5. 2 files Rx.txt and Tx.txt 
*/
typedef struct{
    uint16_t addr;
    uint8_t  conf1;
    uint8_t  chan;
    uint8_t  conf2;
} LoRaModuleConf;

typedef struct {
	uint16_t	src;
    uint16_t	dst;
    uint16_t	index;
    uint8_t		UTC_Hour;    
    uint8_t		UTC_Min;
	uint8_t		UTC_Sec;
	uint8_t		UTC_Day;
	uint8_t		UTC_Mon;
	uint8_t		UTC_Year;
	
	int16_t		Latitude;
	uint32_t	LatitudeDecimal;
	int16_t		Longitude;
	uint32_t	LongitudeDecimal;
	uint8_t		SatellitesUsed;
	int			MSL_Altitude;
	int			MSL_AltitudeDecimal;
} LoraRx;


typedef struct {
	uint16_t 	past_src;
	uint16_t	src;
    uint16_t	dst;
	uint16_t	dst_to_hop;
    uint16_t	index;
	char 		msg;
	uint8_t		mode;
	int			MSL_Altitude;
	int			MSL_AltitudeDecimal;
} LoraRxUAV;

void LORA_UART_IDLECallback(UART_HandleTypeDef *huart);
uint8_t genByte(void);
void genPayload(void);
void sendFrameLoRa(void);
void LoRa_Init(void);
void send_string(char* s);
void GetLoRaConfig(void);
void MakeLoraFrame(void);
void DecodeLoraFrame(void);
void SaveLoraFrame(void);
void LoRaUAV(void);
#endif