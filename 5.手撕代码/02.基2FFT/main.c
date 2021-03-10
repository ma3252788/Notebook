
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <math.h>

#include "fft_tab_creator.h"
#include "fft.h"


/* 定义 π */
#define PI 3.1415926535898


/* 注入原始数据 */
void init_arr(Complex_TypeDef *arr, uint16_t N, float fs)
{
    float t;

    for (int i = 0; i < N; ++i)
    {
        t = i / fs;

        arr[i].Real = 5 + 2 * sin(22.5 * PI * t) + 1.5 * sin(50 * PI * t) + 4 * cos(40 * PI * t);
        arr[i].Image = 0;
    }
}

/* 注入原始数据 + 一阶微分 */
void init_arr_differ_1th_order(Complex_TypeDef *arr1, Complex_TypeDef *arr2, uint16_t N, float fs)
{
    float t;

    for (int i = 0; i < N; ++i)
    {
        t = i / fs;

        arr1[i].Real = 5 + sin(25 * PI * t + PI / 4) / (25 * PI) + 1.5 * sin(50 * PI * t + 0.75 * PI) / (25 * PI) +
                       cos(40 * PI * t) / (20 * PI);
        arr1[i].Image = 0;

        arr2[i].Real = cos(25 * PI * t + PI / 4) + 3 * cos(50 * PI * t + 0.75 * PI) - 2 * sin(40 * PI * t);
        arr2[i].Image = 0;
    }
}

/* 注入原始数据 + 二阶微分 */
void init_arr_differ_2th_order(Complex_TypeDef *arr1, Complex_TypeDef *arr2, uint16_t N, float fs)
{
    float t;

    for (int i = 0; i < N; ++i)
    {
        t = i / fs;

        arr1[i].Real = 5 + sin(25 * PI * t) / (25 * PI) + 1.5 * sin(50 * PI * t) / (25 * PI) +
                       cos(40 * PI * t) / (20 * PI);
        arr1[i].Image = 0;

        arr2[i].Real = -25 * PI * sin(25 * PI * t) - 150 * PI * sin(50 * PI * t) - 80 * PI * cos(40 * PI * t);
        arr2[i].Image = 0;
    }
}


/* 注入原始数据 + 一阶积分 */
void init_arr_integr_1th_order(Complex_TypeDef *arr1, Complex_TypeDef *arr2, uint16_t N, float fs)
{
    float t;

    for (int i = 0; i < N; ++i)
    {
        t = i / fs;

        arr1[i].Real = -25 * PI * sin(25 * PI * t + PI / 4) - 150 * PI * sin(50 * PI * t + 0.75 * PI);
        arr1[i].Image = 0;

        arr2[i].Real = cos(25 * PI * t + PI / 4) + 3 * cos(50 * PI * t + 0.75 * PI);
        arr2[i].Image = 0;
    }
}

/* 注入原始数据 + 二阶积分 */
void init_arr_integr_2th_order(Complex_TypeDef *arr1, Complex_TypeDef *arr2, uint16_t N, float fs)
{
    float t;

    for (int i = 0; i < N; ++i)
    {
        t = i / fs;

        arr1[i].Real = -25 * PI * sin(25 * PI * t + PI / 4) - 150 * PI * sin(50 * PI * t + 0.75 * PI);
        arr1[i].Image = 0;

        arr2[i].Real = sin(25 * PI * t + PI / 4) / (25 * PI) + 3 * sin(50 * PI * t + 0.75 * PI) / (50 * PI);
        arr2[i].Image = 0;
    }
}


/* 测试 fft */
void test_fft_ifft(Complex_TypeDef *arr, uint16_t N, const float *window, float recover, char *fName)
{
    float *amp;
    FILE *fp;

    amp = (float *) malloc(N * sizeof(float));
    fp = fopen(fName, "w+");


    /* 数据写入文件 */
    for (int i = 0; i < N; ++i)
    {
        if (i < N - 1)
            fprintf(fp, "%f,", arr[i].Real);
        else
            fprintf(fp, "%f\n", arr[i].Real);
    }

    /* FFT 变换，获取幅值 */
    FFT_AddWindow(arr, N, window);
    FFT(arr, N);
    FFT_GetAmplitude(arr, N, amp, recover);

    /* 频谱写入文件 */
    for (int i = 0; i < N; ++i)
    {
        if (i < N - 1)
            fprintf(fp, "%f,", amp[i]);
        else
            fprintf(fp, "%f\n", amp[i]);
    }

    /* IFFT 变换 */
    IFFT(arr, N);

    /* 恢复数据写入文件 */
    for (int i = 0; i < N; ++i)
    {
        if (i < N - 1)
            fprintf(fp, "%f,", arr[i].Real);
        else
            fprintf(fp, "%f\n", arr[i].Real);
    }

    /* 关闭文件 */
    fclose(fp);
}


/* 测试 频率微分 */
void test_fft_differ(Complex_TypeDef *arr1, Complex_TypeDef *arr2, uint16_t N,
                     float fs, float fa, float fb, char *fName)
{
    FILE *fp;

    fp = fopen(fName, "w+");

    /* 原始数据写入文件 */
    for (int i = 0; i < N; ++i)
    {
        if (i < N - 1)
            fprintf(fp, "%f,", arr1[i].Real);
        else
            fprintf(fp, "%f\n", arr1[i].Real);
    }

    /* FFT 频域微分 */
    FFT(arr1, N);
    FFT_Differ_2th(arr1, N, fs, fa, fb);
    IFFT(arr1, N);

    /* 频域微分后的结果写入文件 */
    for (int i = 0; i < N; ++i)
    {
        if (i < N - 1)
            fprintf(fp, "%f,", arr1[i].Real);
        else
            fprintf(fp, "%f\n", arr1[i].Real);
    }

    /* 真实微分数据写入文件 */
    for (int i = 0; i < N; ++i)
    {
        if (i < N - 1)
            fprintf(fp, "%f,", arr2[i].Real);
        else
            fprintf(fp, "%f\n", arr2[i].Real);
    }

    /* 关闭文件 */
    fclose(fp);
}


/* 测试 频率积分 */
void test_fft_integr(Complex_TypeDef *arr1, Complex_TypeDef *arr2, uint16_t N,
                     float fs, float fa, float fb, char *fName)
{
    FILE *fp;

    fp = fopen(fName, "w+");

    /* 原始数据写入文件 */
    for (int i = 0; i < N; ++i)
    {
        if (i < N - 1)
            fprintf(fp, "%f,", arr1[i].Real);
        else
            fprintf(fp, "%f\n", arr1[i].Real);
    }

    /* FFT 频域积分 */
    FFT_AddWindow(arr1, N, FFT_WIN_Hamming);
    FFT(arr1, N);
    FFT_Integr_2th(arr1, N, fs, fa, fb);
    IFFT(arr1, N);
    FFT_DelWindow(arr1, N, FFT_WIN_Hamming);

    /* 频域积分后的结果写入文件 */
    for (int i = 0; i < N; ++i)
    {
        if (i < N - 1)
            fprintf(fp, "%f,", arr1[i].Real);
        else
            fprintf(fp, "%f\n", arr1[i].Real);
    }

    /* 真实积分数据写入文件 */
    for (int i = 0; i < N; ++i)
    {
        if (i < N - 1)
            fprintf(fp, "%f,", arr2[i].Real);
        else
            fprintf(fp, "%f\n", arr2[i].Real);
    }

    /* 关闭文件 */
    fclose(fp);
}


int main(void)
{
    Complex_TypeDef *arr1;
    Complex_TypeDef *arr2;
    uint16_t N;
    float fs;


    N = 1024;
    fs = 600;


    arr1 = (Complex_TypeDef *) malloc(N * sizeof(Complex_TypeDef));
    arr2 = (Complex_TypeDef *) malloc(N * sizeof(Complex_TypeDef));


    /* 生成文件 */
    createFiles();


    /* 测试 fft ifft */
    //init_arr(arr1, N, fs);
    //test_fft_ifft(arr1, N, FFT_WIN_Bartlett, FFT_AMP_Bartlett, "../valid/fft_ifft.txt");


    /* 测试 fft 一阶微分 */
    //init_arr_differ_1th_order(arr1, arr2, N, fs);
    //test_fft_differ(arr1, arr2, N, fs, 10, 200, "../valid/fft_differ.txt");


    /* 测试 fft 二阶微分 */
    init_arr_differ_2th_order(arr1, arr2, N, fs);
    test_fft_differ(arr1, arr2, N, fs, 10, 50, "../valid/fft_differ.txt");


    /* 测试 fft 一阶积分 */
    //init_arr_integr_1th_order(arr1, arr2, N, fs);
    //test_fft_integr(arr1, arr2, N, fs, 5, 200, "../valid/fft_integr.txt");


    /* 测试 fft 二阶积分 */
    init_arr_integr_2th_order(arr1, arr2, N, fs);
    test_fft_integr(arr1, arr2, N, fs, 10, 50, "../valid/fft_integr.txt");


    return 0;
}

