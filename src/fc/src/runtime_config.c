#include "runtime_config.h"
#include "beeper.h"
#include "led.h"
#include "ledstrip.h"
#include "sensors.h"
#include "gyro.h"
#include "accelerometer.h"
#include "compass.h"
#include "barometer.h"

#include "pos_estimator.h"
#include "state_control.h"

/********************************************************************************	 
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 * ATKflight飞控固件
 * 运行监测代码	
 * 正点原子@ALIENTEK
 * 技术论坛:www.openedv.com
 * 创建日期:2018/5/2
 * 版本：V1.0
 * 版权所有，盗版必究。
 * Copyright(C) 广州市星翼电子科技有限公司 2014-2024
 * All rights reserved
********************************************************************************/

uint32_t armingFlags = 0;
uint32_t stateFlags = 0;
uint32_t flightModeFlags = 0;


//使能给定飞行模式
uint32_t enableFlightMode(flightModeFlags_e mask)
{
    uint32_t oldVal = flightModeFlags;

    flightModeFlags |= (mask);
    if (flightModeFlags != oldVal)
        beeperConfirmationBeeps(1);
    return flightModeFlags;
}

//禁能给定飞行模式
uint32_t disableFlightMode(flightModeFlags_e mask)
{
    uint32_t oldVal = flightModeFlags;

    flightModeFlags &= ~(mask);
    if (flightModeFlags != oldVal)
        beeperConfirmationBeeps(1);
    return flightModeFlags;
}

//校准进行中
bool isCalibrating(void)
{
	if (!baroIsCalibrationComplete())  // 校准成功 就是不返回1 
	{
		return true;
	}

	if (!posEstimatorIsCalibrationComplete()) 
	{
		return true;
	}

	if (!accIsCalibrationComplete()) 
	{
		return true;
	}

	if (!gyroIsCalibrationComplete()) 
	{
		return true;
	}

    return false; // 全部校准完成就是返回0
}


// ARMING_DISABLED_NOT_LEVEL                       = (1 << 3), // 如果是1 就是不能解锁  原因是不水平
// ARMING_DISABLED_SENSORS_CALIBRATING             = (1 << 4), // 如果是1 就是不能解锁  原因是传感器没有校准
// ARMING_DISABLED_COMPASS_NOT_CALIBRATED          = (1 << 5), // 如果是1 就是不能解锁  原因是罗盘没有校准 
// ARMING_DISABLED_ACCELEROMETER_NOT_CALIBRATED    = (1 << 6), // 如果是1 就是不能解锁  原因是加速度计没有校准
// ARMING_DISABLED_FLASH_WRITING                   = (1 << 8), // 如果是1 就是不能解锁  原因是flash还在写
//更新解锁标志位状态
//根据各个关键的设备是否正常  来决定是否要清除相应的标志位
void updateArmingStatus(void)
{
    if (ARMING_FLAG(ARMED)) //如果已经解锁了 
	{
        LED0_ON;
    } 
	else 
	{
		//传感器校准中
		static bool calibratingFinishedBeep = false;
		if (isCalibrating())  //
		{
			ENABLE_ARMING_FLAG(ARMING_DISABLED_SENSORS_CALIBRATING); //表示还没有通过校准
			calibratingFinishedBeep = false;
		}
		else //全部校准成功 就是这里
		{
			DISABLE_ARMING_FLAG(ARMING_DISABLED_SENSORS_CALIBRATING); //

			if (!calibratingFinishedBeep) 
			{
				calibratingFinishedBeep = true;
				beeper(BEEPER_RUNTIME_CALIBRATION_DONE); //校准成功
			}
		}

		//检查水平最小角
		if (!STATE(SMALL_ANGLE)) 
		{
			ENABLE_ARMING_FLAG(ARMING_DISABLED_NOT_LEVEL);
		}
		else 
		{
			DISABLE_ARMING_FLAG(ARMING_DISABLED_NOT_LEVEL);
		}

		//检查加速度计是否已经校准完成
		if (!STATE(ACCELEROMETER_CALIBRATED)) 
		{
			ENABLE_ARMING_FLAG(ARMING_DISABLED_ACCELEROMETER_NOT_CALIBRATED);
		}
		else 
		{
			DISABLE_ARMING_FLAG(ARMING_DISABLED_ACCELEROMETER_NOT_CALIBRATED);
		}

		//检查磁力计是否校准
		if (sensorsIsMagPresent() && !STATE(COMPASS_CALIBRATED)) 
		{
			ENABLE_ARMING_FLAG(ARMING_DISABLED_COMPASS_NOT_CALIBRATED);
		}
		else 
		{
			DISABLE_ARMING_FLAG(ARMING_DISABLED_COMPASS_NOT_CALIBRATED);
		}
		
		//检查是否在写Flash
		if (STATE(FLASH_WRITING))
		{
			ENABLE_ARMING_FLAG(ARMING_DISABLED_FLASH_WRITING);
		}
		else
		{
			DISABLE_ARMING_FLAG(ARMING_DISABLED_FLASH_WRITING);
		}
		
		//刷新LED灯状态
        if (!isArmingDisabled()) // 全部都是正常了
		{
            warningLedFlash(); //闪烁表示可以解锁了
        } 
		else 
		{
            warningLedON(); 
        }
		
        warningLedUpdate();
    }
}

//锁定飞控
void mwDisarm(void)
{
    if (ARMING_FLAG(ARMED)) 
	{
        DISABLE_ARMING_FLAG(ARMED); //上锁
        beeper(BEEPER_DISARMING); 
    }
}

//解锁飞控
void mwArm(void)
{
    updateArmingStatus();

    if (!isArmingDisabled())  // 只要有一个是 1 ，就是1 ，所以需要全部都是 0 
	{
        if (ARMING_FLAG(ARMED))  //查询是否解锁 
		{
            return;
        }
		ENABLE_ARMING_FLAG(ARMED); // 真正的解锁
		ENABLE_ARMING_FLAG(WAS_EVER_ARMED); // 之前没有解锁过
		
		stateControlResetYawHolding();//复位航向角锁定值
        beeper(BEEPER_ARMING);
        return;
    }

    if (!ARMING_FLAG(ARMED)) 
	{
        beeperConfirmationBeeps(1);
    }
}

