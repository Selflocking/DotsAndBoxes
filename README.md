# DotsAndBoxs

## 使用方法
### 不使用CMake
1. 下载[官方对战平台](http://computergames.caai.cn/platform/SAU_Game_Platform_2.1.0_r4.zip)
2. git clone https://gitee.com/YunShu007/dots-and-boxs.git
3. 进入项目目录，打开命令行执行`g++ *.cpp`编译，使用`git *.cpp -o file.exe`可以自定义编译出的文件名，使用`g++ *.cpp -static`链接静态库。
4. 打开官方对战平台，加载引擎。
### 使用CMake
#### Clion
1. 打开选择从VCS获取，输入https://gitee.com/YunShu007/dots-and-boxs.git
2. 等待clion加载项目，完成即可用
#### VS Code
1. git clone https://gitee.com/YunShu007/dots-and-boxs.git
2. 安装[CMake](https://cmake.org/download/)
3. 安装[CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools), [clangd(可选)](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd)，[Todo Tree(可选)](https://marketplace.visualstudio.com/items?itemName=Gruntfuggly.todo-tree)，[Doxygen Documentation Generator(可选)](https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen)。
4. 等待加载，完成后在侧边栏CMake选项中点击`生成`即可在`build/`文件夹中生成可执行文件
#### Visual Studio
1. 打开选择克隆存储库，输入https://gitee.com/YunShu007/dots-and-boxs.git
2. 等等加载，完成即可用
## 详细介绍
### 各文件功能
`board.h` 棋盘类

`board.cpp` 棋盘类的具体实现

`define.h` 存放常用宏定义

`fun.h` 存放常用函数

`sutpidAI.h` 准备存放AI相关代码

`main.cpp` 主函数所在文件

`.clang-format` 格式化配置文件

### 棋盘
棋盘是11x11大小,有点(DOT),横线(HENG),竖线(SHU),被占据的线(OCCLINE),格子(BOX),先手黑方格子(BLACK_BOX),后手白方格子(WHITE_BOX).

棋盘使用二位数组int8_t map[11][11]储存，横纵坐标皆为偶数时点，皆为奇数时格子，横偶竖奇是横线，横奇竖偶是竖线。

判定一个坐标是否为未被占据的线可以使用if(map[i][j]!=OCCLINE)或者if(map[i][j]==HENG||map[i][j]==SHU)

判定一个格子是否被占可以使用if(map[i][j]==BOX)


### 坐标
坐标LOC定义为`std::pair<int,int>`
