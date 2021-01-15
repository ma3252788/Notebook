# CUDA环境配置和升降级

有时候 NVIDIA 的驱动可能因为一次系统升级或者系统驱动更新就找不到了，鉴于 CSDN 和 某度 上面的笔记经验鱼目混杂，本笔记针对 Ubuntu 的 **CUDA 环境完全卸载、安装以及升降级**的方法做一个记录，个人的测试环境为 Ubuntu 18.04.5 LTS，但是也对 Ubuntu 16.04 LTS 等其他 Ubuntu 系统有效



## 一、驱动卸载

* 如果第一次系统装机，需要提前**将 NVIDIA 相关驱动完全卸载**，然后重装驱动，才能正常使用 CUDA

* 卸载过程如下所示（建议每一次卸载都这样操作）：

  * 查看当前是否有 NVIDIA 驱动

    ```shell
    sudo dpkg --list | grep nvidia
    ```

  * 如果之前已经按照**下述方法**安装过驱动，请先执行这几步；如果是第一次安装驱动，可以跳过这个步骤：

    * 查看之前已经安装过的驱动版本

      ```shell
      cd /usr/local
      ls
      ```

    * 如果发现目录下有`cuda/`文件夹，则说明已经在之前安装过驱动

    * 进入`cuda/bin`，卸载已经安装过的驱动

      ```shell
      cd cuda/bin
      ./cuda-uninstaller
      ```

  * 

  



## 二、驱动下载

* 不建议直接使用`sudo apt-get install nvidia-compute-toolkit`或者`sudo apt-get install nvidia-driver-445`等方式直接安装驱动。驱动版本的安装还是需要按照自己的显卡需求，不然后面还要去卸载，挺麻烦的 ...

* 因为我的显卡是`RTX 2070S`，支持`CUDA 10.1`, `CUDA 10.2`, `CUDA 11.0`。为了后续开发过程中的版本兼容，安装`CUDA 10.2`即可，官网提供了两种本地安装的方式，一种是`deb(local)`【不推荐】，另一种是`runfile(local)`【推荐，简单快捷，安装过程不需要重装依赖包】。我选择`runfile`的安装方式：[[官方地址]](https://developer.nvidia.com/cuda-10.2-download-archive?target_os=Linux&target_arch=x86_64&target_distro=Ubuntu&target_version=1604&target_type=runfilelocal)

  <div align="center">
  <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210115112738284.png" width=800>    
  </div>

  安装如上方式选择驱动版本，然后下载驱动文件（需要翻墙）：

  ```shell
  wget https://developer.download.nvidia.com/compute/cuda/10.2/Prod/local_installers/cuda_10.2.89_440.33.01_linux.run
  ```



## 三、驱动安装和升降级

### 3.1 驱动安装



### 3.2 驱动升降级



## 四、CUDA 配置





## 五、参考链接



