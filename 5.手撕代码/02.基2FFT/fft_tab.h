#ifndef __FFT_TAB_H
#define __FFT_TAB_H


#include <stdint.h>


/* 注意事项：
 *	(1) 基于查表法，将表存储为 const 常量，换取高效的 FFT/IFFT 性能
 *	(2) 包含 倒序表、正弦旋转因子表、余弦旋转因子表
 *	(3) 最大支持 16384 点的 FFT 或者 IFFT 运算
 *	(4) 支持窗函数，默认 Hanning 窗，且需要保证 N >> 1，使其满足 N - 1 ≈ N
 */


/* FFT 最大支持的 幂 和 点数 */
#define FFT_MAX_SUPPORT_M 14
#define FFT_MAX_SUPPORT_N 16384


/* 常量表：2 的幂次 */
extern const uint16_t FFT_TAB_Pow2[16];


/* 幅值相等恢复系数：三角窗、汉宁窗、汉明窗、布莱克曼窗 */
#define FFT_AMP_Bartlett 2.000000
#define FFT_AMP_Hanning 2.000000
#define FFT_AMP_Hamming 1.852000
#define FFT_AMP_Blackman 2.381000


/* 功率相等恢复系数：三角窗、汉宁窗、汉明窗、布莱克曼窗 */
#define FFT_PWR_Bartlett 1.732000
#define FFT_PWR_Hanning 1.633000
#define FFT_PWR_Hamming 1.586000
#define FFT_PWR_Blackman 1.812000


/* 窗函数：三角窗、汉宁窗、汉明窗、布莱克曼窗 */
extern const float FFT_WIN_Bartlett[8193];
extern const float FFT_WIN_Hanning[8193];
extern const float FFT_WIN_Hamming[8193];
extern const float FFT_WIN_Blackman[8193];


/* 常量表：倒序表、正弦因子、余弦因子 */
extern const uint16_t FFT_TAB_ReOrder[16384];
extern const float FFT_TAB_SinWnk[8192];
extern const float FFT_TAB_CosWnk[8192];


#endif /* __FFT_TAB_H */
