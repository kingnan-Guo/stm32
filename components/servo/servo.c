/**
*
*/
#include "stm32f10x.h"
#include "PWM.h"

//舵机初始化
void servo_init(void){
    PWM_Init();
}

/**
 * 舵机 设置角度
 * 180 度 舵机; 可以根据 占空比设置 旋转角度
 * 0 500
 * 180 2500
 *
 * 360  度舵机 ： 根据 传值 更改 转动速度
 * @param Angle 旋转角度
 */
void servo_setAngle(float Angle){
    // 180 度 舵机 ：  ； ；500 偏移量
    PWM_SetCompare2(Angle / 180 * 2000 +  500);
}

