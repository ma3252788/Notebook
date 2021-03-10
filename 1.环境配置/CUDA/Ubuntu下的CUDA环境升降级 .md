# CUDA环境配置和升降级

有时可能因为一次系统升级或者系统驱动更新， NVIDIA 的驱动就找不到了。鉴于 CSDN 和 某度 上面的笔记鱼龙混杂，本笔记针**对 Ubuntu 的 CUDA 环境完全卸载、安装以及升降级**的方法做一个记录，个人的测试环境为 Ubuntu 18.04.5 LTS，但是也对 Ubuntu 16.04 LTS 等其他 Ubuntu 系统有效



## 一、驱动下载

* 不建议直接使用`sudo apt-get install nvidia-compute-toolkit`或者`sudo apt-get install nvidia-driver-445`等方式直接安装驱动。**驱动版本的安装还是需要按照自己的显卡需求**，不然后面还要去卸载，挺麻烦的 ...

* 因为我的显卡是`RTX2070Super`，支持**CUDA 10.1, CUDA 10.2, CUDA 11.0**。为了后续开发过程中的版本兼容安装`CUDA 10.2`即可，官网提供了两种本地安装的方式，一种是`deb(local)`【不推荐】，另一种是`runfile(local)`【推荐，简单快捷，安装过程不需要重装依赖包】。我选择`runfile`的安装方式，官方下载地址：[[官方地址]](https://developer.nvidia.com/cuda-10.2-download-archive?target_os=Linux&target_arch=x86_64&target_distro=Ubuntu&target_version=1604&target_type=runfilelocal)

  <div align="center">
  <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210115112738284.png" width=800>    
  </div>


  如上图所示选择驱动版本，然后下载驱动文件（需要翻墙，我下载到 **/home/user/Downloads** 文件夹）：

  ```shell
  wget https://developer.download.nvidia.com/compute/cuda/10.2/Prod/local_installers/cuda_10.2.89_440.33.01_linux.run
  ```



## 二、驱动卸载

* 如果第一次系统装机，需要提前**将 NVIDIA 相关驱动完全卸载**，然后重装合适的驱动版本，才能正常使用 CUDA 加速库

* 重启 Ubuntu 系统，在重启时按 **DEL（电脑不同可能按键不同）**，进入系统引导选择界面，选择 **Advanced options for Ubuntu**，选择自己的 Ubuntu 内核版本，以 **recovery mode**（安全恢复模式）进入命令行系统（这样做的**目的是不启用任何的显示驱动**，比如 lightdm 等等），如下图所示：

  <div align="center">
  <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210115114942361.png" width=800>    
  </div>

* 这里需要注意的是：**必须选择合适的内核版本！因为有些内核版本对 CUDA 驱动支持不好，可能导致 CUDA 驱动一直安装不上**，我的经验是内核版本不要选太高，我这里选择的是`Linux 4.15.0.131-generic`。提示：这个内核文件在进入Ubuntu后的`/boot`目录下，不需要内核可以在进入系统后直接删掉就行**（请谨慎操作）**，例如：

  ```shell
  # 删除内核
  cd /boot
  sudo rm -rf vmlinux-5.4.0.52-generic
  sudo rm -rf initrd-5.4.0.52-generic
  
  # 更新引导
  sudo update-grub 
  sudo update-grub2
  ```

* 进入命令行模式后，系统界面如下所示，选择**root**，进入管理员模式

  <div align="center">
  <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210118170758485.png" width=800>    
  </div>

* 卸载过程如下所示（建议每一次卸载都这样操作）：

  * 查看当前是否有 NVIDIA 驱动

    ```shell
    sudo dpkg --list | grep nvidia
    ```

  * 如果之前已经按照**本文的NVIDIA驱动的下载和安装方法**安装过驱动，请先执行这几步；如果是第一次安装驱动，可以跳过这个步骤：

    * 查看之前已经安装过的驱动版本

      ```shell
      cd /usr/local & ls
      ```

    * 如果发现目录下有`cuda/`文件夹，则说明已经在之前安装过驱动（一般`cuda/`是`cuda-10.2/`或其他CUDA版本的软连接）

    * 进入`cuda/bin`，卸载已经安装过的驱动

      ```shell
      cd cuda/bin
      ./cuda-uninstaller
      ```
    
    * 全选以下几个选项，然后选择**Done**，等待一段时间后完成卸载，成功卸载会提示：**Successfully uninstalled**

      <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210118171109235.png" width=800>    
      </div>
  
  * 卸载剩余的 NVIDIA 驱动
  
    ```shell
    sudo apt-get autoremove --purge nvidia*
    sudo apt-get autoremove
    ```
  
* 至此驱动卸载完成，开始进行第三步：**驱动安装和升降级**

  

## 三、驱动安装和升降级

### 3.1 驱动安装

* 在第一部分中已经记录了驱动的下载，我把驱动放在了**/home/user/Downloads**目录下，运行`runfile`驱动安装程序

  ```shell
  cd /home/user/Downloads
  chmod +x cuda_10.2.89_440.33.01_linux.run
  sh cuda_10.2.89_440.33.01_linux.run
  ```

* 弹出以下界面，并在左下角输入 **accept**：

  <div align="center">
  <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210118171832188.png" width=800>    
  </div>

* 全选所有选项，并选择 **Install** 后按下回车键

  <div align="center">
  <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210118172005780.png" width=800>    
  </div>

* 安装成功，则会显示以下提示信息，说明安装完成：

  <div align="center">
  <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210118172100967.png" width=800>    
  </div>

* 最后一步，配置环境到 **~/.bashrc**：

  ```shell
  vi ~/.bashrc
  
  # 添加以下两行并保存退出
  export PATH=/usr/local/cuda/bin${PATH:+:${PATH}}
  export LD_LIBRARY_PATH=/usr/local/cuda/lib64 ${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
  
  # 更新环境
  source ~/.bashrc
  ```

* 输入 **nvidia-smi** 查看 CUDA 驱动是否安装成功，如果显示以下界面则说明成功（啰嗦一句：GPU-Util为0%说明**recovery mode**安全恢复模式下确实没有使用任何显卡资源）：

  <div align="center">
  <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210118205021297.png" width=800>    
  </div>

* 输入 **reboot** 重启，**以正常模式进入 ubuntu**，打开终端输入 nvidia-smi，出现以下提示，说明安装成功。

  <div align="center">
  <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210118210143676.png" width=800>    
  </div>

### 3.2 驱动升降级

* 升降级 = （卸载 + 重装），不必赘述 :)



## 四、参考链接

* [1] Ubuntu16.04配置CUDA开发环境 | [CSDN](https://blog.csdn.net/yjj546542806/article/details/79016404)
* [2] ubuntu16.04更新nvidia驱动的两种方法 | [CSDN](https://blog.csdn.net/u010420283/article/details/104020531)
* [3] CUDA Toolkit | [NVIDIA](https://developer.nvidia.com/CUDA-toolkit) 