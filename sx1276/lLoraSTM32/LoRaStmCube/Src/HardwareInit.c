#include "HardwareInit.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "LoraApp.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef  dma1;
DMA_HandleTypeDef  dma2;
ExtIntStatus       extIT;
SPI_HandleTypeDef  hspi;


void HardwareInit(){

    HAL_Init();

/*-------------------- PC13 Configuration begins -----------------------------*/

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 

    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); 

/*------------------PC13 Configuration ends  --------------------------------*/


/*-------------------- LORA GPIO Configuration begins -----------------------*/

    
    GPIO_InitStruct.Pin =  GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.Pin =  GPIO_PIN_15;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 
    M0_LOW();  // PA 0
    M1_LOW();  // PC15
    

/*------------------ LORA GPIO Configuration ends  ---------------------------*/

/*------------------EXTI Configuration begins -------------------------------*/

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) )
        extIT = WAITING;
  else
        extIT = PENDING;
  /* EXTI interrupt init*/
  // HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  // HAL_NVIC_EnableIRQ(EXTI4_IRQn);

/*------------------EXTI Configuration ends -------------------------------*/


/*------------------UART 1 Configuration begins -----------------------------*/

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(&huart1) != HAL_OK)
       asm("bkpt 255");

/*------------------UART 1 Configuration ends -------------------------------*/


/*------------------UART 2 Configuration begins -----------------------------*/

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    

    if(HAL_UART_Init(&huart2) != HAL_OK)
       asm("bkpt 255");
        
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    

/*------------------UART 2 Configuration ends -------------------------------*/


/*------------------UART 3 Configuration begins -----------------------------*/

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    huart3.Instance = USART3;
    huart3.Init.BaudRate = 9600;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(&huart3) != HAL_OK)
       asm("bkpt 255");
     
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);   

/*------------------UART 3 Configuration ends -------------------------------*/




/*------------------ DMA Configuration ends -------------------------------*/

  __HAL_RCC_DMA1_CLK_ENABLE();

 dma1.Instance = DMA1_Channel6;
 dma1.Init.Direction = DMA_PERIPH_TO_MEMORY;
 dma1.Init.PeriphInc = DMA_PINC_DISABLE;
 dma1.Init.MemInc = DMA_MINC_ENABLE;
 dma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
 dma1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
 dma1.Init.Mode = DMA_NORMAL;
 dma1.Init.Priority = DMA_PRIORITY_HIGH;
 
 HAL_DMA_Init(&dma1);
 __HAL_LINKDMA(&huart2, hdmarx, dma1);
// __HAL_DMA_ENABLE_IT(&dma,DMA_IT_TC);
//HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
//HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
//HAL_DMA_Start_IT(&dma, (uint32_t) (&(USART2->DR)), (uint32_t) LoRaRxBuff, (uint32_t)LRBuffSize);

dma2.Instance = DMA1_Channel3;
dma2.Init.Direction = DMA_PERIPH_TO_MEMORY;
dma2.Init.PeriphInc = DMA_PINC_DISABLE;
dma2.Init.MemInc = DMA_MINC_ENABLE;
dma2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
dma2.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
dma2.Init.Mode = DMA_NORMAL;
dma2.Init.Priority = DMA_PRIORITY_HIGH;
 
HAL_DMA_Init(&dma2);
__HAL_LINKDMA(&huart3, hdmarx, dma2);
/*------------------ DMA Configuration ends -------------------------------*/

/*------------------ SPI Configuration begins -------------------------------*/

  __HAL_RCC_SPI1_CLK_ENABLE();
    //  NSS
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //  SCK
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //  MISO
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //  SCK
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //HAL_SPI_DeInit(&hspi);
    hspi.Instance = SPI1;
    hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi.Init.Mode = SPI_MODE_MASTER;
    hspi.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi.Init.Direction = SPI_DIRECTION_2LINES;
    hspi.Init.NSS = SPI_NSS_SOFT;
    hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi.Init.CRCPolynomial = 7;

    HAL_SPI_Init(&hspi);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

/*------------------ SPI Configuration ends -------------------------------*/

}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);  
}


void myprintf(const char *fmt, ...) {
  static char buffer[256];
  __va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  int len = strlen(buffer);
  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, len, -1);
}



