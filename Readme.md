# 华南理工大学软件工程2016级卓越班操作系统实验

## 编译

以Ubuntu系统为例

```
mkdir build
cd build
cmake ..
make
```

然后应该可以在build目录里找到可执行文件。

## 备注

* Lab2代码涉及Linux的进程控制函数，没法在Windows上运行。Lab3、Lab4的代码应该可以在Windows上运行。
* Lab2的process程序的锁在WSL(Windows Subsystem for Linux)上没有预期的效果，这可能是WSL的bug。
