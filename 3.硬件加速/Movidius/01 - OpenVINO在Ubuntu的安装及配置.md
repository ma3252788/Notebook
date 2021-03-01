# 01 - OpenVINO安装及配置

* 以下笔记是本科时的毕业设计时，基于**Movidius V2**的神经棒做**神经网络加速推理**，并在使用过程中对遇到的**安装步骤**和**使用问题**做的一些笔记和记录。因为之前采用为知笔记进行记录，笔记也很乱，因此趁着闲暇时间整理成Markdown格式的文档，方便以后查阅。当时使用的稳定版本为**OpenVINO 2018 R4**，目前英特尔已经对此更新了很多版本，增加了很多功能。对新版本的使用笔记将在后续有更多闲暇时更新...

* 其余三篇笔记分别是：
  * [02 - 编译 Tensorflow 模型并部署执行.md](./02 - 编译 Tensorflow 模型并部署执行.md)
  * [03 - OpenVINO's Inference Engine.md](./03 - OpenVINO's Inference Engine.md)
  * [04 - 编译模型发现的问题及Bug.md](./04 - 编译模型发现的问题及Bug.md)



## 一、Movidius V2 注意事项

* 使用环境：Ubuntu16.04.5 LTS，非 root 模式， xrdp 远程连接 Unity 桌面

* Inter 说二代的神经棒不支持 NCSDK，需要使用 OpenVINO（[[参考链接]]((https://ncsforum.movidius.com/discussion/1302/intel-neural-compute-stick-2-information))），否则会出现以下 **Toolkit Error: USB Failure. Code: Error opening device** 的错误，NCSDK 以及 NCSDK v2 都只适用于一代神经棒（老坑比！）

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301144453596.png" width=700>
  </div>

* 推荐使用 **OpenVINO 2018 R4** 版本，因为这个版本支持 python3.5.2，且比较稳定， OpenVINO 2018 R5 版本个人认为（现在可以用更新的版本），官方说明文档乱七八糟，需要 python3.6 及以上版本，将在以后陈述（事实证明，并不是越新越好），鉴于这个笔记我是在用了 R5 才换成 R4 的，以下的过程还是 R5 版本，两者相差不大，都是细节上的问题。

  

## 二、Movidius V2 相关组件安装

* 下载 **OpenVINO Toolkit R5** ([[参考网址]](https://software.intel.com/en-us/neural-compute-stick/get-started))，解压到当前文件夹。在通过 GUI 界面完成安装之后，会自动弹出一个网页页面，引导之后的安装（弹出的 Linux 环境安装[[引导网页]](https://software.intel.com/en-us/articles/OpenVINO-Install-Linux)，**也可以按照我后面的记录来安装**

  ```shell
  cd ~/Downloads
  tar xvf l_openvino_toolkit_<VERSION>.tgz
  cd l_openvino_toolkit_<VERSION>
  ./install_cv_sdk_dependencies.sh
  ./install_GUI.sh
  ```

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301145559083.png" width=700>
  </div>

* 初始化 OpenVINO 环境变量：

  * 在编译和运行 OpenVINO 之前必须要先初始化 OpenVINO 环境，在终端中输入便可以初始化环境

    ```shell
    source /opt/intel/computer_vision_sdk/bin/setupvars.sh
    ```

  * 假如不想每次打开终端都要去输入这一行代码，则修改 ~/.bashrc 并在最后一行，加入上面这一行代码，则每次启动终端都会自动更新变量

    <div align="center">
        <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301145949659.png" width=700>
    </div>

* 一次性配置模型优化器 Model Optimizer（支持 Caffe、 TensorFlow、 MXNet、 Kaldi *，和 ONNX ）

  * 转到模型优化器的目录，运行安装脚本：

    ```shell
    cd /opt/intel/computer_vision_sdk/deployment_tools/model_optimizer/install_prerequisites
    sudo ./install_prerequisites.sh
    ```

  * 如果觉得下载很慢，可以运行我修改后的[[my_install_prerequisites.sh]](./src/my_install_prerequisits.sh)，更改了其中的下载源，增加超时时间设置，下载更快更稳定

    ```shell
    sudo ./my_install_prerequisites.sh
    ```

* 运行图像分类的 Demo 

  ```shell
  cd /opt/intel/computer_vision_sdk/deployment_tools/demo
  ./demo_squeezenet_download_convert_run.sh
  ```

  *  运行结果，识别结果为跑车，准确率 0.8363345，耗时 4.7708ms：

    <div align="center">
        <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210301150412006.png" width=700>
    </div>

## 三、使用 NCS2 出现 NC DEVICE 没有找到插件的解决方法

* 当我们在 `/opt/intel/computer_vision_sdk/deployment_tools/demo` 文件夹下使用 NCS2 运行 demo.

  ```shell
  sudo ./demo_squeezenet_download_convert_run.sh -d MYRIAD
  ```

* 如若出现以下错误：

  ```shell
  ...(以上提示信息忽略)...
  [ INFO ] Preparing output blobs
  [ INFO ] Loading model to the plugin
  E: [ncAPI] [         0] ncDeviceOpen:636 failed to find device
   
  [ ERROR ] Can not init USB device: NC_DEVICE_NOT_FOUND
  Error on or near line 201; exiting with status 1
  ```

* 按照以下操作进行：

  * 下载并运行以下 [[97-usbboot_pre_ln.sh]](./src/97-usbboot_pre_ln.sh) 文件，以删除 openvino 安装目录下多余的文件，建立 so 文件的软连接

    ```shell
    bash ./97-usbboot_pre_ln.sh
    ```

  * 下载 [[97-usbboot.rules]](./src/97-usbboot.rules) 文件， 将该 rules 文件放在桌面，在桌面空白处打开一个终端，输入一下过程即可

    ```shell
    sudo usermod -a -G user "$(whoami)"    # 首先将当前的Linux用户加入到用户组；
    
    sudo cp 97-myriad-usbboot.rules /etc/udev/rules.d/
    sudo udevadm control --reload-rules
    sudo udevadm trigger
    sudo ldconfig
    
    sudo rm -r 97-usbboot.rules  # 删除桌面上的 rule 文件, 不是 /etc/udev/rules.d/ 下的
    ```

    

