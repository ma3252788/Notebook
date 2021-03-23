# 基二 FFT 算法

> FFT是一种DFT的高效算法，称为[快速傅立叶变换](https://baike.baidu.com/item/快速傅立叶变换/5151122)（fast Fourier transform）。[傅里叶变换](https://baike.baidu.com/item/傅里叶变换/7119029)是时域一频域变换分析中最基本的方法之一。在数字处理领域应用的[离散傅里叶变换](https://baike.baidu.com/item/离散傅里叶变换/6379901)(DFT：Discrete Fourier Transform)是许多数字信号处理方法的基础
>
> ——来自：百度百科

## 一、运行环境

* 编译环境：Ubuntu18.04 / Clion / CMake
* 编程语言：C

##  二、代码实现

### 3.1 查找表的生成

* 对于一些嵌入式设备，内存和计算资源都是有限的。为了加快 FFT 的处理速度，该  FFT 算法采用**牺牲空间换取执行效率**的方法：将表存储为 const 常量，换取高效的 FFT/IFFT 性能。

* `fft_tab.c`和`fft_tab.h`是查找表对应的源文件和头文件，由`fft_tab_creator.c`和`fft_tab_creator.h`生成。在该项目中，直接运行`main.cpp`则可以生成查找表。只需要指定以下两个参数就可以生成对应的 FFT 常量表。比如这里指定的 **M = 14，N = 2^14 = 16384**，则生成的 FFT 常量表最大支持 **16384** 点的傅里叶变换。如果需要 **32768** 点的傅里叶变换，只需要改为`FFT_CREATOR_MAX_M=15`，以及`FFT_CREATOR_MAX_N=32768`。

  ```c
  /* 生成的表的最大点数 及 对应的幂次 */
  #define FFT_CREATOR_MAX_M   14
  #define FFT_CREATOR_MAX_N   16384
  ```

* `fft_tab.h`包含以下常量表：

  * 倒序表、正弦旋转因子表、余弦旋转因子表
  * 窗函数表：三角窗、汉宁窗、汉明窗、布莱特曼窗，默认 Hanning 窗
  * 2的幂次表：`1,2,4,8,16,32,64,128,256...`

### 3.2 快速傅里叶变换

* 支持傅里叶变换和傅里叶反变换。调用方式请见[[fft.h]](./fft.h)

  ```c
  /* 傅里叶变换 傅里叶反变换 */
  void FFT(Complex_TypeDef *array, uint16_t N);
  void IFFT(Complex_TypeDef *array, uint16_t N);
  ```

* 注意：其中 N 必须是 2 的幂次

### 3.3 频域的积分和微分

* 支持一阶频域积分、二阶频域积分，一阶频域微分、二阶频域微分。调用方式请见[[fft.h]](./fft.h)

  ```c
  /* 频域积分 */
  void FFT_Integr_1th(Complex_TypeDef *array, uint16_t N, float fs, float fa, float fb);
  void FFT_Integr_2th(Complex_TypeDef *array, uint16_t N, float fs, float fa, float fb);
  
  /* 频域微分 */
  void FFT_Differ_1th(Complex_TypeDef *array, uint16_t N, float fs, float fa, float fb);
  void FFT_Differ_2th(Complex_TypeDef *array, uint16_t N, float fs, float fa, float fb);
  ```

* 注意：其中 N 必须是 2 的幂次

## 三、使用方法

* 在项目中，直接包含`fft.c`，`fft.h`，`fft_tab.c`，`fft_tab.h`即可

