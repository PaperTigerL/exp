### README.md

```markdown
## 项目名称：基于飞腾平台的远距离探测系统

### 项目结构
```
远距离探测系统/
├── README.md
├── src/
│   ├── main.c
│   ├── signal_processing.c
│   ├── signal_processing.h
│   ├── utils.c
│   └── utils.h
├── data/
│   ├── input_signal.txt
│   └── output_results.txt
├── docs/
│   ├── 实训报告.doc
│   └── system_design_flowchart.png
├── Makefile
└── tests/
    ├── test_signal_processing.c
    └── test_utils.c
```

### README.md

```markdown
# 基于飞腾平台的远距离探测系统

## 项目简介
本项目旨在基于飞腾教育开发板，利用VSIPL函数库和C/C++编程语言，实现一个远距离探测系统。通过处理雷达信号，实现对目标的检测。

## 目录结构
- `src/` - 源代码目录
  - `main.c` - 主程序入口
  - `signal_processing.c` - 信号处理相关函数实现
  - `signal_processing.h` - 信号处理相关函数声明
  - `utils.c` - 工具函数实现
  - `utils.h` - 工具函数声明
- `data/` - 数据目录
  - `input_signal.txt` - 输入信号数据
  - `output_results.txt` - 输出结果数据
- `docs/` - 文档目录
  - `实训报告.doc` - 实训报告
  - `system_design_flowchart.png` - 系统设计流程图
- `tests/` - 测试目录
  - `test_signal_processing.c` - 信号处理模块的测试代码
  - `test_utils.c` - 工具模块的测试代码
- `Makefile` - 项目构建文件

## 安装与运行
### 环境要求
- 飞腾教育开发板
- VSIPL 函数库
- GCC 编译器

### 构建与运行
1. 使用以下命令构建项目：
    ```sh
    make
    ```

2. 运行程序：
    ```sh
    ./dist_det_system
    ```

3. 查看输出结果：
    输出结果保存在 `data/output_results.txt` 文件中。

## 文件详细描述
### `src/main.c`
主程序入口，负责读取输入信号数据，调用信号处理模块进行处理，并将结果输出到指定文件。

### `src/signal_processing.c`
实现主要的信号处理算法，包括雷达信号的生成、添加噪声、希尔伯特滤波器设计和目标检测等功能。

### `src/signal_processing.h`
声明 `signal_processing.c` 中的函数接口。

### `src/utils.c`
提供一些工具函数，如文件读取、写入等辅助功能。

### `src/utils.h`
声明 `utils.c` 中的工具函数接口。

### `data/input_signal.txt`
存放输入的雷达信号数据，供程序读取和处理。

### `data/output_results.txt`
存放程序处理后的输出结果。

### `docs/实训报告.doc`
详细的实训报告，包含项目背景、目的、要求、设计过程、代码实现和测试结果等内容。

### `docs/system_design_flowchart.png`
系统设计流程图，帮助理解信号处理过程。

### `tests/test_signal_processing.c`
对信号处理模块进行单元测试，确保各函数正确实现。

### `tests/test_utils.c`
对工具函数进行单元测试，确保文件读取和写入功能正常。

## 功能接口描述
### `signal_processing.h`
```c
#ifndef SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_H

// 函数声明
void generate_radar_signal(float* signal, int length);
void add_gaussian_noise(float* signal, int length, float snr);
void design_hilbert_filter(float* filter, int length);
int detect_targets(float* signal, int length, float* distances);

#endif // SIGNAL_PROCESSING_H
```

### `utils.h`
```c
#ifndef UTILS_H
#define UTILS_H

// 函数声明
void read_signal_from_file(const char* filename, float* signal, int* length);
void write_results_to_file(const char* filename, const float* results, int length);

#endif // UTILS_H
```

## 输入输出
### 输入
- 雷达信号数据：从 `data/input_signal.txt` 文件读取。

### 输出
- 处理结果：将检测到的目标距离输出到 `data/output_results.txt` 文件。

## 测试
- 使用 `tests/test_signal_processing.c` 和 `tests/test_utils.c` 进行单元测试，验证各模块功能的正确性。

## 项目实现步骤
1. 搭建开发环境，安装必要的工具和库。
2. 编写 `main.c`，实现主程序逻辑。
3. 实现 `signal_processing.c`，完成信号处理算法。
4. 实现 `utils.c`，提供文件读写功能。
5. 编写 `Makefile`，实现项目的构建和清理。
6. 编写单元测试代码，验证各模块功能。
7. 运行测试，确保程序正确性。
8. 完成实训报告。

希望这份详细的README文档能帮助你顺利实现该项目。
```

### `utils.h`

```c_cpp
#ifndef UTILS_H
#define UTILS_H

// 函数声明
void read_signal_from_file(const char* filename, float* signal, int* length);
void write_results_to_file(const char* filename, const float* results, int length);

#endif // UTILS_H
```

## 输入输出

### 输入

- 雷达信号数据：从 `data/input_signal.txt` 文件读取。

### 输出

- 处理结果：将检测到的目标距离输出到 `data/output_results.txt` 文件。

## 测试

- 使用 `tests/test_signal_processing.c` 和 `tests/test_utils.c` 进行单元测试，验证各模块功能的正确性。

## 项目实现步骤

1. 搭建开发环境，安装必要的工具和库。
2. 编写 `main.c`，实现主程序逻辑。
3. 实现 `signal_processing.c`，完成信号处理算法。
4. 实现 `utils.c`，提供文件读写功能。
5. 编写 `Makefile`，实现项目的构建和清理。
6. 编写单元测试代码，验证各模块功能。
7. 运行测试，确保程序正确性。
8. 完成实训报告。



```c_cpp
#ifndef SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_H

// 函数声明
void generate_radar_signal(float* signal, int length);
void add_gaussian_noise(float* signal, int length, float snr);
void design_hilbert_filter(float* filter, int length);
int detect_targets(float* signal, int length, float* distances);

#endif // SIGNAL_PROCESSING_H
```
