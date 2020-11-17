# Pytorch基于CUDA的底层加速

有时候科研会苦于Pytorch上没有给出想要的轮子，在网上搜寻了一番发现Pytorch有个拓展模块叫C++ extension，即**借助Pytorch给的接口，以C++的形式去实现自己想要的功能，甚至可以让你自己编写基于CUDA编程的操作**。现在有挺多优质的项目在用着拓展模块，主要原因是自己造的轮子更适用。本次本人需要的是基于CUDA加速，Pytorch调用的DCT快速变换，苦于这个南瓜不知道从哪里开始啃，所以不妨先看看Pytorch是如何实现卷积，然后依葫芦画瓢岂不美哉？



## 一、环境依赖

### 1.1 运行环境

- Ubuntu 18.04.5 LTS (8G RAM)
- Pytorch 1.7.0+
- CUDA 10.2+
- NVIDIA RTX2070 Super (8G)

### 1.2 安装环境

```shell
# Create your conda environment
conda create -n cuda python=3.7

# Activate conda environment and upgrade pip
conda/source activate cuda
python -m pip install --upgeade pip

# Install dependencies
pip install torch==1.7.0 torchvision==0.8.0 [-i https://pypi.douban.com/simpe]
```



## 二、注意事项

### 2.1 Pytoch机制简介

* Pytorch内部有三大块：`C10`、`ATen`、`Torch`，我们主要关注后面两块([点我跳转](https://link.zhihu.com/?target=https%3A//github.com/Pytorch/Pytorch/tree/master/aten/src))。Torch是一个基于C实现的开源项目，在Pytorch中分成以下几个部分：
  - TH = TorcH
  - THC = TorcH Cuda
  - THCUNN = TorcH CUda Neural Network
  - THNN = TorcH Neural Network
* 可以看出，每个模块都有`CPU`和`GPU`版，对于神经网络(nn)还有特定的模块(`THCUNN`，`THNN`)，Pytorch实现的卷积在**generic**下的[SpatialConvolutionMM.cu](https://link.zhihu.com/?target=https%3A//github.com/Pytorch/Pytorch/blob/master/aten/src/THCUNN/generic/SpatialConvolutionMM.cu)，我们关注其中三个方法：
  - `void THNN_(SpatialConvolutionMM_updateOutput)` 前向传输
  - `void THNN_(SpatialConvolutionMM_updateGradInput)` 获得对输入的梯度
  - `void THNN_(SpatialConvolutionMM_accGradParameters) `获得对参数的更新梯度
* 只要我们搞懂了这三个函数，我们也能借助Pytorch的接口很轻易地实现卷积！

### 2.2 



## 三、源码实现

### 3.1 卷积的实现



### 3.2 DCT变换的实现



## 四、参考资料

### 4.1 相关链接

* Pytorch源码小记-卷积 | [知乎：霖霖霖](https://zhuanlan.zhihu.com/p/83517817)
* Pytoch扩展优质模块：Pytorch-Encoding | [Github: zhanghang1989](https://link.zhihu.com/?target=https%3A//github.com/zhanghang1989/Pytorch-Encoding)

