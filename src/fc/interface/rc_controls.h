#ifndef __RC_CONTROL_H
#define __RC_CONTROL_H
#include "sys.h"
#include "rx.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ATKflight�ɿع̼�
 * ң�ؿ��ƴ���	
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2018/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
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
    ROL_LO = (1 << (2 * ROLL)), // 0000 0001   //��ʾ ��ͨ�� ֵ̫С
    ROL_CE = (3 << (2 * ROLL)), // 0000 0011   //��ʾ����
    ROL_HI = (2 << (2 * ROLL)), // 0000 0010   //��ʾ ��ͨ�� ֵ̫��

    PIT_LO = (1 << (2 * PITCH)),   // 0000 0100
    PIT_CE = (3 << (2 * PITCH)),   // 0000 1100  //��ʾ����
    PIT_HI = (2 << (2 * PITCH)),   // 0000 1000

    THR_LO = (1 << (2 * THROTTLE)), // 0001
    THR_CE = (3 << (2 * THROTTLE)), // 0011  //��ʾ����
    THR_HI = (2 << (2 * THROTTLE)), // 0010
	
	YAW_LO = (1 << (2 * YAW)),
    YAW_CE = (3 << (2 * YAW)), //��ʾ����
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
