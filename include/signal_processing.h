#ifndef SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_H
#include <stdint.h> // 包含固定宽度整型定义
#include <math.h>
#include <stdlib.h> // 包含 srand, RAND_MAX
#include <time.h> // 含 time
#include "vsip.h" // VSIP库的头文件
// 值阈值设置，用于区分噪声和信号
#define PEAKED_THRESHOLD 0.0001f // 增加阈值以更好地区分噪声和信号峰值// 增加阈值以更好地区分噪声和信号峰值
// 定义物理常量
#define SPEED_OF_LIGHT 299792458.0f // 光速，单位：米/秒
#define RAD_FREQUENCY 2e9f // 雷达频率，单位：赫兹
// 定义雷达参数
#define PULSE_WIDTH 7e-6f // 脉冲宽度，单位：秒
#define FREQ_START 222e6f // 线性调频下限截止频率，单位：赫兹
#define BANDWIDTH 6e6f // 带宽，单位：赫兹
#define PHASE 0.0f // 初相位
#define FILTER_BAND_CENTER 225e6f // 带通滤波器中心频率，单位：赫兹
#define FILTER_BANDWIDTH 6e6f // 带通滤波器带宽，单位：赫兹
#define ADC_RATE 20e6f // 采样率，单位：赫兹
typedef struct {
    int num_targets;
    float distance;
} Targets;
// 函数声明
void generate_gaussian_noise(vsip_vview_f* noise_vec, int length);
void add_gaussian_noise(vsip_vview_f* signal_vec, vsip_vview_f* noise_vec, float snr_db);
void generate_radar_echo(vsip_vview_f* signal_vec, int length);
float calculateAmplitude(float distance);
float calculate_delay(float distance);
void design_hilbert(vsip_vview_f* filter, int length);
Targets detect_targets(vsip_vview_f* signal, int length);
void print_vector(vsip_vview_f* vec, int length);
void print_complex_vector(vsip_cvview_f* vec, int length);
#endif // SIGNAL_PROCESSING_H