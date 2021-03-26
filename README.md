# Notebook

本仓库是本人基于 Github 和 Typora 创建的笔记库。主要记录了一些**软硬开发经验**、**论文总结**、**开发笔记**和**部分手撕的代码**，方便后续查阅和复习

> 按时挖坑，不时更新...



## TODO

### 1、环境配置

* [x] GRUB 双硬盘双系统引导 ：HHD（Windows 10 Pro）, SSD（Ubuntu 18.04.5 LTS）
* [x] CUDA 升降级：Ubuntu 18.04.5 LTS

### 2、论文笔记

* [x] 旋转目标检测：遥感方向，截止2020年10月（总结了部分方法）
* [x] 语义分割网络
  * [x] Deeplab系列：Deeplab, Deeplab V2, Deeplab V3, Deeplab V3+
* [ ] 图像理解
  * [ ]  如何在神经网络中表征局部-整体层次结构（Hinton）
  * [ ] 关于频域学习

### 3、硬件加速

- [ ] CUDA
- [x] Movidius (VPU)
  - [x] 1.OpenVINO在Ubuntu的安装及配置
  - [x] 2.编译 Tensorflow 模型并部署执行
  - [x] 3.OpenVINO's Inference Engine
  - [x] 4.编译模型发现的问题及Bug

### 4、讲义资料

- [x] C++ 讲义资料（itheima）：基础入门、核心编程、提高编程

### 5、手撕代码

- [x] 1.KMP 算法：一种改进的字符串匹配算法，尽量减少模式串与主串的匹配次数以达到快速匹配的目的。
- [x] 2.基2 FFT 算法：采用查找表实现的基2 FFT算法，32位 float 精度；查找表保存在 FLASH 区域，适用于大容量的嵌入式设备。