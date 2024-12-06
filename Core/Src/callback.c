//
// Created by augety on 23-10-8.
//
#include "callback.h"
#include <stdio.h>
#include "usart.h"
#include "touch.h"

/*
 * 按键中断 PI4 下降沿中断 触发中断回调函数。
 * */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin==KEY_Pin)
    {
        while(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin)==0);
        LED_R_GPIO_Port->ODR^=LED_R_Pin;
        printf("The key is put\r\n");
    }
    if(GPIO_Pin==GPIO_PIN_12)
    {
//        printf("The touch sreen is triggered\r\n");
//        printf("TS_int:%d\n",HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_12));
//        while(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_12)==0);
    }
}





