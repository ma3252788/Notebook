# 04 - 编译模型发现的问题及Bug

在模型编译和将模型载入神经棒进行实际使用和测试的过程中，有些问题因为当时资料很少，而且官方文档写得比较乱，在实际运行过程中遇到过很多问题，最多的就是**模型无法正确转换**，**Layer不支持**，**模型转换前后的输出结果不一致**以及**精度损失**。可能这些问题在新版本中已经解决，但是在后续开发和使用中如果遇到了这种类似的问题，也可以多留个心眼。



## 一、在编译过程中发现优化器（mo_tf.py）的 Bug 和问题

* 在 **OpenVINO 2018 R5** 版本中，使用 tensorflow 保存的模型优化编译为 Inference engine 可以使用的模型时，tensorflow 的图中**不能有 tf.exp(x), tf.pow(x)** 的运算，即使是在图中就用了一个也不行，不然，就会出现以下错误**(List of operations that cannot be converted to IE IR)**：

  ```shell
  ...(上面的编译信息省略)...
  [ ERROR ]  List of operations that cannot be converted to IE IR:
  [ ERROR ]      Exp (2)
  [ ERROR ]          detector/yolo-v3-tiny/Exp
  [ ERROR ]          detector/yolo-v3-tiny/Exp_1
  [ ERROR ]  Part of the nodes was not translated to IE. Stopped. 
   For more information please refer to Model Optimizer FAQ (<INSTALL_DIR>/deployment_tools/documentation/docs/MO_FAQ.html)
  ```

  * 我尝试过使用 tf.sigmoid(x) 的方式计算 tf.exp(x) 曲线救国，即 e^(x) = S(x) / (1-S(x))，其中 S(x) = sigmoid(x)，但是实际上和我想的一样，虽然**这种方式可以编译成功**，这种方式只对 x ∈ (-3, 3) 之间有效（sigmoid(x) 的曲线决定的），否则计算结果不是误差很大就是 inf。**到目前为止，没有找到可以解决的办法。**
  * 除了 tf.exp(x)，还有 tf.pow(x) 和 tf.log(x) 出现在 tensorflow 中时，**也会出现该错误**，可能真的和这个神经棒所描述的那样：优异 “视觉” 神经棒，所以处理 **卷积池化** 这种操作比较擅长，但是处理这种数学运算还是有点鸡肋。
  * 此外，当我在使用 **tf.image.non_maximum_suppression(...)** 的时候，也出现了 **List of operations that cannot be converted to IE IR** 这个错误，我想这个问题是因为 OpenVINO 支持不到这个层。

* 虽然在官方网站上说了，Movidius 设备支持 BatchNormalization 这个**批归一化层**，但是在我实际的操作过程中，发现 Keras.Layer 中的 BatchNormalization 和 OpenVINO 编译器替换的 BatchNormalization 还是有差别的，即 tensorflow 计算的结果和 InferenceEngine **在相同参数下计算出来的结果不一致**，因而在需要将模型优化为 OpenVINO 模型的时候，**不建议**在 tensorflow 网络中使用 基于 tensorflow 为底层，keras 给出的 BatchNormalization 层。

* **千万不要随便使用 tf.expand_dim(...) !!!** 肝 到吐血得到的教训。我在写 Faster RCNN 的时候，一次输入一张图，三维数据，比如 [200, 240, 3]，然后使用 tf.expand_dim(x, axis=0) 将其升维到 [1, 200, 240, 3]，写好的模型转为 IR, 但是吐血的事情发生了，输出的 feature_map 完全不对！我以为是 OpenVINO 转换的问题，查了好多帖子搞得精力憔悴。总之，OpenVINO 会在卷积之前将其 [N, H, W, C] 转为 [N, C, H, W]，有可能在这里对 tf.expand_dim(...) 支持不好导致结果有误差。

## 二、精度损失

* Movidius V2 为了能够增加运算速度，因此**支持的精度为 FP16**，而我们的模型一般默认的精度是 **FP32**，在模型转换的过程中会不可避免得遇到一些精度损失带来的连锁反应：检测框定位预测不准、检测框的长宽预测不准、物体识别出错等待。
* 除了专门的量化压缩等方法外，如果要规避这个问题，可以在预测一些比较小的值的时候（如长宽的对数偏移量），将这些值调大10倍左右，例如某一个检测框的偏移量的 **ground truth = 0.3**，那么可以将该值放大 10 倍，使网络模型需要预测的值从 0.3 增大为 3，然后后续手动将预测值减小 10 倍，减小在 **FP16** 量化时带来的损失。当然，这样做可能需要对网络模型的预测层（如 sigmoid 或者 linear 层）做相应的改进，避免预测值过大，超出激活函数的最佳响应范围。