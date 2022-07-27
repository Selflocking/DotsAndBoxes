# Heap Overflow DotsAndBoxes

## 使用方法
首先确保你的MinGW使用MSVCRT，gcc版本最好为11.2。如果不确定，请使用这个[MinGW-w64](https://ysod.azurewebsites.net/%E5%BC%80%E5%8F%91/MinGW/winlibs-x86_64-posix-seh-gcc-11.2.0-mingw-w64msvcrt-10.0.0-r1.zip)或从群文件下载
使用方法: 
1. 将MinGW解压到某个地方，进入`/bin`目录，复制路径。
2. 打开设置，进入系统-系统信息-高级系统设置，选择环境变量，在打开的窗口中双击用户变量里的`Path`,点击新建，把复制的路径粘贴进去，并把它上移到顶部。
3. `win`+`r`输入`cmd`打开命令行，输入`gcc -v`，如果输出的信息中`COLLECT_LTO_WRAPPER=`大致为你复制的路径即为成功。
### 不使用CMake
#### VS Code
1. git clone https://gitee.com/YunShu007/dots-and-boxes.git
2. 进入项目目录，新建`build`文件夹，将`3rdparty/SFML/bin`文件夹中的`dll`文件复制到刚刚新建的`build`文件夹中, 将`res`目录复制进`build`目录中
3. 使用VS Code打开项目，请保证VSCode仅打开项目文件夹，而不是打开项目文件夹的父文件夹
4. 新建`.vscode`文件夹,在`.vscode`里新建文件`tasks.json`和`launch.json`.
tasks.json:
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "g++ src/*.cpp src/AI/*.cpp -o build/game.exe -I ./3rdparty/SFML/include -L./3rdparty/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system"
        }
    ]
}
```
如果你使用的是官方C++插件，launch.json:
```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "(gdb) 启动",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}\\build\\game.exe",
      "cwd": "${workspaceFolder}\\build",
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "gdb.exe",
      "preLaunchTask": "build"
    }
  ]
}
```
#### 命令行编译
1. git clone https://gitee.com/YunShu007/dots-and-boxes.git
2. 进入项目目录，新建`build`文件夹，将`3rdparty/SFML/bin`文件夹中的`dll`文件复制到刚刚新建的`build`文件夹中，将`res`目录复制进`build`目录中
3. 回到项目目录，打开命令行执行
```bash
g++ src/*.cpp src/AI/*.cpp -o build/game.exe -I ./3rdparty/SFML/include -L./3rdparty/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system
```
4. 进入`build`文件夹，即可看到`game.exe`, 执行即可。
### 使用CMake (推荐)
首先安装[CMake](https://github.com/Kitware/CMake/releases/download/v3.24.0-rc4/cmake-3.24.0-rc4-windows-x86_64.msi)
#### VS Code
1. git clone https://gitee.com/YunShu007/dots-and-boxes.git
2. 安装[CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)插件
3. 重启VS Code
4. 重新打开会让你选择工具,选择`GCC 11.2.0 X86_64-w64-mingw32-make`
5. 如果上一步不小心选错，按`Ctrl`+`Shift`+`P`打开指令栏，输入kit,点击`CMake: 选择工具包`重新选择
6. 成功后点击VS Code下方状态栏上的运行按钮即可运行，如需调试请安装官方插件[C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
#### Clion
1. 打开Clion，选择从VCS导入，输入https://gitee.com/YunShu007/dots-and-boxes.git
2. 等待加载，完成后会弹出配置cmake的窗口，直接点确定即可。

> Clion自带Cmake和符合要求的MinGW,所以可以开箱即用
## 开发指南
### AI设计
入口函数为`void UCT(Board *board, int player)`,在此函数中更改AI逻辑

### 棋盘
棋盘是11x11大小,有点(DOT),横线(HENG),竖线(SHU),被占据的线(OCCLINE),格子(BOX),先手黑方格子(BLACK_BOX),后手白方格子(WHITE_BOX).

棋盘使用二位数组int8_t map[11][11]储存，横纵坐标皆为偶数时点，皆为奇数时格子，横偶竖奇是横线，横奇竖偶是竖线。

判定一个坐标是否为未被占据的线可以使用if(map[i][j]!=OCCLINE)或者if(map[i][j]==HENG||map[i][j]==SHU)

判定一个格子是否被占可以使用if(map[i][j]==BOX)


### 坐标
坐标LOC定义为`std::pair<int,int>`
