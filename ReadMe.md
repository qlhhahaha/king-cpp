## 说明


1. 每周的练习和作业都存放在对应序号的week0x文件夹中，其中practice存放课堂练习的代码（不作考核但会作业参考），homework 存放课后作业的代码。

2.  最终项目考核代码存放在 ClassExamProject 文件夹中。

3. C++作业提交作业时只需要提交项目的代码文件.h,.cpp，.sln 文件和.vcxproj 文件即可，无需提交编译产生的中间产物（最小编译单元）。

更多详细情况见： https://kdocs.cn/l/cdZi2WxolyLn


### 代码编译教程

1. 首先下载qt5相关的库，下载完后存放到和Develop-Training目录同级目录下，最后解压，解压方式选择解压到当前文件
   操作完毕后, Develop-Train、thirdparty_install两个在同级位置。

    下载链接: https://365.kdocs.cn/l/crFHGQuSLP9Q
    

2. 打开cmd窗口,进入当前位置, 创建build目录，进入后执行相关命令。【Develop-Train、thirdparty_install、build三个在同级目录】
    ```
    mkdir build
    cd build
    ```
4. 使用VS中的编译环境,实际请根据自己安装的VS路径来修改
    ```
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsamd64_x86.bat"
    ```
5. 执行相关构建命令，根据Debug或Release自己选择

-   构建Debug命令
    ```
    cmake  -G "Visual Studio 16 2019"  -A x64 -DCMAKE_BUILD_TYPE=Debug  ../Develop-Training

    msbuild /m Develop-Train.sln /p:Platform=x64 /p:Configuration=Debug
    ```


- 构建Release命令
    ```
    cmake  -G "Visual Studio 16 2019"  -A x64 -DCMAKE_BUILD_TYPE=Release  ../Develop-Training

    msbuild /m Develop-Train.sln /p:Platform=x64 /p:Configuration=Release