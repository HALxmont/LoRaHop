
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "LoraApp.h"
#include "HardwareInit.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern ExtIntStatus extIT;
//extern char LoRaRxBuff[LRBuffSize];
extern DMA_HandleTypeDef dma1;
extern DMA_HandleTypeDef dma2;



void EXTI4_IRQHandler(void)   // <----- The ISR Function We're Looking For!
{
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != 0x00u)
  {

      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4); // Clears The Interrupt Flag
  }
}


void DMA1_Channel6_IRQHandler(void)
{
  
  HAL_DMA_IRQHandler(&dma1); 
    
  //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
  

}

void USART2_IRQHandler(void)
{
    /* USER CODE BEGIN USART1_IRQn 0 */
    /* USER CODE END USART1_IRQn 0 */
    
    HAL_UART_IRQHandler(&huart2);
    
    /* USER CODE BEGIN USART1_IRQn 1 */
    USER_UART_IRQHandler(&huart2);                                //New function added to handle idle serial interrupts
    /* USER CODE END USART1_IRQn 1 */
}

void USART3_IRQHandler(void)
{
    /* USER CODE BEGIN USART1_IRQn 0 */
    /* USER CODE END USART1_IRQn 0 */
    
    HAL_UART_IRQHandler(&huart3);
    
    /* USER CODE BEGIN USART1_IRQn 1 */
    USER_UART_IRQHandler(&huart3);                                //New function added to handle idle serial interrupts
    /* USER CODE END USART1_IRQn 1 */
}


void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
  if(USART2 == huart->Instance)                                   
    {
    if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   
      {
          __HAL_UART_CLEAR_IDLEFLAG(huart);                     
           
           GPS_UART_IDLECallback(huart);                          
      }
    }
  if(USART3 == huart->Instance)                                   
    {
      if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   
        {
          __HAL_UART_CLEAR_IDLEFLAG(huart);                     
           
          LORA_UART_IDLECallback(huart);                          
        }
    }
}





void SysTick_Handler(void)
{
  HAL_IncTick();
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1) {}
}


void MemManage_Handler(void)
{
  while (1) {}
}

void BusFault_Handler(void)
{
  while (1) {}
}

void UsageFault_Handler(void)
{
  while (1) {}
}

void SVC_Handler(void)
{
}


void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}


HAL_DMA1_IRQHandler(DMA_HandleTypeDef *hdma){

 
  if(__HAL_DMA_GET_TC_FLAG_INDEX(hdma) == DMA_FLAG_TC6){

        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      __HAL_DMA_CLEAR_FLAG(hdma, DMA_FLAG_TC6);
  }


} 