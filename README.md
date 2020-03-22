# 项目说明
本项目旨在在尽可能充分利用TI am57xx芯片中Cortex-A15、C66x DSP、EDMA等片上资源的前提下，实现一个简易的视觉里程计（SLAM前端）的库以及一个简单的测试程序，目前还没有实现SLAM后端的功能。本项目初始提交由[《视觉SLAM十四讲》第一版第九章0.4版本](https://github.com/gaoxiang12/slambook/tree/master/project/0.4)的代码复制而来，后续提交对原代码中一些不合理的地方进行了修改，并且增加了一些使该项目能够在嵌入式环境下运行以及加速的代码。

# 源码结构
cmake_modules：顶级CMakeLists.txt文件中find_packages()需要使用的文件<br>
include/myslam：视觉里程计的头文件<br>
src：视觉里程计的源代码<br>
test：存放了测试程序run_vo的源代码，以及用于测试的param.yaml配置文件，测试程序使用的TUM数据集需要另外下载<br>

# 编译项目
## 准备开发环境
1. 在Ubuntu Host上安装Eigen库
```shell
sudo apt-get install libeigen3-dev
```
2. 安装am57xx processor linux sdk，默认安装在/opt目录，如安装在其它目录，请对应修改顶层CMakeLists.txt文件
3. 交叉编译Sophus库
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
4. 交叉编译G2O库
```shell
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

# API说明
待补充

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
run_vo测试程序和libmyslam.so库均位于${CMAKE_BINARY_DIR}/out/目录，run_vo调用libmyslam.so并且通过控制台打印输出中间结果。运行run_vo前请修改param.yaml文件中的TUM数据集路径，然后执行```./build-x86/out/run_vo ./test/param.yaml```命令运行run_vo测试程序，param.yaml中还存放了myslam库中需要使用的视觉里程计参数和与数据集相对应的相机外参。
