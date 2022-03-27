#ifndef __RC_CONTROL_H
#define __RC_CONTROL_H
#include "sys.h"
#include "rx.h"

/********************************************************************************	 
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 * ATKflight飞控固件
 * 遥控控制代码	
 * 正点原子@ALIENTEK
 * 技术论坛:www.openedv.com
 * 创建日期:2018/5/2
 * 版本：V1.0
 * 版权所有，盗版必究。
 * Copyright(C) 广州市星翼电子科技有限公司 2014-2024
 * All rights reserved
********************************************************************************/

typedef enum 
{
    THROTTLE_LOW = 0,
    THROTTLE_HIGH
} throttleStatus_e;

typedef enum 
{
    NOT_CENTERED = 0,
    CENTERED
} rollPitchStatus_e;

typedef enum 
{
    ROL_LO = (1 << (2 * ROLL)), // 0000 0001   //表示 此通道 值太小
    ROL_CE = (3 << (2 * ROLL)), // 0000 0011   //表示正常
    ROL_HI = (2 << (2 * ROLL)), // 0000 0010   //表示 此通道 值太大

    PIT_LO = (1 << (2 * PITCH)),   // 0000 0100
    PIT_CE = (3 << (2 * PITCH)),   // 0000 1100  //表示正常
    PIT_HI = (2 << (2 * PITCH)),   // 0000 1000

    THR_LO = (1 << (2 * THROTTLE)), // 0001
    THR_CE = (3 << (2 * THROTTLE)), // 0011  //表示正常
    THR_HI = (2 << (2 * THROTTLE)), // 0010
	
	YAW_LO = (1 << (2 * YAW)),
    YAW_CE = (3 << (2 * YAW)), //表示正常
    YAW_HI = (2 << (2 * YAW)),
} stickPositions_e;

typedef enum 
{
	AUX_LO = 0,
    AUX_CE = 1,
    AUX_HI = 2,
} auxPositions_e;

stickPositions_e getRcStickPositions(void);
bool checkStickPosition(stickPositions_e stickPos);

bool areSticksInApModePosition(uint16_t ap_mode);
throttleStatus_e calculateThrottleStatus(void);
rollPitchStatus_e calculateRollPitchCenterStatus(void);
void processRcStickPositions(void);

int32_t getRcStickDeflection(int32_t axis, uint16_t midrc);
void processRcAUXPositions(void);

#endif
