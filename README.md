# 崇实战队 27 赛季算法组梯队考核作答

这是我的算法组梯队考核练习仓库。目前整理了 TASK1～TASK6 的作答，以及六道问答题。TASK7 是进阶选做题，本次未提交。

题目来自 [崇实战队 27 赛季算法组梯队考核仓库](https://github.com/WUST-RM/wust_27_algorithm_assessment)。

## 作答内容

| 题目 | 内容 | 主要文件 |
| --- | --- | --- |
| TASK1 | Linux 终端配置与美化 | [终端截图](<TASK1/屏幕截图 2026-11-08 142241.png>) |
| TASK2 | Bash 脚本、CMake 构建与命令行参数 | `TASK2/run.sh`、`TASK2/src/main.cpp` |
| TASK3 | 网格地图的 DFS 可达区域搜索与 A* 最小代价路径搜索 | `TASK3/main.cpp` |
| TASK4 | 不同图像数据来源的统一处理 | `TASK4/main.cpp` |
| TASK5 | 双电梯调度模拟 | `TASK5/my_scheduler.hpp`、`TASK5/my_scheduler.cpp` |
| TASK6 | 串口帧编码、CRC 校验与流式解码 | `TASK6/protocol.hpp`、`TASK6/protocol.cpp` |

六道问答题的作答见 [ANSWERS.md](ANSWERS.md)。

## 编译与运行

代码主要在 Ubuntu/WSL 环境中练习。运行下面的命令时，先进入本仓库根目录。需要安装 C++ 编译器和 CMake。

TASK3 可以直接编译。程序会从标准输入读取地图，输入格式以题目要求为准。

```bash
g++ -std=c++17 TASK3/main.cpp -o TASK3/task3
./TASK3/task3
```

TASK4～TASK6 使用 CMake。以 TASK6 为例：

```bash
cmake -S TASK6 -B TASK6/build
cmake --build TASK6/build
./TASK6/build/task6
```

运行 TASK4 或 TASK5 时，将命令里的 `TASK6` 换成对应的目录名，最后运行相应的 `task4` 或 `task5`。TASK2 也有独立的 `CMakeLists.txt`，可用相同方式构建，生成的程序位于 `TASK2/app`，与 `TASK2/run.sh` 使用的路径一致。
