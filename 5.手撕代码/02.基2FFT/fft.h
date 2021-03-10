#ifndef _FFT_H
#define _FFT_H


#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#include "fft_tab.h"


/* 注意要点：
 *  (1) 以下是 FFT 快速傅里叶变换及 IFFT 反快速傅里叶变换算法
 *  (2) 抽选方法为 频率抽选，运算结束后无需倒序，直接使用即可
 *  (3) 支持 2^x(2 <= x <= 14) 点 FFT 及 IFFT，即最大 16384 点
 *  (4) FFT 转换后的数组，直流分量 F(0)，谐波分量频带关于 N/2 对称
 *  (5) 支持窗函数，三角窗、汉宁窗、汉明窗、布莱克曼窗
 */


/* 定义圆周率 */
#define FFT_PI 3.1415926535898


/* 定义复数结构体 */
typedef struct
{

    float Real;
    float Image;

} Complex_TypeDef;


/* 复数运算 */
Complex_TypeDef Complex_Mul(Complex_TypeDef com1, Complex_TypeDef com2);
Complex_TypeDef Complex_Add(Complex_TypeDef com1, Complex_TypeDef com2);
Complex_TypeDef Complex_Sub(Complex_TypeDef com1, Complex_TypeDef com2);
Complex_TypeDef Complex_Div(Complex_TypeDef com1, Complex_TypeDef com2);
Complex_TypeDef Complex_NumMul(Complex_TypeDef com, float num);
Complex_TypeDef Complex_NumDiv(Complex_TypeDef com, float num);


/* 傅里叶变换的支持函数 */
void FFT_ClearArray(Complex_TypeDef *array, uint16_t N);
void FFT_AddWindow(Complex_TypeDef *array, uint16_t N, const float *window);
void FFT_DelWindow(Complex_TypeDef *array, uint16_t N, const float *window);
void FFT_GetAmplitude(Complex_TypeDef *array, uint16_t N, float *amplitude, float recover);
void FFT_GetPhase(Complex_TypeDef *array, uint16_t N, float *phase);


/* 傅里叶变换 傅里叶反变换 */
void FFT(Complex_TypeDef *array, uint16_t N);
void IFFT(Complex_TypeDef *array, uint16_t N);


/* 频域微分 */
void FFT_Differ_1th(Complex_TypeDef *array, uint16_t N, float fs, float fa, float fb);
void FFT_Differ_2th(Complex_TypeDef *array, uint16_t N, float fs, float fa, float fb);


/* 频域积分 */
void FFT_Integr_1th(Complex_TypeDef *array, uint16_t N, float fs, float fa, float fb);
void FFT_Integr_2th(Complex_TypeDef *array, uint16_t N, float fs, float fa, float fb);


#endif /* _FFT_H */
