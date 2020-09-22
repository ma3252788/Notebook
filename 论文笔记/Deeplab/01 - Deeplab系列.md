## 一、简介与参考

语义分割是对图像中每个像素进行划分，本质上仍旧是一种逐像素的分类问题。语义分割的方法可以抽象为：全卷积 CNN 对图像进行特征提取，然后融合浅层特征进行上采样将 score map 恢复到输入大小。其中涉及到的几个重要的部分为：CNN backbone、特征融合策略、上采样策略。另外，与图像分类不同的是，单看每个像素都是一个孤立点，要想分割效果好就要充分利用当前像素点周围的特征信息。语义分割的论文始终都是在围绕这四个点进行不断迭代改进，只是所选择的思路有所不同。比如对于“pool 层在扩大感受野的同时会损失空间信息”这个问题，FCN采用与浅层特征相融合的方式解决，而 DeepLab系列的策略则是用空洞卷积代替 pool 层，在不损失空间信息的前提下增大卷积的感受野。（*摘录于蒋博笔记*）

* Deeplab系列（从 V1 到 V3+ 一共四个版本）是比较经典的语言分割方法

* 虽然后续引入空洞卷积的方法直观有效，但是不如 RPN 这种划时代意义的网络来的让人振奋

<br>

#### `（问：语义分割是什么意思？语义分割的语义是什么意思？）`

* 图像的像素级分类称之为语义图像分割（Adapted from : Abstract of DeepLabV1）

<br>

#### 参考资料：

**DeepLabV1** (https://arxiv.org/pdf/1412.7062v3.pdf): Semantic Image Segmentation with Deep Convolutional Nets and Fully Connected CRFs

**DeepLabV2** (https://arxiv.org/pdf/1606.00915.pdf): DeepLab: Semantic Image Segmentation with Deep Convolutional Nets, Atrous Convolution, and Fully Connected CRFs

**DeepLabV3** (https://arxiv.org/pdf/1706.05587.pdf): Rethinking Atrous Convolution for Semantic Image Segmentation

**DeepLabV3+** (https://arxiv.org/pdf/1802.02611.pdf): Encoder-Decoder with Atrous Separable Convolution for Semantic Image Segmentation

<br>

## 二、系列讲解

### 2.1 DeepLabV1

#### (1) 模型特点与结构

* 将“空洞”方法应用于 DCNN，应用全连接的条件随机场：提高模型获取精细细节的能力

* V1采用backbone为VGG16的全卷积网络（已预训练），通过CNN得到feature map并通过双线性插值恢复到原图大小得到coarse output，最后用CRF得到精细输出

* 模型示意图：

<div align="center">
    <img src="..\Deeplab\assets\01 - Deeplab系列\2b86f35d-6a63-46a9-bbac-327d3ee9bb2e.jpg" alt="img" style="zoom:85%;" />
</div>

#### (2) 模型解决的问题

* **空洞卷积**：通过将网络最后两层maxpool层去掉，在相应的网络层引入空洞卷积在更多保留空间信息的同时**对网络的感受野**不产生影响，通过空洞卷积的引入最终的特征图大小从原输入的1/32变为1/8，特征密度也更大了。例如：对使用了s=2后的lower resolution feature map再进行标准的卷积的效果和在原来的feature map上使用rate=2的空洞卷积的效果是一样的。

* **条件随机场CRF**：V1中运用CRF对CNN输出的coarse output进行精细化调整。CRF将coarse output建模为一个全连接的无向图模型，每个像素代表一个节点，边代表节点间的关系。CRF就相当于用每个节点的全局关系进行调整

* **多尺度特征融合（Sec 4.3）**：融合了低网络层的特征，将网络前四个max pool的feature map提取出来(每个featuremap后接3×3卷积、1×1卷积，输出128通道的feature map)与主干网络输出concate起来之后一起送入最终的分类层中。

#### (3) 模型的优势与缺陷

* 优势：
  * 速度上：以基于“空洞”的稠密 DCNN 算法，在 GPU 上能达到 8 帧，而 CRF 仅有 0.5 帧每秒
  * 准确率：达到了当时的 SOTA，在 PASCAL 数据集上，比当时的第二名超出 7.2%
  * 简洁性：由 DCNN + CRF 级联组成

* 缺陷：
  * 随机条件场 CRF 方法使该 DeepLabV1 不能构成端到端的模型，也限制了模型的速度
  * VGG 网络再更深时，会有梯度消失或者梯度爆炸，不利于特征表达的问题

<br>

### 2.2 DeepLabV2

#### (1) 模型特点与结构

* 该 DeepLabV2 和 V1 相较之下没有什么太大的改进，增加了 ASPP 网络（我个人觉得就是换了点写法），特征提取网络采用 VGG16 或者 ResNet-101，V2 的也是在得到 feature map 之后，使用双线性插值的方式将特征图的分辨率提高到和输入图片相同的大小* 最后上采样后的特征图也是采用 CRF 对像素分类进行细化，完善分割效

* 模型示意图（`Any difference between DeepLabV1?`）：

  <img src="..\Deeplab\assets\01 - Deeplab系列\806c135a-4957-4f76-907b-6baa9c984982.jpg" alt="img"  align="center" style="zoom:90%;"/>

#### (2) 模型解决的问题

* backbone 使用 ResNet-101 能够提升网络的表达能力

* 针对**图像中同一类物体存在不同的尺度**这一特点，V2中提出ASPP模块，对同一特征图并行地用多个不同扩张率（6，12，18，24）的空洞卷积进行处理然后进行特征融合。使用多尺度进行空洞卷积，在经过1*1的卷积之后连接起来。多尺度特征提取，得到全局和局部特征：

  <img src="..\Deeplab\assets\01 - Deeplab系列\0.391306651716615.png" alt="img" style="zoom:90%;" align="center" />

<br>

### 2.3 DeepLabV3

#### (1) 模型特点与结构

* DeepLab V3主要做了三件事情：

  * (1) **ASPP模块进行了改进**：V3中提到虽然不同扩张率的空洞卷积可以得到不同尺度的特征信息，但是随着扩张率的增大，kernel 所能覆盖到的有效特征值越来越少，极端情况下会退化成1×1卷积。所以V3对ASPP的支路做了变化：用1×1卷积将 rate=24 的支路替换掉，同时为了结合图像全局信息还新增了全局平均池化支路，并运用双线性插值恢复到所需的尺寸。另外，ASPP还加入了BN层帮助训练
  * (2) **网络可以更深**：对于空洞卷积模块，文章给出了一种串行的网络设计使得网络可以更深

  * (3) **去掉了CRF处理模块**：DCNN的输出已经够好，后处理意义不大了

* 模型示意图：

  <img src="..\Deeplab\assets\01 - Deeplab系列\image-20200922110615449.png" alt="img" style="zoom:90%;" />

#### (2) 模型解决的问题

* 使用多尺度的空洞卷积核**来解决实例对象多尺度的问题**，构成 ASPP 网络，让网络能够扩大感受野并有效提取全局特征
* 因为 DCNN 网络的表现性能已经足够好了，因此**取消了条件随机场 CRF **的校对过程，一定程度上可以提高网络的速度

#### (3) 模型的优势与缺陷

* 优势：
  * 提出了多尺度融合的方法，利用多尺度空洞卷积解决了对象多尺度问题
  * 使用 ResNet 网络，网络模型可以更深了
* 缺陷：
  * 因为深度网络存在 stride=2 的层，导致 feature 的分辨率下降，从而导致预测精度降低，造成边界信息丢失的问题
  * 最后的 feature map 依旧是采用**双线性插值**的方法，将分辨率提高到原图大小，这样势必**依旧会造成精度受损的问题**

#### (4) 模型复现

* 代码地址：https://github.com/Memoristor/Deep-Coding/tree/master/network/deeplab

<br>

### 2.4 DeepLabV3+

#### (1) 模型特点与结构

* Deeplab V3+ 相比于 Deeplab V3 主要更改在**Backbone升级、替换Pooling层、精细化的上采样**
  * (1) **Backbone升级**。尝试将ResNet101改为了Xception，并且将深度可分离卷积应用到了ASPP模块中，所有的pool层都用；
  * (2) **替换Pool层**。V3+将Xception的结构也做了一定的改进，其中最重要的就是将所有的Pool层都用stride=2的深度可分离卷积进行了替换；
  * (3) **上采样策略精细化**。前三版的最终输出都是直接通过双线性插值直接恢复到输入大小。V3+中在双线性插值的基础上借鉴了Encoder-Decoder的结构：首先将特征图双线性插值上采样4倍，与浅层特征空间大小一致的特征图（ResNet中为Conv2，Xception中为第二个stride=2的输出）进行结合，这种策略能够得到更加准确的物体轮廓信息。特征融合的过程为：浅层特征先经过1×1卷积缩减通道数，然后与4倍上采样的特征图concate之后经过3×3卷积融合，最后再进行一次4倍的双线性插值得到最终的输出。

* 模型示意图：

<img src="..\Deeplab\assets\01 - Deeplab系列\image-20200922170419155.png" alt="img" style="zoom:58%;" />

<img src="..\Deeplab\assets\01 - Deeplab系列\image-20200922170015427.png" alt="img" style="zoom:75%;" />



#### (2) 模型解决的问题

* 使用新颖的**编码器 - 解码器**结构，解决前三个版本都是使用双线性插值将 feature map 恢复到原图分辨率，造成精度损失的问题
* 由于**尺度比较大的特征图的计算需要花费较多的计算资源**，Deeplab V3+做了折衷，将 stride=8 提高到 stride=16
* 将**Xception模型**用于分段任务，并将**深度可分离卷积**应用于ASPP模块和解码器模块，从而形成了更快，更强大的编码器-解码器网络。

#### (3) 模型的优势与缺陷

* 优势：
  * 采用 Encoder-Decoder 的结构，对语义分割进行精细化，能够在没有什么后续处理的情况下，达到 SOTA 水准
  * 采用Xception模型、深度可分离卷积构成更强大的编码-解码器
  * stride从8增加到16，减少了Deeplab的计算量
* 缺陷：
  * 将 Decoder 的特征图恢复尺度时虽然结合了 Encoder 中相同尺度的 feature map，但是依旧逃脱不了使用双线性插值的命运
  * 虽然**空洞卷积能够在不过多损失空间信息的前提下保证感受野不变**，但是该卷积核的形状固定，并不能对物体的形状进行自适应改变

#### (4) 模型复现

* 代码地址：https://github.com/Memoristor/Deep-Coding/tree/master/network/deeplab

<br>

## 三、各版本的对比

**V1与FCN**：相比之前FCN框架下的语义分割算法，DeepLab系列最重要的改进就在于引入空洞卷积机制来平衡网络感受野不断加大过程中丢失的空间信息，同时对于每一个点都增大了上下文信息。DeepLab中还引入了score map的后处理阶段，用CRF对网络上采样得到的输出运用概率图模型进一步的精炼。

**V1与V2**：用空洞卷积解决了空间信息损失的问题之后，语义分割还面临的一个问题是在同一个场景下同类物体可能会表现出不同的尺度。V2用ASPP模块提取多尺度的特征解决这个问题。

**V2与V3**：V3主要是对ASPP模块的完善。V2中用不同扩张率的空洞卷积来提取不同尺度的特征。但是V3中提出，当扩张率过大时卷积核所能覆盖到的有效特征会减少，极端情况下空洞卷积会退化成1×1卷积。针对这个问题V3删除掉了扩张率最大的支路用1×1卷积代替，同时增加了全局平均池化的支路引入全局信息。

**V3与V3+**：当主要的模块（空洞卷积、ASPP）都完善之后，V3+引入了更强大的网络backbone，并且对最后的网络上采样策略进行了完善，引入了Encoder-Decoder的结构，提高输入对轮廓信息的恢复能力。

<br>

## 四、总结与思考

### (1) 个人想法

* **Deeplab系列总结**：DeepLab从V1到V3+始终是围绕着**简介中的四点**在改进，V2和V3过渡阶段提出并改进了ASPP，V3+将前面工作的精华都结合起来之后得到了最终的DeepLab形态。

* **V3到V3+为什么stride从8增加到16**：空洞卷积需要在高分辨率的特征图上进行操作，V1和V2的stride都为8，高分辨率的特征图会带来超大的计算量。但随着网络backbone的不断升级，ASPP的提出并完善，DeepLab的性能不断增强也就不需要这么高的特征图分辨率（同时在这个过程中CRF也变得没有意义），在保证一个比较好的性能的同时，将stride变为16能够加快计算。

* **为什么是Xception**：类似2中的理由，空洞卷积需要的高分辨率特征会加大网络的计算量，Xception网络在性能提升的时候引入了深度可分离卷积可以进一步减小DeepLab的计算量。

### (2) 关于改进的思考

* **空洞卷积**的初衷是为了在不过多损失空间信息的前提下保证感受野不变，但空洞卷积只能按照固定形状对感受野进行扩张。可变卷积在扩张卷积kernel感受野的时候还可以根据图像的信息进行形状的自适应，对于轮廓不规整的物体应该会有性能提升。

* DeepLab的作者在之后的研究中还尝试了运用**NAS**来得到比更有效的ASPP架构。

