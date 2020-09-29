# 旋转目标检测

**2020年火箭军AI大赛：大尺度遥感图片的旋转目标检测及分类**



## 一、简介及参考

目前目标检测的工作集中在三个方面：**水平区域目标检测(Horizontal region object detection)，任意方向目标检测(Arbitrary-oriented object detection)，方位信息分类(Classification for orientation information)**。**水平区域目标检测**即为经典的目标检测方法，旨在利用水平边界框检测图像的常规对象，如`FasterRCNN`，`YOLO`，`SSD`是常见的**Anchor-based**的方法，而`CenterNet`，`CornerNet`则是**Anchor-free**的方法；**任意方向目标检测**则是对任意方向上的目标能够用一个框更准确的表示物体的范围，主流的方法可分为基于框的旋转和基于四边形的顶点偏移两种，当前达到SOTA水平的方法有`ICN`，`	ROI-Transformer`，`	ROI-Transformer`，`SCRDet`，`R3Det`等方法。最后一种**方位信息分类**的方法则是用于诸如人脸方向检测等，不在该笔记的讨论之内。

<br>

#### 参考链接

* ECCV2020｜遥感旋转目标检测方法解读：http://www.360doc.com/content/20/0708/21/32196507_923054270.shtml
* 知乎 | 遥感目标检测相关论文：https://zhuanlan.zhihu.com/p/98703562
* GITHUB | DOTA 数据集相关的代码库：https://github.com/SJTU-Thinklab-Det/DOTA-DOAI
* 上交大牛 | 遥感目标检测个人论文说明：https://www.zhihu.com/people/flyyoung-68/posts

<br>

#### 相关论文

* [1] Yang X , Yan J . **Arbitrary-Oriented Object Detection with Circular Smooth Label**. *ECCV*. 2020.
* 

<br>

#### 数据集

* DOTA 1.0：
* DOTA 1.5：
* HRSC2016：

<br>

## 二、已有方法

### 2.1 基于回归的旋转框检测器（Regression-based rotation detectors）

#### (1) 基于Anchor的方法（Anchor-Based）



#### (2) 不基于Anchor的方法（Anchor-Free）



### 2.2 





## 三、方法总结







## 四、个人思考

