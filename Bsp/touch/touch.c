#include "touch.h"
#include "user_i2c_gpio.h"


void printf_no(const char *abc);
void touch_RST()
{
    TS_RST_0;
    HAL_Delay(10);
    TS_RST_1;
    HAL_Delay(10);
}
void Touch_Read_Reg(uint16_t reg_address,uint8_t * pdate,uint16_t size)
{
    User_I2C_Mem_Read(T_S_ADDR, reg_address, pdate, size);
//    HAL_I2C_Mem_Read(&hi2c1,T_S_ADDR,reg_address,
//                     I2C_MEMADD_SIZE_16BIT,pdate,size,0XFF);
}

void Touch_Write_Reg(uint16_t reg_address,uint8_t * pdate,uint16_t size)
{
    User_I2C_Mem_Write(T_S_ADDR, reg_address, pdate, size);
//    HAL_I2C_Mem_Write(&hi2c1,T_S_ADDR,reg_address,
//                     I2C_MEMADD_SIZE_16BIT,pdate,size,0XFF);
}
// 初始化屏幕
void Touch_Init(void) {
    uint8_t mode = 0x00;
    uint8_t buffer[8]={0};
    uint16_t X_Max,Y_Max;
    Int_GPIO_Output();
    TS_RST_0; // 复位触摸屏
    GPIOD->ODR&=~GPIO_PIN_12;// 拉低INT
    HAL_Delay(10);
    TS_RST_1; // 释放复位
    HAL_Delay(10);
    Int_GPIO_Input();// INT改为悬浮输入
    /*检测I2C设备地址是否有回应，并在调用"i2c_CheckDevice()"过程中初始化软件I2C的SDA、SCL总线*/
    if(i2c_CheckDevice(T_S_ADDR)==0)
        printf("i2c_CheckDevice:True\n");
    else
    {
        printf("i2c_CheckDevice:False\n");
        return;
    }
    Touch_Read_Reg(GT911_PRODUCT_ID_REG,buffer,6);
    buffer[6]=buffer[4];
    buffer[4]=0;
//    printf("Touch screen Version:%s\r\n",buffer);
//    printf("Default Ver:%#x\r\n",((buffer[5]<<8) | buffer[6]));   /* 打印固件版本 */

    if(buffer[0] != 0x31)
    {
        Touch_Read_Reg(0x804D,buffer,1);
        buffer[0]&= 0x3;     /* 获取中断模式 */
//        printf("buffer[0] != 0x31\r\n");
    }
    else {
        Touch_Read_Reg(0x8056,buffer,1);
        buffer[0]&= 0x3;     /* 获取中断模式 */
    }
//    switch(buffer[0]) {
//        case 0x0:
//            printf("InterruptMode:IntRisingEdge\r\n");
//            break;
//        case 0x1:
//            printf("InterruptMode:IntFallingEdge\r\n");
//            break;
//        case 0x2:
//            printf("InterruptMode:IntLowLevel\r\n");
//            break;
//        case 0x3:
//            printf("InterruptMode:IntHighLevel\r\n");
//            break;
//        default : printf("InterruptMode: Error\r\n");
//            break;
//    }

    Touch_ReadMaxXY(&X_Max,&Y_Max);
//    printf("Touch_ReadMaxXY X:%d Y:%d\r\n",X_Max,Y_Max);
    mode=0;
    Touch_Write_Reg(GT911_CLEARBUF_REG,&mode,1);//清标志
    HAL_Delay(5);
    Touch_Read_Reg(GT_GSTID_REG,&mode, 1);
//    printf("Number of fingers: %#X\n", mode);
}
// 读取触摸数据
void Touch_scan(Touch_Point* Touch_use) {
    uint8_t num_fingers=0;
    uint8_t buffer[9]={0};
    uint8_t temp=0;
    uint16_t coord_X,coord_Y;
    uint8_t i = 0;
    static uint16_t a=0,b=0;

    Touch_Read_Reg(GT_GSTID_REG,&num_fingers, 1);
    Touch_Write_Reg(GT_GSTID_REG,&temp,1);//清标志
    Touch_Write_Reg(GT_GSTID_REG,&temp,1);//清标志
    Touch_Write_Reg(GT_GSTID_REG,&temp,1);//清标志
    Touch_Write_Reg(GT_GSTID_REG,&temp,1);//清标志
    Touch_use->Press_Status=(num_fingers&0X10)>>4;/*检测是否有按键仍在按下*/
    num_fingers&=0XF;/*取出按键数量*/
    if(num_fingers==0)
    {
        b++;
        if(b<=3)
        {
            if(a>0)
            {
                Touch_use->Touch_num=1;
                return;
            }
            else
            {
                b=0;
                Touch_use->Touch_num=num_fingers;
                return;
            }
        }
        else
        {
            a=b=0;
            Touch_use->Touch_num=num_fingers;
            return;
        }

    }
    a++;
    b=0;
    Touch_use->Touch_num=num_fingers;
    Touch_Read_Reg(GT_TP1_REG  + (i * 8), buffer, 4);
    Touch_Read_Reg(GT_TP1_REG  + (i * 8), buffer, 4);
    for ( i = 0; i < num_fingers; i++) {
        Touch_Read_Reg(GT_TP1_REG  + (i * 8), buffer, 4);
        coord_X=((uint16_t )buffer[1] << 8) + buffer[0];
        coord_Y=((uint16_t )buffer[3] << 8) + buffer[2];
        printf("The Coord:X:%d,Y:%d\n",coord_X,coord_Y);
        Touch_use->Coord_X[i]=coord_X;
        Touch_use->Coord_Y[i]=coord_Y;
        Touch_use->Point_ID[i]=i+1;
    }
}
void Touch_ReadMaxXY(uint16_t *X,uint16_t *Y)
{
    uint8_t buf[4];
    Touch_Read_Reg(GT911_MAXXY_REG,buf,4);
    *X = buf[0] + buf[1] * 256;
    *Y = buf[2] + buf[3] * 256;
}
void touch_test()
{
    touch_RST();
    printf("i2c_CheckDevice:%d\n",i2c_CheckDevice(T_S_ADDR));
}
void Touch_SoftRst()
{
    uint8_t temp=0;
    temp=2;
    Touch_Write_Reg(GT911_COMMAND_REG,&temp,1);
    HAL_Delay(100);
    temp=0;
    Touch_Write_Reg(GT911_COMMAND_REG,&temp,1);
}
void Int_GPIO_Output()
{
// 配置PG12为输出模式
    GPIOG->MODER &= ~(GPIO_MODER_MODE12);
    GPIOG->MODER |= (1 << GPIO_MODER_MODE12_Pos);

// 配置PG12为推挽输出
    GPIOG->OTYPER &= ~(GPIO_OTYPER_OT12);

// 配置PG12为高速输出
    GPIOG->OSPEEDR |= (3 << GPIO_OSPEEDR_OSPEED12_Pos);
}
void Int_GPIO_Input()
{
// 配置PG12为输入模式
    GPIOG->MODER &= ~(GPIO_MODER_MODE12);

// 配置PG12为悬浮输入（默认状态）
    GPIOG->PUPDR &= ~(GPIO_PUPDR_PUPD12);
}
void printf_no(const char *abc)
{

}