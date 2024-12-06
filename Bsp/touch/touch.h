#ifndef STM32H750XBH6_7_CACHE_TEST_TOUCH_H
#define STM32H750XBH6_7_CACHE_TEST_TOUCH_H
#include "main.h"

#define TS_RST_0 HAL_GPIO_WritePin(TS_RST_GPIO_Port, TS_RST_Pin, GPIO_PIN_RESET) // 根据实际引脚修改
#define TS_RST_1 HAL_GPIO_WritePin(TS_RST_GPIO_Port, TS_RST_Pin, GPIO_PIN_SET)   // 根据实际引脚修改
// FT5206 I2C地址
#define T_S_ADDR 0XBA

#define GT911_READ_XY_REG    0x814E /* 坐标寄存器 */

#define GT911_CLEARBUF_REG   0x814E /* 清除坐标寄存器 */

#define GT911_CONFIG_REG     0x8047 /* 配置参数寄存器 */

#define GT911_MAXXY_REG      0x8048 /*GT911触摸板的分辨率*/

#define GT911_COMMAND_REG    0x8040 /* 实时命令 */

#define GT911_PRODUCT_ID_REG 0x8140 /* 芯片ID */

#define GT911_VENDOR_ID_REG  0x814A /* 当前模组选项信息 */

#define GT911_CONFIG_VERSION_REG   0x8047 /* 配置文件版本号 */

#define GT911_CONFIG_CHECKSUM_REG  0x80FF /* 配置文件校验码 */

#define GT911_FIRMWARE_VERSION_REG 0x8144 /* 固件版本号 */

#define GT_GSTID_REG 	        0X814E  /* GT9147当前检测到的触摸情况 */
#define GT_TP1_REG 		        0X8150  /* 第一个触摸点数据地址 */
#define GT_TP2_REG 		        0X8158	/* 第二个触摸点数据地址 */
#define GT_TP3_REG 		        0X8160  /* 第三个触摸点数据地址 */
#define GT_TP4_REG 		        0X8168  /* 第四个触摸点数据地址  */
#define GT_TP5_REG 		        0X8170	/* 第五个触摸点数据地址   */

typedef struct {
    uint8_t Point_ID[5];
    uint8_t Touch_num;
    uint8_t Press_Status;
    uint16_t Coord_X[5];
    uint16_t Coord_Y[5];
}Touch_Point;



void Int_GPIO_Output();
void Int_GPIO_Input();

void touch_RST();
void touch_test();
void Touch_Init(void);
void Touch_Read_Reg(uint16_t reg_address,uint8_t * pdate,uint16_t size);
void Touch_Write_Reg(uint16_t reg_address,uint8_t * pdate,uint16_t size);
void Touch_ReadMaxXY(uint16_t *X,uint16_t *Y);
void Touch_scan(Touch_Point* Touch_use);
void Touch_SoftRst();

/*
		0x814E R/W Bufferstatus Large_Detect number of touch points
			Bit7: Buffer status，1表示坐标（或按键）已经准备好，主控可以读取；0表示未就绪，数据无效。当主控读取完坐标后，必须通过I2C将此标志（或整个字节）写为0。
			Bit4: HaveKey, 1表示有按键，0表示无按键（已经松键）。
			Bit3~0: Number of touch points, 屏上的坐标点个数

		0x814F R Point1 track id
		0x8150 R Point1Xl 触摸点 1，X 坐标低 8 位
		0x8151 R Point1Xh 触摸点 1，X 坐标高 8 位
		0x8152 R Point1Yl 触摸点 1，Y 坐标低 8 位
		0x8153 R Point1Yh 触摸点 1，Y 坐标高 8 位
		0x8154 R Point1 触摸点 1，触摸面积低 8 位
		0x8155 R Point1 触摸点 1，触摸面积高 8 位
		0x8156 ----

		0x8157 R Point2 track id
		0x8158 R Point2Xl 触摸点 2，X 坐标低 8 位
		0x8159 R Point2Xh 触摸点 2，X 坐标高 8 位
		0x815A R Point2Yl 触摸点 2，Y 坐标低 8 位
		0x815B R Point2Yh 触摸点 2，Y 坐标高 8 位
		0x815C R Point2 触摸点 2，触摸面积低 8 位
		0x815D R Point2 触摸点 2，触摸面积高 8 位
		0x815E ----

		0x815F R Point3 track id
		0x8160 R Point3Xl 触摸点 3，X 坐标低 8 位
		0x8161 R Point3Xh 触摸点 3，X 坐标高 8 位
		0x8162 R Point3Yl 触摸点 3，Y 坐标低 8 位
		0x8163 R Point3Yh 触摸点 3，Y 坐标高 8 位
		0x8164 R Point3 触摸点 3，触摸面积低 8 位
		0x8165 R Point3 触摸点 3，触摸面积高 8 位
		0x8166 ----

		0x8167 R Point4 track id
		0x8168 R Point4Xl 触摸点 4，X 坐标低 8 位
		0x8169 R Point4Xh 触摸点 4，X 坐标高 8 位
		0x816A R Point4Yl 触摸点 4，Y 坐标低 8 位
		0x816B R Point4Yh 触摸点 4，Y 坐标高 8 位
		0x816C R Point4 触摸点 4，触摸面积低 8 位
		0x816D R Point4 触摸点 4，触摸面积高 8 位
		0x816E ----

		0x816F R Point5 track id
		0x8170 R Point5Xl 触摸点 5，X 坐标低 8 位
		0x8171 R Point5Xh 触摸点 5，X 坐标高 8 位
		0x8172 R Point5Yl 触摸点 5，Y 坐标低 8 位
		0x8173 R Point5Yh 触摸点 5，Y 坐标高 8 位
		0x8174 R Point5 触摸点 5，触摸面积低 8 位
		0x8175 R Point5 触摸点 5，触摸面积高 8 位
		0x8176 --

	*/
#endif //STM32H750XBH6_7_CACHE_TEST_TOUCH_H
