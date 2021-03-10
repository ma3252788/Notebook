
#include "fft_tab_creator.h"


/* 函数功能：计算一个正整数，将二进制码倒序后的数值
 * 参数说明：uint32_t Number ：需要计算的数值
 *          uint32_t MaxBits：最大位数，比如对于 1024，MaxBits = 10
 * 返回说明：返回得到的倒序数值
 */
uint32_t reversedOrder(uint32_t Number, uint32_t MaxBits)
{
    uint32_t Temp = 0;                                                                  // 临时数值为 0
    for (uint32_t i = 0; i < MaxBits; i++)                                              // 循环遍历每一位
    {
        if (i < MaxBits / 2)                                                            // 左移
            Temp |= (Number & ((uint32_t) 1 << i)) << (MaxBits - 2 * i - 1);            // 按位拼接
        else                                                                            // 右移
            Temp |= (Number & ((uint32_t) 1 << i)) >> (2 * i - MaxBits + 1);            // 按位拼接

    }
    return Temp;
}


/* 函数功能：计算 Sin 旋转因子
 * 参数说明：uint16_t n：数值 n
 *          uint16_t k：数值 k
 * 返回说明：返回得到 Sin 旋转因子
 */
float sinWnk(uint16_t n, uint16_t k)
{
    return sin(2 * FFT_CREATOR_PI * (float) k / (float) n);
}


/* 函数功能：计算三角窗
 * 注意要点：(1) 需保证 N >> 1
 * 参数说明：uint16_t n：数值 n
 *          uint16_t N：数值 N
 * 返回说明：返回得到三角窗函数
 */
float bartlett(uint16_t n, uint16_t N)
{
    if (n < N / 2)
        return (2.0 * n) / N;
    else
        return 2.0 - (2.0 * n) / N;
}


/* 函数功能：计算汉宁窗
 * 注意要点：(1) 需保证 N >> 1
 * 参数说明：uint16_t n：数值 n
 *          uint16_t N：数值 N
 * 返回说明：返回得到三角窗函数
 */
float hanning(uint16_t n, uint16_t N)
{
    return 0.5 * (1 - cos(2 * FFT_CREATOR_PI * n / N));
}


/* 函数功能：计算汉宁窗
 * 注意要点：(1) 需保证 N >> 1
 * 参数说明：uint16_t n：数值 n
 *          uint16_t N：数值 N
 * 返回说明：返回得到三角窗函数
 */
float hamming(uint16_t n, uint16_t N)
{
    return 0.54 - 0.46 * cos(2.0 * FFT_CREATOR_PI * n / N);
}


/* 函数功能：计算布莱克曼窗
 * 注意要点：(1) 需保证 N >> 1
 * 参数说明：uint16_t n：数值 n
 *          uint16_t N：数值 N
 * 返回说明：返回得到三角窗函数
 */
float blackman(uint16_t n, uint16_t N)
{
    return 0.42 - 0.5 * cos(2.0 * FFT_CREATOR_PI * n / N) + 0.08 * cos(4.0 * FFT_CREATOR_PI * n / N);
}


/* 函数功能：计算 Cos 旋转因子
 * 参数说明：uint16_t n：数值 n
 *          uint16_t k：数值 k
 * 返回说明：返回得到 Cos 旋转因子
 */
float cosWnk(uint16_t n, uint16_t k)
{
    return cos(2 * FFT_CREATOR_PI * (float) k / (float) n);
}


/* 函数功能：生成 fft_tab.c 和 fft_tab.h
 * 参数说明：无
 * 返回说明：无
 */
void createFiles(void)
{
    FILE *fp = NULL;

    /* 写入 fft_tab.c */

    fp = fopen("../fft_tab.c", "w+");


    fprintf(fp, "#include \"fft_tab.h\"\n");


    fprintf(fp, "\n\n");
    fprintf(fp, "/* FFT 2 的幂次表 */\n");
    fprintf(fp, "const uint16_t FFT_TAB_Pow2[16] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};\n");


    fprintf(fp, "\n\n");
    fprintf(fp, "/* FFT 三角窗 %d / 2 + 1 点 */\n", FFT_CREATOR_MAX_N);
    fprintf(fp, "const float FFT_WIN_Bartlett[%d] = \n", FFT_CREATOR_MAX_N / 2 + 1);
    fprintf(fp, "{");
    for (uint32_t i = 0; i <= FFT_CREATOR_MAX_N / 2; i++)
    {
        if (i == FFT_CREATOR_MAX_N / 2)
            fprintf(fp, "%4.6f", bartlett(i, FFT_CREATOR_MAX_N));
        else if ((i + 1) % 12 == 0 && i != 0)
            fprintf(fp, "%4.6f,\r", bartlett(i, FFT_CREATOR_MAX_N));
        else
            fprintf(fp, "%4.6f,", bartlett(i, FFT_CREATOR_MAX_N));
    }
    fprintf(fp, "};\n");


    fprintf(fp, "\n\n");
    fprintf(fp, "/* FFT 汉宁窗 %d / 2 + 1 点 */\n", FFT_CREATOR_MAX_N);
    fprintf(fp, "const float FFT_WIN_Hanning[%d] = \n", FFT_CREATOR_MAX_N / 2 + 1);
    fprintf(fp, "{");
    for (uint32_t i = 0; i <= FFT_CREATOR_MAX_N / 2; i++)
    {
        if (i == FFT_CREATOR_MAX_N / 2)
            fprintf(fp, "%4.6f", hanning(i, FFT_CREATOR_MAX_N));
        else if ((i + 1) % 12 == 0 && i != 0)
            fprintf(fp, "%4.6f,\r", hanning(i, FFT_CREATOR_MAX_N));
        else
            fprintf(fp, "%4.6f,", hanning(i, FFT_CREATOR_MAX_N));
    }
    fprintf(fp, "};\n");


    fprintf(fp, "\n\n");
    fprintf(fp, "/* FFT 汉明窗 %d / 2 + 1 点 */\n", FFT_CREATOR_MAX_N);
    fprintf(fp, "const float FFT_WIN_Hamming[%d] = \n", FFT_CREATOR_MAX_N / 2 + 1);
    fprintf(fp, "{");
    for (uint32_t i = 0; i <= FFT_CREATOR_MAX_N / 2; i++)
    {
        if (i == FFT_CREATOR_MAX_N / 2)
            fprintf(fp, "%4.6f", hamming(i, FFT_CREATOR_MAX_N));
        else if ((i + 1) % 12 == 0 && i != 0)
            fprintf(fp, "%4.6f,\r", hamming(i, FFT_CREATOR_MAX_N));
        else
            fprintf(fp, "%4.6f,", hamming(i, FFT_CREATOR_MAX_N));
    }
    fprintf(fp, "};\n");


    fprintf(fp, "\n\n");
    fprintf(fp, "/* FFT 布莱克曼窗 %d / 2 + 1 点 */\n", FFT_CREATOR_MAX_N);
    fprintf(fp, "const float FFT_WIN_Blackman[%d] = \n", FFT_CREATOR_MAX_N / 2 + 1);
    fprintf(fp, "{");
    for (uint32_t i = 0; i <= FFT_CREATOR_MAX_N / 2; i++)
    {
        if (i == FFT_CREATOR_MAX_N / 2)
            fprintf(fp, "%4.6f", blackman(i, FFT_CREATOR_MAX_N));
        else if ((i + 1) % 12 == 0 && i != 0)
            fprintf(fp, "%4.6f,\r", blackman(i, FFT_CREATOR_MAX_N));
        else
            fprintf(fp, "%4.6f,", blackman(i, FFT_CREATOR_MAX_N));
    }
    fprintf(fp, "};\n");


    fprintf(fp, "\n\n");
    fprintf(fp, "/* FFT 倒序表最大 %d 点 */\n", FFT_CREATOR_MAX_N);
    fprintf(fp, "const uint16_t FFT_TAB_ReOrder[%d] = \n", FFT_CREATOR_MAX_N);
    fprintf(fp, "{");
    for (uint32_t i = 0; i < FFT_CREATOR_MAX_N; i++)
    {
        if (i == FFT_CREATOR_MAX_N - 1)
            fprintf(fp, "%d", reversedOrder(i, FFT_CREATOR_MAX_M));
        else if ((i + 1) % 22 == 0 && i != 0)
            fprintf(fp, "%d,\r", reversedOrder(i, FFT_CREATOR_MAX_M));
        else
            fprintf(fp, "%d,", reversedOrder(i, FFT_CREATOR_MAX_M));
    }
    fprintf(fp, "};\n");


    fprintf(fp, "\n\n");
    fprintf(fp, "/* FFT 正弦旋转因子最大 %d / 2 点 */\n", FFT_CREATOR_MAX_N);
    fprintf(fp, "const float FFT_TAB_SinWnk[%d] = \n", FFT_CREATOR_MAX_N / 2);
    fprintf(fp, "{");
    for (uint32_t i = 0; i <= FFT_CREATOR_MAX_N / 2; i++)
    {
        if (i == FFT_CREATOR_MAX_N / 2 - 1)
            fprintf(fp, "%4.6f", sinWnk(FFT_CREATOR_MAX_N, i));
        else if ((i + 1) % 12 == 0 && i != 0)
            fprintf(fp, "%4.6f,\r", sinWnk(FFT_CREATOR_MAX_N, i));
        else
            fprintf(fp, "%4.6f,", sinWnk(FFT_CREATOR_MAX_N, i));
    }
    fprintf(fp, "};\n");


    fprintf(fp, "\n\n");
    fprintf(fp, "/* FFT 余弦旋转因子最大 %d / 2 点 */\n", FFT_CREATOR_MAX_N);
    fprintf(fp, "const float FFT_TAB_CosWnk[%d] = \n", FFT_CREATOR_MAX_N / 2);
    fprintf(fp, "{");
    for (uint32_t i = 0; i < FFT_CREATOR_MAX_N / 2; i++)
    {
        if (i == FFT_CREATOR_MAX_N / 2 - 1)
            fprintf(fp, "%4.6f", cosWnk(FFT_CREATOR_MAX_N, i));
        else if ((i + 1) % 12 == 0 && i != 0)
            fprintf(fp, "%4.6f,\r", cosWnk(FFT_CREATOR_MAX_N, i));
        else
            fprintf(fp, "%4.6f,", cosWnk(FFT_CREATOR_MAX_N, i));
    }
    fprintf(fp, "};\n");

    fclose(fp);

    /* 写入 fft_tab.h */

    fp = fopen("../fft_tab.h", "w+");


    fprintf(fp, "#ifndef __FFT_TAB_H\n");
    fprintf(fp, "#define __FFT_TAB_H\n");

    fprintf(fp, "\n\n");
    fprintf(fp, "#include <stdint.h>\n");

    fprintf(fp, "\n\n");
    fprintf(fp, "/* 注意事项：\n");
    fprintf(fp, " *\t(1) 基于查表法，将表存储为 const 常量，换取高效的 FFT/IFFT 性能\n");
    fprintf(fp, " *\t(2) 包含 倒序表、正弦旋转因子表、余弦旋转因子表\n");
    fprintf(fp, " *\t(3) 最大支持 %d 点的 FFT 或者 IFFT 运算\n", FFT_CREATOR_MAX_N);
    fprintf(fp, " *\t(4) 支持窗函数，默认 Hanning 窗，且需要保证 N >> 1，使其满足 N - 1 ≈ N\n");
    fprintf(fp, " */\n");


    fprintf(fp, "\n\n");
    fprintf(fp, "/* FFT 最大支持的 幂 和 点数 */\n");
    fprintf(fp, "#define FFT_MAX_SUPPORT_M %d\n", FFT_CREATOR_MAX_M);
    fprintf(fp, "#define FFT_MAX_SUPPORT_N %d\n", FFT_CREATOR_MAX_N);


//    fprintf(fp, "\n\n");
//    fprintf(fp, "/* FFT 的点数枚举类型 */\n");
//    fprintf(fp, "typedef enum\n");
//    fprintf(fp, "{\n");
//    for (uint32_t i = 3; i <= FFT_CREATOR_MAX_M; i++)
//        fprintf(fp, "\tFFT_N_%d = %d,\n", ((int) pow(2, i)), ((int) pow(2, i)));
//    fprintf(fp, "\n");
//    fprintf(fp, "} FFT_N;\n");


    fprintf(fp, "\n\n");
    fprintf(fp, "/* 常量表：2 的幂次 */\n");
    fprintf(fp, "extern const uint16_t FFT_TAB_Pow2[16];\n");


    fprintf(fp, "\n\n");
    fprintf(fp, "/* 幅值相等恢复系数：三角窗、汉宁窗、汉明窗、布莱克曼窗 */\n");
    fprintf(fp, "#define FFT_AMP_Bartlett %4.6f\n", 2.0);
    fprintf(fp, "#define FFT_AMP_Hanning %4.6f\n", 2.0);
    fprintf(fp, "#define FFT_AMP_Hamming %4.6f\n", 1.852);
    fprintf(fp, "#define FFT_AMP_Blackman %4.6f\n", 2.381);


    fprintf(fp, "\n\n");
    fprintf(fp, "/* 功率相等恢复系数：三角窗、汉宁窗、汉明窗、布莱克曼窗 */\n");
    fprintf(fp, "#define FFT_PWR_Bartlett %4.6f\n", 1.732);
    fprintf(fp, "#define FFT_PWR_Hanning %4.6f\n", 1.633);
    fprintf(fp, "#define FFT_PWR_Hamming %4.6f\n", 1.586);
    fprintf(fp, "#define FFT_PWR_Blackman %4.6f\n", 1.812);


    fprintf(fp, "\n\n");
    fprintf(fp, "/* 窗函数：三角窗、汉宁窗、汉明窗、布莱克曼窗 */\n");
    fprintf(fp, "extern const float FFT_WIN_Bartlett[%d];\n", FFT_CREATOR_MAX_N / 2 + 1);
    fprintf(fp, "extern const float FFT_WIN_Hanning[%d];\n", FFT_CREATOR_MAX_N / 2 + 1);
    fprintf(fp, "extern const float FFT_WIN_Hamming[%d];\n", FFT_CREATOR_MAX_N / 2 + 1);
    fprintf(fp, "extern const float FFT_WIN_Blackman[%d];\n", FFT_CREATOR_MAX_N / 2 + 1);


    fprintf(fp, "\n\n");
    fprintf(fp, "/* 常量表：倒序表、正弦因子、余弦因子 */\n");
    fprintf(fp, "extern const uint16_t FFT_TAB_ReOrder[%d];\n", FFT_CREATOR_MAX_N);
    fprintf(fp, "extern const float FFT_TAB_SinWnk[%d];\n", FFT_CREATOR_MAX_N / 2);
    fprintf(fp, "extern const float FFT_TAB_CosWnk[%d];\n", FFT_CREATOR_MAX_N / 2);
    fprintf(fp, "\n\n");


    fprintf(fp, "#endif /* __FFT_TAB_H */\n");

    fclose(fp);
}