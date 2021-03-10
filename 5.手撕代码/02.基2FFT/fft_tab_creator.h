#ifndef FFT_FFT_TAB_CREATOR_H
#define FFT_FFT_TAB_CREATOR_H


#include <stdio.h>
#include <stdint.h>
#include <math.h>


/* 圆周率 π */
#define FFT_CREATOR_PI 3.1415926535898


/* 生成的表的最大点数 及 对应的幂次 */
#define FFT_CREATOR_MAX_M   14
#define FFT_CREATOR_MAX_N   16384


/* 函数声明 */
uint32_t reversedOrder(uint32_t Number, uint32_t MaxBits);
float sinWnk(uint16_t n, uint16_t k);
float cosWnk(uint16_t n, uint16_t k);
void createFiles(void);


#endif /* FFT_FFT_TAB_CREATOR_H */
