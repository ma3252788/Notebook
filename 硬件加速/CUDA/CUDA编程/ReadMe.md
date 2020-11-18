# CUDA编程学习

GPU 加速计算是指同时利用图形处理器 (GPU) 和 CPU，加快科学、分析、工程、消费和企业应用程序的运行速度。GPU 加速器于 2007 年由 NVIDIA 率先推出，现已在世界各地为政府实验室、高校、公司以及中小型企业的高能效数据中心提供支持。GPU 能够使从汽车、手机和平板电脑到无人机和机器人等平台的应用程序加速运行. GPU 加速计算可以提供非凡的应用程序性能，能将应用程序计算密集部分的工作负载转移到 GPU，同时仍由 CPU 运行其余程序代码。从用户的角度来看，应用程序的运行速度明显加快. 理解 GPU 和 CPU 之间区别的一种简单方式是比较它们如何处理任务。CPU 由专为顺序串行处理而优化的几个核心组成，而 GPU 则拥有一个由数以千计的更小、更高效的核心（专为同时处理多重任务而设计）组成的大规模并行计算架构。

CUDA是建立在NVIDIA的CPUs上的一个通用并行计算平台和编程模型，基于CUDA编程可以利用GPUs的并行计算引擎来更加高效地解决比较复杂的计算难题。近年来，GPU最成功的一个应用就是深度学习领域，基于GPU的并行计算已经成为训练深度学习模型的标配。



# 一、编程环境

* CUDA支持C/C++，Python，Fortran等，这里我们选择CUDA C/C++接口
* 编译器以及编译环境
  * CUDA环境：NVIDIA, `CUDA v10.2`， nvcc
  * 操作系统：Ubuntu 18.04.5 LTS
  * 编译器：nvcc (C/C++)
  * 调试器：nvcc-gdb
  * 性能分析：`nsight`，`nvprof`
  * 函数库：`cublas`，`nvblas`，`cusolver`，`cufftw`，`cusparse`，`nvgraph`
* CUDA驱动安装：略



# 二、基础知识

* CUDA编程模型是一个**异构模型**，需要CPU和GPU**共同工作**

* `host`和`device`是两个重要的概念，利用`host`代指CPU及其内存，而用`Device`指代GPU及其内存，CUDA编程中即包含`host`程序也包含`device`程序

* `__global__`标识符表示在GPU上运行，在调用时需要`<<<gride, block>>>`来指定kernel需要执行的线程数量，而定义为`__global__`时的函数的返回值必须是`void`！！

  ```c
  dim3 grid(3, 2)		// 创建 x 方向 3 个，y 方向 2 个 block 构成的网格（grid）
  dim3 block(5, 3)	// 创建 x 方向 5 个，y 方向 3 个 thread 构成的宏块（block）
  kernel_fun<<<grid, block>>>(params...)
  ```

* GPU是异构模型，所以需要区分`host`和`device`上的代码，在CUDA中是通过函数类型限定词开区别`host`和`device`上的函数，主要的三个函数类型
  限定词如下：

  * \_\_global\_\_ ：在`device`上执行，从`host`中调用(一些特定的GPU也可以从`device`上调用)，返回类型必须是void,不支持可变参数参数，不能成为类成员函数。**注意用\_\_global\_\_ 定义 的kernel是异步的**，这意味着`host`**不会等待kernel执行完就执行下一步**。
  * \_\_device\_\_ ：在`device`上执行，**单仅可以从device中调用，不可以和\_\_global\_\_ 同时用**。
  * \_\_host\_\_ ：在`host`上执行，仅可以从`host`上调用，一般省略不写，**不可以和\_\_global\_\_ 同时用，但可和\_\_device_\_**，此时 函数会在`device`和`host`都编译 。

* 一个线程需要两个内置的坐标变量(blockldx,threadldx)来唯一标识，它们都是dim3类型变量，**其中blockldx指明线程所在grid中的位置，而threaldx指明线程所在block中的位置**：

  * threadldx包含三个值：threadldx.x, threadldx.y, threadldx.z
  * blockldx同样包含三个值： blockldx.x, blockldx.y, blockldx.z

  ```c
  /* get thread id： 1D block and 2D grid */
  #define get_tid() (blockDim.x * (blockldx.x + blockldx.y * gridDim.x) + threadldx.x)
  
  /* get block id： 2D grid */
  #define get_bid() (blockldx.x + blockldx.y * gridDim.x)
  
  __global__ void vec_add(double *x, double *y, double *z, int n) 
  {
      int i= get_tid(); // user-defined function
      if(i < n) z[i] = x[i] + y[i];
  }
  ```

* **一个线程块上的线程是放在同一个流式多处理器(SM) 上的**。单个SM的资源有限，这导致线程块中的线程数是有限制的，现代GPUs的线程块可支持的线程数可达1024个。

* 网格和块的线程的逻辑顺序为：`X>Y>Z`，例如：

  ```c
  dim3 grid(3, 2);
  dim3 block(5, 3);
  ```

  * 块：(0, 0),(1, 0), (2, 0), (0, 1), (1,1), (2, 1)
  * 线程：(0, 0), (1, 0), (2, 0), (3, 0), (4, 0), (0, 1),(1,1), (2, 1), (3, 1),(4, 1), (0, 2), (1,2), (2, 2),(3, 2), (4, 2)

<div align="center">
    <img src="https://i.loli.net/2020/11/18/6NpLiAjyJB42UTF.png" width=400></img>
</div>



# 三、代码示例

### 3.1 向量相加

```c
#include <stdio.h>
#include <cuda.h>

typedef float FLOAT;
#define USE_UNIX 1

/* get thread id： 1D block and 2D grid */
#define get_tid() (blockDim.x * (blockIdx.x + blockIdx.y * gridDim.x) + threadIdx.x)

/* get block id： 2D grid */
#define get_bid() (blockIdx.x + blockIdx.y * gridDim.x)

/* warm up, start GPU, optional */
void warmup();

/* get time stamp */
double get_time(void);

/* host, add */
void vec_add_host(FLOAT *x, FLOAT *y, FLOAT *z, int N);

/* device function */
__global__ void vec_add(FLOAT *x, FLOAT *y, FLOAT *z, int N)
{
    /* 1D block */
    int idx = get_tid();

    if (idx < N) z[idx] = z[idx] + y[idx] + x[idx];
}

void vec_add_host(FLOAT *x, FLOAT *y, FLOAT *z, int N)
{
    int i;

    for (i = 0; i < N; i++) z[i] = z[i] + y[i] + x[i];
}

/* a little system programming */
#if USE_UNIX
#include <sys/time.h>
#include <time.h>

double get_time(void)
{
    struct timeval tv;
    double t;

    gettimeofday(&tv, (struct timezone *)0);
    t = tv.tv_sec + (double)tv.tv_usec * 1e-6;

    return t;
}
#else
#include <windows.h>

double get_time(void)
{
    LARGE_INTEGER timer;
    static LARGE_INTEGER fre;
    static int init = 0;
    double t;

    if (init != 1) {
        QueryPerformanceFrequency(&fre);
        init = 1;
    }

    QueryPerformanceCounter(&timer);

    t = timer.QuadPart * 1. / fre.QuadPart;

    return t;
}
#endif

/* warm up GPU */
__global__ void warmup_knl()
{
    int i, j;

    i = 1;
    j = 2;
    i = i + j;
}

void warmup()
{
    int i;

    for (i = 0; i < 8; i++) {
        warmup_knl<<<1, 256>>>();
    }
}

int main()
{
    int N = 20000000;
    int nbytes = N * sizeof(FLOAT);

    /* 1D block */
    int bs = 256;

    /* 2D grid */
    int s = ceil(sqrt((N + bs - 1.) / bs));
    dim3 grid = dim3(s, s);

    FLOAT *dx = NULL, *hx = NULL;
    FLOAT *dy = NULL, *hy = NULL;
    FLOAT *dz = NULL, *hz = NULL;

    int itr = 30;
    int i;
    double th, td;

    /* warm up GPU */
    warmup();

    /* allocate GPU mem */
    cudaMalloc((void **)&dx, nbytes);
    cudaMalloc((void **)&dy, nbytes);
    cudaMalloc((void **)&dz, nbytes);

    if (dx == NULL || dy == NULL || dz == NULL) {
        printf("couldn't allocate GPU memory\n");
        return -1;
    }

    printf("allocated %.2f MB on GPU\n", nbytes / (1024.f * 1024.f));

    /* alllocate CPU mem */
    hx = (FLOAT *) malloc(nbytes);
    hy = (FLOAT *) malloc(nbytes);
    hz = (FLOAT *) malloc(nbytes);

    if (hx == NULL || hy == NULL || hz == NULL) {
        printf("couldn't allocate CPU memory\n");
        return -2;
    }
    printf("allocated %.2f MB on CPU\n", nbytes / (1024.f * 1024.f));

    /* init */
    for (i = 0; i < N; i++) {
        hx[i] = 1;
        hy[i] = 1;
        hz[i] = 1;
    }

    /* copy data to GPU */
    cudaMemcpy(dx, hx, nbytes, cudaMemcpyHostToDevice);
    cudaMemcpy(dy, hy, nbytes, cudaMemcpyHostToDevice);
    cudaMemcpy(dz, hz, nbytes, cudaMemcpyHostToDevice);

    /* warm up */
    warmup();

    /* call GPU */
    cudaThreadSynchronize();
    td = get_time();
    
    for (i = 0; i < itr; i++) vec_add<<<grid, bs>>>(dx, dy, dz, N);

    cudaThreadSynchronize();
    td = get_time() - td;

    /* CPU */
    th = get_time();
    for (i = 0; i < itr; i++) vec_add_host(hx, hy, hz, N);
    th = get_time() - th;

    printf("GPU time： %e, CPU time： %e, speedup： %g\n", td, th, th / td);

    cudaFree(dx);
    cudaFree(dy);
    cudaFree(dz);

    free(hx);
    free(hy);
    free(hz);

    return 0;
}
```





# 四、参考链接

* CUDA编程讲解8小时掌握 | [[B站]](https：//www.bilibili.com/video/BV11K411N7Wm?from=search&seid=695359217594164921)[[Github]](https：//github.com/huiscliu/Tutorials)