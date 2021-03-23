# KMP算法

> **KMP算法**是一种改进的字符串匹配算法，由D.E.Knuth，J.H.Morris和V.R.Pratt提出的，因此人们称它为克努特—莫里斯—普拉特操作（简称KMP算法）。KMP算法的核心是利用匹配失败后的信息，尽量减少模式串与主串的匹配次数以达到快速匹配的目的。具体实现就是通过一个next()函数实现，[函数](https://baike.baidu.com/item/函数/18686609)本身包含了模式串的局部匹配信息。KMP算法的[时间复杂度](https://baike.baidu.com/item/时间复杂度/1894057)为：O(m+n) 。
>
> —— 来自：百度百科

## 一、运行环境

* 编译环境：Ubuntu18.04 / Clion / CMake
* 编程语言：C++

## 二、代码实现

### 1.1 关于 next 数组实现

* next 数组的长度等于 **模式字符串的长度+1**

* next[0] 固定设置为 -1，只是为了编程方便

  ```c
    // 获取 next 数组，注意其长度等于 s.size() + 1
      void getNext(string s, int *next) {
  
          int i = 0, j = -1;
  
          // next 的 0 位必须是 -1
          next[0] = -1;
  
          while (i < s.size()) {
  
              if (j == -1 || s[i] == s[j]) {
                  next[++i] = ++j;
              } else {
                  j = next[j];
              }
          }
      }
  ```

* 当字符串为`abababc`时，对应的next数组为：`-1	0	0	1	2	3	4	0`。与实际结果相符合。

### 1.2 关于 KMP 快速匹配

* 代码实现，其具体的原理和过程参见参考链接[4]：

  * 在这个代码实现中，有一个**比较细节的点**：`target.size()` 即 **string 类型的 size() 输出的是无符号整数，而我们申明的 `i, j`是有符号整数**！如果直接写`while (i < target.size() && j < pattern.size())`，那么在无符号整数于带符号整数比较的过程中，**某些编译器会将带符号整数强转为无符号整数**。例如当 `j= -1`的时候，如果被转为了无符号整数，那么`j < pattern.size()`可能输出的是`false`而不是`true`，程序可能提前终止！

  ```c
     // 利用 next 数组进行匹配
      bool isMatch(string target, string pattern) {
  
          // 分配模式字符串 next 数组的空间
          int *next = new int[pattern.size() + 1]();
  
          // 获取模式字符串的 next 数组
          getNext(pattern, next);
  
          // 输出 next 数组
          /*
          for (int i = 0; i < pattern.size() + 1; ++i) {
              cout << next[i] << '\t';
          }
          cout << endl;
          */
  
          // 利用 next 数组匹配主串
          int i = 0, j = 0;
          int len_t = target.size(), len_p = pattern.size();
          while (i < len_t && j < len_p) {
              if (j == -1 || target[i] == pattern[j]) {
                  i++;
                  j++;
              } else {
                  j = next[j];
              }
          }
  
          bool res = (j == pattern.size());
  
          // 删除 next 数组空间
          delete[] next;
  
          // 返回结果
          return res;
      }
  
  ```

### 1.3 KMP 代码检验

* 主串：`abcadaabababc`，模式串：`abababc`。得到**模式串**的`next`数组为：`-1 0 0 1 2 3 4 0`

  ```c
  // 执行代码
  bool res = s.isMatch("abcadaabababc", "abababc");
  if (res) {
      cout << "matched" << endl;
  } else {
      cout << "not matched" << endl;
  }
  
  // 执行输出
  matched
  ```
  
  
  

## 三、参考链接

* [1] 天勤公开课：KMP算法易懂版 | [[B站]](https://www.bilibili.com/video/BV1jb411V78H?from=search&seid=14079601092891626322)

* [2] 代码随想录：帮你把KMP算法学个通透！（理论篇）| [[B站]](https://www.bilibili.com/video/BV1PD4y1o7nd)
* [3] 代码随想录：帮你把KMP算法学个通透！（求next数组代码篇）| [[B站]](https://www.bilibili.com/video/BV1M5411j7Xx?from=search&seid=14079601092891626322)

* [4] 如何更好地理解和掌握 KMP 算法? | [[知乎]](https://www.zhihu.com/question/21923021)