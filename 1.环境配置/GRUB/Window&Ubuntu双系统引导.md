# Window&Ubuntu双系统引导

双硬盘安装 Windows 和 Ubuntu 双系统，可能因为引导的丢失，要么只能启动 Windows，要么只能启动 Ubuntu。在经历了多次测试，无脑找了无数资料后，终于摸清楚了**利用 Ubuntu Grub 引导 Windows 和 Ubuntu 双系统的方法**，特意在这里记录一下。



## 一、测试环境&准备材料

* Windows 10 pro (on 1T HHD)，Ubunut 18.04.5 LTS (on 256G SSD)
* Windows 和 Ubuntu 的引导分区分别在 HHD 和 SSD 上
* Windows 的启动模式为 Legency
* 系统的安装顺序为，先安装 Window 在 HHD 硬盘，然后安装 Ubuntu 在 SSD 硬盘。BIOS 引导设置为 SSD 硬盘**
* 出现问题：在 Ubuntu 中使用 `sudo update-grub`或者`sudo update-grub2`无法找到 Windows 的引导，启动的之后也没有选择 Windows 的选项
* 准备材料：一个 Ubuntu18.04.05 LTS 的系统引导盘。我为了方便是用的一张 8G SD 卡加读卡器做的系统盘



## 二、修复引导

* 插入 Ubuntu 系统安装盘，在重启时设置 BIOS 从该系统盘启动，选择 **Try Ubuntu without install**，进入 Ubuntu 系统界面

* 打开一个终端，安装 **boot-repair** 并启动

  ```shell
  # 新增源
  sudo apt-add-repository ppa:yannubuntu/boot-repair
  
  # 安装 boot-repair
  sudo apt update
  sudo apt install boot-repair
  sudo boot-repair
  ```

* 待 boot-repair 启动后，选择第一项**推荐修复**，等待一段时间，如果提示以下信息（**Boot successfully repaired**）则说明修复双系统引导成功

  ```
  Boot successfully repaired.
  
  Please write on a paper the following URL:
  http://paste.ubuntu.com/p/2f7XNF37D8/
  
  
  In case you still experience boot problem, indicate this URL to:
  boot.repair@gmail.com or to your favorite support forum.
  
  You can now reboot your computer.
  
  Please do not forget to make your BIOS boot on the removable disk!
  Please set your BIOS in Legacy mode in order to start your Ubuntu 18.04.5 LTS, then type command [sudo update-grub] in order to add the Windows entry to your GRUB menu.
  ```

* 但是一般来说不会就这样一步到位，我遇到的问题是出现了以下错误：提示我需要一个具有 **bios_grub** 标志的 boot 引导分区

  ```
  GPT detected. Please create a BIOS-Boot partition (>1MB, unformatted filesystem, bios_grub flag). This can be performed via tools such as Gparted. Then try again.
  ```

* 再重新开启一个终端，输入 `gparted`，选择 Ubuntu 所在的 SSD 硬盘，如下所示。发现我的 boot 分区 `/dev/nvme0np1` 的标志是 **boot,esp**，确实不是 **bios_grub**，因此需要修改一下这个分区的标志

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210119104628931.png" width=800>
  </div>

* 右键选择该 boot 分区 `/dev/nvme0np1`，第一步选择**卸载分区**，然后再选择**管理标签**，修改该分区的标签为**bios_grub**

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210119104852349.png" width=800>
  </div>

* 保存所有`gparted`的更改，然后退出该`gparted`软件。重新打开 boot-repair，并再次选择**推荐修复**，按照提示的指令一行一行操作，即可修复成功

* 重启系统，拔出Ubuntu系统盘，然后正常进入 Ubuntu 系统，更新系统引导。如下图所示，出现 **Found Windows 10 on /dev/sda1** 说明修复成功

  ```shell
  sudo update-grub
  sudo update-grub2
  ```

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210119105306688.png" width=800>
  </div>



## 三、美化界面

* Ubuntu 的默认黑底白字的引导界面说起来，真觉得挺难看的，实际上是可以更换引导界面的主题的，这是我参照的修改GRUB引导主题的[链接](https://blog.csdn.net/w84963568/article/details/78884003)

* 我选择的是一个名为`poly light`的 GRUB 引导主题，也可以按照自己的喜好在官方主题里面去找自己喜欢的然后自己修改，参见下述参考链接

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/68747470733a2f2f692e696d6775722e636f6d2f39564266314e6c2e676966.gif" width=800>
  </div>

* `poly light`主题的快捷安装方式

  ```shell
  wget -O - https://github.com/shvchk/poly-light/raw/master/install.sh | bash
  ```

* 需要注意的是，需要修改一下配置文件的分辨率，不然GRUB启动的时候不是高清背景，看着更难受了

  ```
  sudo vi /etc/grub.d/00_header
  
  # 在注释下添加
  GRUB_THEME="/boot/grub/themes/主题包名/theme.txt"
  GRUB_GFXMODE="1920x1080x32"
  ```

  <div align="center">
      <img src="https://markdow-picbed.oss-cn-beijing.aliyuncs.com/img/image-20210119110047709.png" width=800>
  </div>

* 更新引导配置文件

  ```shell
  sudo update-grub
  sudo update-grub2
  ```

* 重启后，大功告成！

  

## 参考链接

* [1] 重装系统时，如何判断Windows的启动方式是Legacy还是UEFI？| [百度知道](https://baijiahao.baidu.com/s?id=1655241625863350057&wfr=spider&for=pc)
* [2] T440s 预装win8.1 GPT+EFI ubuntu 双系统 图文过程 | [百度贴吧](https://tieba.baidu.com/p/2805772637?red_tag=1028795307)
* [3] [Ubuntu笔记]我的Linux之路--Grub2主题修改 | [CSDN](https://blog.csdn.net/w84963568/article/details/78884003)
* [4] GRUB 官方主题 | [GRUB](https://www.gnome-look.org/browse/cat/109/order/latest)
* [5] GRUB poly light 主题 | [Github](https://github.com/shvchk/poly-light)

