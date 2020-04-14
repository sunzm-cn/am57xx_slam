# 项目说明
本项目旨在在尽可能充分利用TI am57xx芯片中Cortex-A15、C66x DSP、EDMA等片上资源的前提下，实现一个简易的视觉里程计（SLAM前端）的库以及一个简单的测试程序，目前还没有实现SLAM后端的功能。本项目初始提交由[《视觉SLAM十四讲》第一版第九讲0.4版本](https://github.com/gaoxiang12/slambook/tree/master/project/0.4)的代码复制而来，主要对源代码作了以下增改：
1. 增加了Singleton.hpp，实现了一个线程安全的单例模式模板类，并且在Config类中继承了这个类；
2. 将visual_odometry.cpp中add_fram函数基于switch语句的状态机改为了基于boost库中的提供的状态机模板类；
3. 将run_vo.cpp测试程序与内部头文件和类解耦；
4. 修改CMakeLists.txt使其支持使用am57xx processor linux sdk进行编译，并且在am57xx芯片上运行。

# 源码结构
include/myslam：视觉里程计的头文件<br>
include/utils：与视觉里程计无关的头文件，如单例模式模板类<br>
src：视觉里程计的源代码<br>
test：存放了测试程序run_vo的源代码，以及用于测试的param.yaml配置文件，测试程序使用的TUM数据集需要另外下载<br>

# 编译项目
## 准备开发环境
1. 在Ubuntu Host上安装Eigen库
```shell
sudo apt-get install libeigen3-dev
```
2. 安装am57xx processor linux sdk，默认安装在/opt目录，如安装在其它目录，请对应修改顶层CMakeLists.txt文件
3. 编译OpenCV库（SDK中TI已经交叉编译好了3.1.0版本的OpenCV）
```shell
sudo apt-get install build-essential libgtk2.0-dev libvtk5-dev libjpeg-dev libtiff4-dev libjasper-dev libopenexr-dev libtbb-dev
wget https://codeload.github.com/opencv/opencv/zip/3.1.0
unzip 3.1.0
cd opencv-3.1.0
mkdir build-x86
cd build-x86
cmake ..
make
sudo make install
```
4. 交叉编译Sophus库
```shell
git clone https://github.com/strasdat/Sophus.git
cd Sophus
git checkout a621ff2e56c56c839a6c40418d42c3c254424b5c
mkdir build-x86 build-armv7a
cd build-x86
cmake ..
make
sudo cp libSophus.so /usr/local/lib
sudo cp -r ../sophus /usr/local/include
source /opt/ti-processor-sdk-linux-am57xx-evm-03.01.00.06/linux-devkit/environment-setup
cd ../build-armv7a
cmake ..
make
sudo cp libSophus.so /opt/ti-processor-sdk-linux-am57xx-evm-03.01.00.06/linux-devkit/sysroots/armv7ahf-neon-linux-gnueabi/usr/lib
```
5. 交叉编译G2O库
```shell
sudo apt-get install libqt4-dev qt4-qmake libqglviewer-dev libsuitesparse-dev libcxsparse3.1.2 libcholmod2.1.2
git clone https://github.com/RainerKuemmerle/g2o.git
cd g2o
git checkout ff647bd7537860a2b53b3b774ea821a3170feb13
mkdir build-x86 build-armv7a
cd build-x86
cmake ..
make
sudo cp ../lib/* /usr/local/lib
sudo cp -r ../g2o /usr/local/include
source /opt/ti-processor-sdk-linux-am57xx-evm-03.01.00.06/linux-devkit/environment-setup
cd ../build-armv7a
cmake ..
make
sudo cp ../lib/* /opt/ti-processor-sdk-linux-am57xx-evm-03.01.00.06/linux-devkit/sysroots/armv7ahf-neon-linux-gnueabi/usr/lib
```
## 编译x86版本
```shell
git clone https://github.com/sunzm-cn/am57xx_slam.git
cd am57xx_slam/
mkdir build-x86
cd build-x86
cmake ..
make
ls ./out
```
## 交叉编译am57xx版本
```shell
git clone https://github.com/sunzm-cn/am57xx_slam.git
cd am57xx_slam/
source /opt/ti-processor-sdk-linux-am57xx-evm-03.01.00.06/linux-devkit/environment-setup
mkdir build-armv7a
cd build-armv7a
cmake ..
make
ls ./out
```

# libmyslam.so使用说明
1. 包含visual_odometry.h头文件；
2. 声明并初始化一个myslam::VO对象，传入param.yaml配置文件的路径作为构造函数的参数；
3. 调用initiate()成员函数初始化对象内部的状态机；
4. 调用add_frame(std::shared_ptr\<cv::Mat\> color, std::shared_ptr\<cv::Mat\> depth, double time_stamp)成员函数传入一帧RGBD图像，进行相机运动估算并且打印结果；
5. 调用is_lost()成员函数判断视觉里程计是否正常工作，若跟丢的特征点数量超过配置文件中设置的数目则函数返回false。

# 测试程序
## 验证通过的环境
开发板：Tronlong TL5728-EasyEVM-A3<br>
Host Linux：Ubuntu 14.04<br>
Linux SDK版本：ti-processor-sdk-linux-am57xx-evm-03.01.00.06-Linux-x86-Install.bin<br>
OpenCV版本：3.1.0<br>
OpenCL版本：1.1.10.3<br>
Eigen版本：3.2.0-8<br>
Sophus版本：https://github.com/strasdat/Sophus/commit/a621ff2e56c56c839a6c40418d42c3c254424b5c<br>
G2O版本：https://github.com/RainerKuemmerle/g2o/commit/ff647bd7537860a2b53b3b774ea821a3170feb13<br>
数据集：https://vision.in.tum.de/rgbd/dataset/freiburg1/rgbd_dataset_freiburg1_desk.tgz<br>
数据集下载页面：https://vision.in.tum.de/data/datasets/rgbd-dataset/download<br>
## 运行run_vo测试程序测试libmyslam.so
run_vo测试程序和libmyslam.so库均位于${CMAKE_BINARY_DIR}/out/目录，run_vo调用libmyslam.so并且通过控制台打印输出每一帧估计出的相机运动的变换矩阵以及各步骤的执行时间等调试信息。执行```./build-x86/out/run_vo ./test/param.yaml /home/charming/Downloads/data```命令运行run_vo测试程序，```./test/param.yaml```为存放TUM数据集相机外参和视觉里程计设置的文件，```/home/charming/Downloads/data```为存放TUM数据集的目录。
