#ifndef HARDWAREINIT_H_
#define HARDWAREINIT_H_
#include"stm32f1xx.h"

#define LED_PIN                                GPIO_PIN_13
#define LED_GPIO_PORT                          GPIOC



typedef enum {WAITING = 0, PENDING = !WAITING} ExtIntStatus;


void HardwareInit();
void SystemClock_Config(void);
void myprintf(const char *fmt, ...);
#endif