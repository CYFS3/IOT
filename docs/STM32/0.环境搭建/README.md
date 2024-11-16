# ·环境搭建

## 一.keil环境搭建

### 1.安装Keil5 MDK

[Keil5资料下载链接](https://jiangxiekeji.com/download.html)

(1)打开 **Keil5 MDK** 文件夹，接着双击 **MDK524a.EXE** 文件开始安装软件

(2)点击next![1](./picture/1.png)

(3)点击同意，点击next![2](./picture/2.png)

(4)这里默认为放在C盘，如果你要放在C盘的话那就不用更改路径，如果要更改路径到D盘或者E盘则点击旁边的Browse![2024-10-04 154712](./picture/3.png)

(5)这里最好**在D盘或者E盘的最外层创建一个文件夹用于专门存放**（注意：文件名和文件路径都最好不要带有特殊字符和中文名称）![4](./picture/4.png)

(6)点击next![2024-10-04 154859](./picture/5.png)

(7)这里是个人信息随意填写（注意：最好不要使用中文）![2024-10-04 154911](./picture/6.png)

(8)这个时间比较久，耐心等待一会，安装之后会弹出一个窗口提示是否安装ULINK的驱动，这里我们点是就行了![2024-10-04 154925](./picture/7.png)



(9)**不要勾选**Show Release Notes，最后Finish![1](./picture/8.png)

(10)这边现在还不需要配置![1](./picture/9.png)

(11)![1](./picture/10.png)



### 2.安装器件支持包

（1）我们介绍的是离线安装

![1](./picture/11.png)

（2）双击该文件![1](./picture/12.png )

（3）直接点击next![1](./picture/13.png)

（4）![1](./picture/14.png)

（5）点击Finish即可![1](./picture/15.png)

（6）打开keil软件

![1](./picture/16.png)

（7）新建一个新的工程

![1](./picture/17.png)

（8）随便取一个名字

![1](./picture/18.png)

（9）如果软件支持包下载成功则可以看到如图显示![1](./picture/19.png)

![1](./picture/20.png)

### 3.软件注册

(1)先**右键**然后**以管理员的身份运行**

![1](./picture/21.png)

(2)点击**File**然后点击**License Management**

![1](./picture/22.png)

(3)**复制CID**

![1](./picture/23.png)

(4)解压keygen_new2032.zip![1](./picture/24.png)

（5）解压完打开文件夹然后点进去，如果解压完以后文件夹里面没有内容则可能是被电脑中的杀毒软件给清除掉了，关闭杀毒软件即可![1](./picture/25.png)

（6）

![1](./picture/26.png)

![1](./picture/27.png)

![1](./picture/28.png)

![1](./picture/29.png)



###  4.安装STLINK驱动

(1)首先我们将Stlink插到电脑上

![1](./picture/30.png)

(2)**右键**此电脑**点击属性**

![1](./picture/31.png)

(3)进入**设备管理器**，点击**其他设备**，如果STM32上有若图所示的黄色标识则你的电脑没有安装Stlink的驱动，如果没有标识则已安装![1](./picture/32.png)

(4)打开文件夹 ***keil5 >ARM>STLink>USBSriver***

![1](./picture/33.png)

(5)

![1](./picture/34.png)

![1](./picture/35.png)

![1](./picture/36.png)

(6)安装成功

![1](./picture/37.png)



### 5.安装USB转串口驱动

(1)插上USB

![1](./picture/38.png)

![1](./picture/39.png)

（2）打开下载的文件夹：教程>STM32入门教程>资料![1](./picture/40.png)

(3)根据如图所示的路径找到CH341….![1](./picture/41.png)

（4）然后就是安装![1](./picture/42.png)

![1](./picture/43.png)

![1](./picture/45.png)

（5）安装成功后![1](./picture/46.png)

## 二.VSCode下载

[VScode安装包,提取码：bite](https://pan.baidu.com/s/1DrROJ2IuUoyreqnnBVldDg)

(1)**下载文件并解压**![1](./picture/47.png)

(2)**双击**进入安装

![1](./picture/48.png)

![1](./picture/49.png)



（3）**和下载Keil一样改变软件存在路径**

![1](./picture/50.png)

![1](./picture/51.png)



（4）**全部勾选**

![1](./picture/52.png)

![1](./picture/53.png)

![1](./picture/54.png)

![1](./picture/55.png)

（5）打开**VScode**

![1](./picture/56.png)

（6）然后下面会**弹出一个框框点击即可**

![1](./picture/57.png)

（7）解压该文件

![1](./picture/58.png)

（8）压缩完以后的样子

![1](./picture/59.png)

（9）**点击路径**![1](./picture/60.png)

（10）在**电脑上找到该图标**点击进去

![image-20241005174036492](./picture/image-20241005174036492.png)

（11）搜索：编辑系统环境变量![1](./picture/61.png)

![1](./picture/62.png)

![1](./picture/63.png)

![1](./picture/64.png)

![1](./picture/65.png)

（12）**返回文件路径**![1](./picture/66.png)

（13）找到***gcc.exe***![1](./picture/67.png)

(14)打开**VScode**![1](./picture/68.png)

（15）***创建新文件***![1](./picture/69.png)

（16）创建的新文件**vscodepro**![1](./picture/70.png)

![1](./picture/71.png)

![1](./picture/72.png)

(17)输入**changworld.c**

![1](./picture/73.png)

(18)**编写一个C语言代码**

![1](./picture/74.png)

（19）

![1](./picture/75.png)

（20）打开刚刚要**找到的gcc.exe文件**，然后复制路径 

![1](./picture/78.png)

![1](./picture/79.png)

（21）**回到C代码**

![1](./picture/80.png)

![1](./picture/81.png)

![1](./picture/82.png)

（22）点击**完成以后右上角**会出现

![1](./picture/83.png)

![1](./picture/84.png)

![1](./picture/85.png)

![1](./picture/86.png)

## 三.VScode与keil软件结合使用配置

1.

![1](./picture/87.png)

2.

![1](./picture/88.png)

3.

![1](./picture/89.png)

4.

!(C:/Users/56515/Pictures/Screenshots/屏幕截图 2024-11-07 175535.png)

![1](./picture/90.png)

5.![1](./picture/91.png)

6.注意创建文件必须在keil软件中创建才能在VScode中使用，当然在VScode中修改的内容需要在VScode中保存才能在Keil软件中显示

![1](./picture/92.png)

7.

![1](./picture/933.png)

8.处理乱码



![1](./picture/94.png)

9.输入Encoding

![1](./picture/95.png)

