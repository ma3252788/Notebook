
/************************************************* 以下是 fft.c 的所有源码 *************************************************/


#include "fft.h"


/************************************************* 以下是常用的复数运算函数 *************************************************/


/* 函数功能：复数的乘法
 * 参数说明：无
 * 返回说明：返回复数乘法的结果
 */
Complex_TypeDef Complex_Mul(Complex_TypeDef com1, Complex_TypeDef com2)
{
    Complex_TypeDef temp;
    temp.Real = com1.Real * com2.Real - com1.Image * com2.Image;
    temp.Image = com1.Real * com2.Image + com1.Image * com2.Real;
    return temp;
}


/* 函数功能：复数的加法
 * 参数说明：无
 * 返回说明：返回复数加法的结果
 */
Complex_TypeDef Complex_Add(Complex_TypeDef com1, Complex_TypeDef com2)
{
    Complex_TypeDef temp;
    temp.Real = com1.Real + com2.Real;
    temp.Image = com1.Image + com2.Image;
    return temp;
}


/* 函数功能：复数的减法
 * 参数说明：无
 * 返回说明：返回复数减法的结果
 */
Complex_TypeDef Complex_Sub(Complex_TypeDef com1, Complex_TypeDef com2)
{
    Complex_TypeDef temp;
    temp.Real = com1.Real - com2.Real;
    temp.Image = com1.Image - com2.Image;
    return temp;
}


/* 函数功能：复数的除法
 * 注意要点：(1) 如果除数的模为 0，运算会出错！
 * 参数说明：无
 * 返回说明：返回复数除法的结果
 */
Complex_TypeDef Complex_Div(Complex_TypeDef com1, Complex_TypeDef com2)
{
    Complex_TypeDef temp;
    float num = com2.Real * com2.Real + com2.Image * com2.Image;

    com2.Image = -com2.Image;
    temp = Complex_Mul(com1, com2);
    temp.Real /= num;
    temp.Image /= num;

    return temp;
}


/* 函数功能：复数乘以一个实数
 * 参数说明：无
 * 返回说明：返回复数乘法的结果
 */
Complex_TypeDef Complex_NumMul(Complex_TypeDef com, float num)
{
    Complex_TypeDef temp;
    temp.Real = com.Real * num;
    temp.Image = com.Image * num;
    return temp;
}


/* 函数功能：复数除以一个实数
 * 注意要点：除数不可以为 0！
 * 参数说明：无
 * 返回说明：返回复数除法的结果
 */
Complex_TypeDef Complex_NumDiv(Complex_TypeDef com, float num)
{
    Complex_TypeDef temp;
    temp.Real = com.Real / num;
    temp.Image = com.Image / num;
    return temp;
}


/************************************************* 以下是 FFT 的静态函数 *************************************************/


/* 函数功能：获取 FFT 算法需要的旋转因子
 *          uint16_t n  ：数值 n
 *          uint16_t k  ：数值 k
 *          uint16_t N  ：因子表对应的点数
 * 返回说明：返回复数类型的旋转因子
 */
static Complex_TypeDef FFT_GetWnkForFFT(uint16_t n, uint16_t k, uint16_t N)
{
    Complex_TypeDef temp;
    uint16_t nk = (N * k / n) * (FFT_MAX_SUPPORT_N / N);

    temp.Real = FFT_TAB_CosWnk[nk];
    temp.Image = -FFT_TAB_SinWnk[nk];
    return temp;
}


/* 函数功能：获取 IFFT 算法需要的旋转因子
 *          uint16_t n  ：数值 n
 *          uint16_t k  ：数值 k
 *          uint16_t N  ：因子表对应的点数
 * 返回说明：返回复数类型的旋转因子
 */
static Complex_TypeDef FFT_GetWnkForIFFT(uint16_t n, uint16_t k, uint16_t N)
{
    Complex_TypeDef temp;
    uint16_t nk = (N * k / n) * (FFT_MAX_SUPPORT_N / N);

    temp.Real = FFT_TAB_CosWnk[nk];
    temp.Image = FFT_TAB_SinWnk[nk];
    return temp;
}


/* 函数功能：使用快速二分查找法，获取 FFT 的层数
 * 参数说明：Complex_TypeDef *array  ：复数数组表
 *          uint16_t N              ：FFT 的点数
 * 返回说明：返回 FFT 点数对应的级数
 */
static uint16_t FFT_GetLevel(uint16_t N)
{
    uint16_t l = 0, r = 16, i = 0, v = 0;

    while (l != r)
    {
        i = (l + r) / 2;                                                                // 计算中间点的位置
        v = FFT_TAB_Pow2[i];                                                            // 只需要访问一次，加快运算速度
        if (v > N)                                                                      // 删除右边的区间
            r = i;
        else if (v < N)                                                                 // 删除左边的区间
            l = i;
        else                                                                            // 如果中间点和左右界值相同
            return i;
    }

    return l;
}


/* 函数功能：对顺序放置的数组按照倒序排序
 * 参数说明：Complex_TypeDef *array  ：复数数组表
 *          uint16_t N              ：FFT 的点数
 * 返回说明：无
 */
static void FFT_ReverseOrder(Complex_TypeDef *array, uint16_t N)
{
    uint16_t i, j, t;
    Complex_TypeDef temp;

    t = FFT_MAX_SUPPORT_N / N;                                                          // 计算获取表的数据的间隔
    for (i = 0; i < N; i++)
    {
        j = FFT_TAB_ReOrder[i * t];                                                     // 获取倒序的下标
        if (i < j)                                                                      // 顺序下标小于（大于）倒序下标，交换值
        {
            temp = array[i];                                                            // 暂存原始值
            array[i] = array[j];                                                        // 原始值放倒序值
            array[j] = temp;                                                            // 倒序值放原始值
        }
    }
}


/************************************************* 以下是 FFT 的功能函数 *************************************************/


/* 函数功能：清除数组的数据的实部和虚部，并赋值为 0
 * 参数说明：Complex_TypeDef *array  ：复数数组表
 *          uint16_t N             ：FFT 的点数
 * 返回说明：无
 */
void FFT_ClearArray(Complex_TypeDef *array, uint16_t N)
{
    for (uint16_t i = 0; i < N; i++)
    {
        array[i].Real = 0;
        array[i].Image = 0;
    }
}


/* 函数功能：为采样值添加窗函数
 * 参数说明：Complex_TypeDef *array  ：复数数组表
 *          uint16_t N              ：FFT 的点数
 *          const float *window     ：FFT 的窗函数
 * 返回说明：无
 */
void FFT_AddWindow(Complex_TypeDef *array, uint16_t N, const float *window)
{
    uint16_t i, t, n, s;

    if (window == NULL)                                                                 // 如果没有窗，则使用矩形窗即可
        return;

    t = FFT_MAX_SUPPORT_N / N;                                                          // 计算获取表的数据的间隔
    s = FFT_MAX_SUPPORT_N / 2;                                                          // 窗函数常量表(对称)的长度
    for (i = 0; i < N; i++)
    {
        n = t * i;                                                                      // 顺序获取窗的值
        if (n <= s)
            array[i] = Complex_NumMul(array[i], window[n]);
        else                                                                            // 倒序获取窗的值(关于 s 对称)
            array[i] = Complex_NumMul(array[i], window[FFT_MAX_SUPPORT_N - n]);
    }

}


/* 函数功能：为采样值删除窗函数
 * 注意要点：(1) 若需要删除的窗函数有元素为 0 (如 hanning 窗)，会导致计算出错！
 * 参数说明：Complex_TypeDef *array  ：复数数组表
 *          uint16_t N              ：FFT 的点数
 *          const float *window     ：FFT 的窗函数
 * 返回说明：无
 */
void FFT_DelWindow(Complex_TypeDef *array, uint16_t N, const float *window)
{
    uint16_t i, t, n, s;

    if (window == NULL)                                                                 // 如果没有窗，则使用矩形窗即可
        return;

    t = FFT_MAX_SUPPORT_N / N;                                                          // 计算获取表的数据的间隔
    s = FFT_MAX_SUPPORT_N / 2;                                                          // 窗函数常量表(对称)的长度
    for (i = 0; i < N; i++)
    {
        n = t * i;                                                                      // 顺序获取窗的值
        if (n <= s)
            array[i] = Complex_NumDiv(array[i], window[n]);
        else                                                                            // 倒序获取窗的值(关于 s 对称)
            array[i] = Complex_NumDiv(array[i], window[FFT_MAX_SUPPORT_N - n]);
    }
}


/* 函数功能：对数组计算傅里叶变换的幅度值
 * 注意要点：在获取频率幅度前，需要先对数组执行傅里叶变换 FFT
 * 参数说明：Complex_TypeDef *array      ：复数数组表
 *          Complex_TypeDef* amplitude  ：FFT 的幅值
 *          uint16_t N                  ：FFT 的点数
 *          float recover               ：FFT 的窗函数的恢复系数
 * 返回说明：无
 */
void FFT_GetAmplitude(Complex_TypeDef *array, uint16_t N, float *amplitude, float recover)
{
    float m = recover * 2.0 / N;                                                        // 计算一次常浮点，避免多次运算

    for (uint16_t i = 0; i < N; i++)                                                    // 计算傅里叶变换的幅值
        amplitude[i] = m * sqrt((double) array[i].Real * array[i].Real + array[i].Image * array[i].Image);
    amplitude[0] /= 2.0;                                                                // 直流分量的是真实值的 2 倍
}


/* 函数功能：对数组计算傅里叶变换的相位
 * 注意要点：(1) 两个数组的长度需要保持一致
 *          (2) 若需要获取初始相位，需满足：抽样频率应为信号频率的整数倍，抽样点数应包含整周期
 * 参数说明：Complex_TypeDef *array ：复数数组表
 *          Complex_TypeDef* phase ：FFT 的相位
 *          uint16_t N             ：FFT 的点数
 * 返回说明：无
 */
void FFT_GetPhase(Complex_TypeDef *array, uint16_t N, float *phase)
{
    for (uint16_t i = 0; i < N; i++)
        phase[i] = atan2((double) array[i].Image, array[i].Real);
}


/* 函数功能：对数组执行快速傅里叶变换 FFT 的算法
 * 注意要点：(1) 如果 window == NULL，则为默认的矩形窗
 *          (2) 若需要使用窗函数，尽可能保证 N >> 1
 * 参数说明：Complex_TypeDef *array  ：复数数组表
 *          uint16_t N              ：FFT 的点数
 * 返回说明：无
 */
void FFT(Complex_TypeDef *array, uint16_t N)
{
    uint16_t i, j, k, temp1, temp2, index, M;                                           // 循环数和暂存 幂值
    Complex_TypeDef xt_temp1, xt_temp2;                                                 // 暂存数，减小 CPU 访问数组次数
    Complex_TypeDef w2jp1k, wk_temp;                                                    // 旋转因子

    M = FFT_GetLevel(N);                                                                // 获取层级
    FFT_ReverseOrder(array, N);                                                         // 按照倒序表排序

    for (j = 0; j < M; j++)                                                             // 计算第 j + 1 层级
    {
        temp1 = FFT_TAB_Pow2[j + 1];
        temp2 = FFT_TAB_Pow2[j];
        for (i = 0; i < N / temp1; i++)                                                 // 计算第 i + 1 层级单元
        {
            for (k = 0; k < temp2; k++)                                                 // 计算第 k + 1 计算单元
            {
                w2jp1k = FFT_GetWnkForFFT(temp1, k, N);                                 // 查表法获取旋转因子

                index = temp1 * i + k;                                                  // 计算跳算节点位置
                xt_temp1 = array[index];                                                // 只需要访问一次，加快运算
                xt_temp2 = array[index + temp2];                                        // 只需要访问一次，加快运算
                wk_temp = Complex_Mul(xt_temp2, w2jp1k);                                // 只用计算一次，加快运算
                array[index] = Complex_Add(xt_temp1, wk_temp);
                array[index + temp2] = Complex_Sub(xt_temp1, wk_temp);
            }
        }
    }
}


/* 函数功能：对数组执行反快速傅里叶变换 IFFT 的算法
 * 参数说明：Complex_TypeDef *array  ：复数数组表
 *          uint16_t N              ：IFFT 的点数
 * 返回说明：无
 */
void IFFT(Complex_TypeDef *array, uint16_t N)
{
    uint16_t i, j, k, temp1, temp2, index, M;                                           // 循环数和暂存 幂值
    Complex_TypeDef xt_temp1, xt_temp2;                                                 // 暂存数，减小 CPU 访问数组次数
    Complex_TypeDef w2jp1k, wk_temp;                                                    // 旋转因子

    M = FFT_GetLevel(N);                                                                // 获取层级
    FFT_ReverseOrder(array, N);                                                         // 按照倒序表排序

    for (j = 0; j < M; j++)                                                             // 计算第 j + 1 层级
    {
        temp1 = FFT_TAB_Pow2[j + 1];
        temp2 = FFT_TAB_Pow2[j];
        for (i = 0; i < N / temp1; i++)                                                 // 计算第 i + 1 层级单元
        {
            for (k = 0; k < temp2; k++)                                                 // 计算第 k + 1 计算单元
            {
                w2jp1k = FFT_GetWnkForIFFT(temp1, k, N);                                // 查表法获取旋转因子

                index = temp1 * i + k;                                                  // 计算跳算节点位置
                xt_temp1 = array[index];                                                // 只需要访问一次，加快运算
                xt_temp2 = array[index + temp2];                                        // 只需要访问一次，加快运算
                wk_temp = Complex_Mul(xt_temp2, w2jp1k);                                // 只用计算一次，加快运算
                array[index] = Complex_Add(xt_temp1, wk_temp);
                array[index + temp2] = Complex_Sub(xt_temp1, wk_temp);
            }
        }
    }

    for (i = 0; i < N; i++)                                                             // 实部和虚部都除以 N
        array[i] = Complex_NumDiv(array[i], N);
}


/************************************************* 以下是 频域微分 函数 *************************************************/


/* 函数功能：利用傅里叶变换的性质，计算 一阶频域微分
 * 注意要点：(1) 对时域微分，其频谱为：x'(t) <--> (jw)F(jw)
 *          (2) 由于使用窗函数不能够恢复原始信号，因此均采用矩形窗
 *          (3) 如果采样非整周期，或者采样频率不能将 N 整除，会出现频谱泄露，靠近 0 和 N 的点，波形大幅抖动
 *          (4) 为了消除 (3) 中叙述的问题，可设置上限截止频率和下限截止频率进行带通滤波处理
 * 参数说明：Complex_TypeDef *array  ：复数数组表
 *          uint16_t N              ：FFT 的点数
 *          float fs                ：采样频率
 *          float fa                ：下限截止频率
 *          float fb                ：上限截止频率
 * 调用示例：
 *          >> FFT(arr, N);
 *          >> FFT_Differ_1th(arr, N, fs, fa, fb);
 *          >> IFFT(arr, N);
 * 返回说明：无
 */
void FFT_Differ_1th(Complex_TypeDef *array, uint16_t N, float fs, float fa, float fb)
{
    uint16_t k;
    float dw, w, wa, wb;
    Complex_TypeDef com;

    dw = 2.0 * FFT_PI * fs / N;                                                         // 计算角频率分辨率
    wa = 2.0 * FFT_PI * fa;                                                             // 计算下限角频率
    wb = 2.0 * FFT_PI * fb;                                                             // 计算上限角频率

    for (k = 0; k < N; k++)
    {
        if (k <= N / 2)                                                                 // 正频带，jwX(jw)
        {
            w = dw * k;                                                                 // 计算当前正角频率
            if (w < wa || w > wb)                                                       // 滤除指定正角频率
                w = 0;
        }
        else                                                                            // 负频带，-jwX(jw)
        {
            w = -dw * (N - k);                                                          // 计算当前负角频率
            if (w > -wa || w < -wb)                                                     // 滤除指定负角频率
                w = 0;
        }

        com.Real = 0;
        com.Image = w;
        array[k] = Complex_Mul(array[k], com);
    }
}


/* 函数功能：利用傅里叶变换的性质，计算 二阶频域微分
 * 注意要点：(1) 对时域微分，其频谱为：x''(t) <--> (-w^2)F(jw)
 *          (2) 由于使用窗函数不能够恢复原始信号，因此均采用矩形窗
 *          (3) 如果采样非整周期，或者采样频率不能将 N 整除，会出现频谱泄露，靠近 0 和 N 的点，波形大幅抖动
 *          (4) 为了消除 (3) 中叙述的问题，可设置上限截止频率和下限截止频率进行带通滤波处理
 * 参数说明：Complex_TypeDef *array  ：复数数组表
 *          uint16_t N              ：FFT 的点数
 *          float fs                ：采样频率
 *          float fa                ：下限截止频率
 *          float fb                ：上限截止频率
 * 调用示例：
 *          >> FFT(arr, N);
 *          >> FFT_Differ_2th(arr, N, fs, fa, fb);
 *          >> IFFT(arr, N);
 * 返回说明：无
 */
void FFT_Differ_2th(Complex_TypeDef *array, uint16_t N, float fs, float fa, float fb)
{
    uint16_t k;
    float dw, w, wa, wb;

    dw = 2.0 * FFT_PI * fs / N;                                                         // 计算角频率分辨率
    wa = 2.0 * FFT_PI * fa;                                                             // 计算下限角频率
    wb = 2.0 * FFT_PI * fb;                                                             // 计算上限角频率

    for (k = 0; k < N; k++)
    {
        if (k <= N / 2)                                                                 // 正频带，(-w^2)X(jw)
        {
            w = dw * k;                                                                 // 计算当前正角频率
            if (w < wa || w > wb)                                                       // 滤除指定正角频率
                w = 0;
        }
        else                                                                            // 负频带，(-w^2)X(jw)
        {
            w = -dw * (N - k);                                                          // 计算当前负角频率
            if (w > -wa || w < -wb)                                                     // 滤除指定负角频率
                w = 0;
        }

        array[k] = Complex_NumMul(array[k], -w * w);
    }
}


/************************************************* 以下是 频域积分 函数 *************************************************/




/* 函数功能：利用傅里叶变换的性质，计算 一阶频域积分
 * 注意要点：(1) 对时域微分，其频谱为：∫x(t)dt <--> F(jw)/(jw)
 *          (2) 由于使用窗函数不能够恢复原始信号，因此均采用矩形窗
 *          (3) 频域积分的适用范围是 ∫x(t)dt 在 -∞ 到 +∞ 的积分值为 0！如振动加速度的一次积分(稳态信号)
 *          (4) 如果存在直流成分，则在频域积分过程中，会将其去除！请在频域积分前提前处理(如去除趋势项)
 *          (5) 如果采样非整周期，或者采样频率不能将 N 整除，会出现频谱泄露，积分波形会存在低频振动！
 *          (6) 为了消除 (5) 中叙述的问题，可设置上限截止频率和下限截止频率进行带通滤波处理
 *          (7) 最低下限截止频率 fa > 0，选取合适的截止频率滤除不需要的波形
 * 参数说明：Complex_TypeDef *array  ：复数数组表
 *          uint16_t N              ：FFT 的点数
 *          float fs                ：采样频率
 *          float fa                ：下限截止频率
 *          float fb                ：上限截止频率
 * 调用示例：
 *          >> FFT(arr, N);
 *          >> FFT_Integr_1th(arr, N, fs, fa, fb);
 *          >> IFFT(arr, N);
 * 返回说明：无
 */
void FFT_Integr_1th(Complex_TypeDef *array, uint16_t N, float fs, float fa, float fb)
{
    uint16_t k;
    float dw, w, wa, wb;
    Complex_TypeDef com;

    dw = 2.0 * FFT_PI * fs / N;                                                         // 计算角频率分辨率
    wa = 2.0 * FFT_PI * fa;                                                             // 计算下限角频率
    wb = 2.0 * FFT_PI * fb;                                                             // 计算上限角频率

    for (k = 0; k < N; k++)
    {
        if (k == 0)                                                                     // 直流分量，直接去除
        {
            array[k].Real = 0;
            array[k].Image = 0;
            continue;
        }
        else if (k <= N / 2)                                                            // 正频带，X(jw)/(jw)
        {
            w = dw * k;                                                                 // 计算当前正角频率
            if (w < wa || w > wb)                                                       // 滤除指定正角频率
            {
                array[k].Real = 0;
                array[k].Image = 0;
                continue;
            }
        }
        else                                                                            // 负频带，X(jw)/(jw)
        {
            w = -dw * (N - k);                                                          // 计算当前负角频率
            if (w > -wa || w < -wb)                                                     // 滤除指定负角频率
            {
                array[k].Real = 0;
                array[k].Image = 0;
                continue;
            }
        }

        com.Real = 0;
        com.Image = w;
        array[k] = Complex_Div(array[k], com);
    }
}


/* 函数功能：利用傅里叶变换的性质，计算 二阶频域积分
 * 注意要点：(1) 对时域微分，其频谱为：∫∫x(t)dt <--> F(jw)/(-w^2)
 *          (2) 由于使用窗函数不能够恢复原始信号，因此均采用矩形窗
 *          (3) 频域积分的适用范围是 ∫x(t)dt 在 -∞ 到 +∞ 的积分值为 0！如振动加速度的二次积分(稳态信号)
 *          (4) 如果存在直流成分，则在频域积分过程中，会将其去除！请在频域积分前提前处理(如去除趋势项)
 *          (5) 如果采样非整周期，或者采样频率不能将 N 整除，会出现频谱泄露，积分波形会存在低频振动！
 *          (6) 为了消除 (5) 中叙述的问题，可设置上限截止频率和下限截止频率进行带通滤波处理
 *          (7) 最低下限截止频率 fa > 0，选取合适的截止频率滤除不需要的波形
 * 参数说明：Complex_TypeDef *array  ：复数数组表
 *          uint16_t N              ：FFT 的点数
 *          float fs                ：采样频率
 *          float fa                ：下限截止频率
 *          float fb                ：上限截止频率
 * 调用示例：
 *          >> FFT(arr, N);
 *          >> FFT_Integr_2th(arr, N, fs, fa, fb);
 *          >> IFFT(arr, N);
 * 返回说明：无
 */
void FFT_Integr_2th(Complex_TypeDef *array, uint16_t N, float fs, float fa, float fb)
{
    uint16_t k;
    float dw, w, wa, wb;

    dw = 2.0 * FFT_PI * fs / N;                                                         // 计算角频率分辨率
    wa = 2.0 * FFT_PI * fa;                                                             // 计算下限角频率
    wb = 2.0 * FFT_PI * fb;                                                             // 计算上限角频率

    for (k = 0; k < N; k++)
    {
        if (k == 0)                                                                     // 直流分量，直接去除
        {
            array[k].Real = 0;
            array[k].Image = 0;
            continue;
        }
        else if (k <= N / 2)                                                            // 正频带，X(jw)/(-w^2)
        {
            w = dw * k;                                                                 // 计算当前正角频率
            if (w < wa || w > wb)                                                       // 滤除指定正角频率
            {
                array[k].Real = 0;
                array[k].Image = 0;
                continue;
            }
        }
        else                                                                            // 负频带，X(jw)/(-w^2)
        {
            w = -dw * (N - k);                                                          // 计算当前负角频率
            if (w > -wa || w < -wb)                                                     // 滤除指定负角频率
            {
                array[k].Real = 0;
                array[k].Image = 0;
                continue;
            }
        }

        array[k] = Complex_NumDiv(array[k], -w * w);
    }
}


/************************************************* 以上是 fft.c 的全部内容 *************************************************/
