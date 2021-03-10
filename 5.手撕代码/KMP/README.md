# KMP算法

> **KMP算法**是一种改进的字符串匹配算法，由D.E.Knuth，J.H.Morris和V.R.Pratt提出的，因此人们称它为克努特—莫里斯—普拉特操作（简称KMP算法）。KMP算法的核心是利用匹配失败后的信息，尽量减少模式串与主串的匹配次数以达到快速匹配的目的。具体实现就是通过一个next()函数实现，[函数](https://baike.baidu.com/item/函数/18686609)本身包含了模式串的局部匹配信息。KMP算法的[时间复杂度](https://baike.baidu.com/item/时间复杂度/1894057)O(m+n) [1] 。
>
> —— 来自：百度百科

## 一、运行环境

* 编译环境：Windows / Clion / CMake
* 编程语言：C++

## 二、代码实现

### 1.1 关于 next 数组实现



### 1.2 关于 KMP 快速匹配



## 三、参考链接

* [1] 天勤公开课：KMP算法易懂版 | [[B站]](https://www.bilibili.com/video/BV1jb411V78H?from=search&seid=14079601092891626322)

* [2] 代码随想录：帮你把KMP算法学个通透！（理论篇）| [[B站]](https://www.bilibili.com/video/BV1PD4y1o7nd)
* [3] 代码随想录：帮你把KMP算法学个通透！（求next数组代码篇）| [[B站]](https://www.bilibili.com/video/BV1M5411j7Xx?from=search&seid=14079601092891626322)

