#include "signal_processing.h"
void print_vector(vsip_vview_f* vec, int length) {
    for (int i = 0; i < length; i++) {
        printf("%f ", vsip_vget_f(vec, i));
    }
    printf("\n");
}

void print_complex_vector(vsip_cvview_f* vec, int length) {
    for (int i = 0; i < length; i++) {
        vsip_cscalar_f sample = vsip_cvget_f(vec, i);
        printf("%f + %fi ", sample.r, sample.i);
    }
    printf("\n");
}
// 封装的汉明窗函数，返回一个VSIP的向量视图(vview_f*)

vsip_vview_f* vsip_vcreate_hamming_f(vsip_length n, vsip_memory_hint hint) {
    vsip_vview_f* window = vsip_vcreate_f(n, hint);
    if (window == NULL) {
        // 显式处理错误情况，打印错误信息或日志，然后返回
        fprintf(stderr, "Error: Failed to create vector view in vsip_vcreate_hamming_f.\n");
        return NULL; // 明确返回NULL
    }
    

    vsip_block_f* block = vsip_vgetblock_f(window);
    if (block == NULL) {
        fprintf(stderr, "Error: Failed to get block from vector view.\n");
        vsip_vdestroy_f(window);
        return NULL; // 明确返回NULL
    }
  

    for (vsip_index i = 0; i < n; ++i) {
        vsip_scalar_f value = vsip_vget_f(window, i); // 获取向量中的第i个元素
        //printf("Value at index %d before update: %f\n", i, value);
        value = 0.5f - 0.5f * cosf(2.0f * M_PI * (i + 0.5f) / n); // 更新元素值
        //printf("Calculated Hamming value at index %d: %f\n", i, value);
        vsip_vput_f(window, i, value); // 更新向量中的元素
        //printf("Value at index %d after update: %f\n", i, vsip_vget_f(window, i));
    }

    vsip_blockdestroy_f(block);
 
    return window; // 成功创建并填充数据后返回window
}







void generate_gaussian_noise(vsip_vview_f* noise_vec, int length) {
    // 创建一个随机数生成器的状态
    vsip_randstate* rand_state = vsip_randcreate(0, 1, 0, VSIP_NPRNG);
    // 使用 vsip_vrandn_f 函数生成高斯噪声
    vsip_vrandn_f(rand_state, noise_vec);
    // 销毁随机数生成器的状态
    vsip_randdestroy(rand_state);
}
// 使用vsip库的函数添加高斯白噪声
void vsip_vsmul_f(vsip_vview_f* vector, float scale_factor, vsip_vview_f* result) {
    vsip_length length = vsip_vgetlength_f(vector);
    for (vsip_index i = 0; i < length; ++i) {
        vsip_vput_f(result, i, vsip_vget_f(vector, i) * scale_factor);
    }
}
void add_gaussian_noise(vsip_vview_f* signal_vec, vsip_vview_f* noise_vec, float snr_db) {
    // 计算信号的功率
    float signal_power = vsip_vsumsqval_f(signal_vec); // 平方和
    signal_power /= vsip_vgetlength_f(signal_vec); // 平均功率

    // 根据信噪比计算噪声功率
    float noise_power = signal_power / powf(10.0f, snr_db / 10.0f);    // 转换算术
    float noise_std = sqrtf(noise_power); // 标准偏差

    // 噪声向量缩放以匹配所需的信噪比
    vsip_vsmul_f(noise_vec, noise_std, noise_vec);

    // 创建一个新的向量视图来存储加法的结果
    vsip_vview_f* result_vec = vsip_vcreate_f(vsip_vgetlength_f(signal_vec), VSIP_MEM_NONE);
    if (result_vec == NULL) {
        // 处理错误情况，例如内存分配失败
        return;
    }

    // 将噪声加到信号向量，并将结果存储在 result_vec 中
    vsip_vadd_f(signal_vec, noise_vec, result_vec);

    // 将结果复制回 signal_vec
    vsip_vcopy_f_f(result_vec, signal_vec);

    // 释放 result_vec
    vsip_valldestroy_f(result_vec);
}




// 根据距离计算振幅值
float calculateAmplitude(float distance) {
    // 这里假设振幅与距离成反比，可以根据实际需求调整公式
    return 10.0f / (distance * distance);
}

void generate_radar_echo(vsip_vview_f* signal_vec, int length) {
    if (signal_vec == NULL || length <= 0) {
        return; // 防止无效参数
    }

    const float timeResolution = PULSE_WIDTH / (float)(length - 1);
    const float distance1 = 600.0f; // 第一个目标距离
    const float distance2 = distance1 + 600.0f; // 第二个目标距离

    vsip_vfill_f(0.0f, signal_vec); // 初始化信号向量



    print_vector(signal_vec, length);

    // 计算每个目标的回波信号
    for (int targetIndex = 0; targetIndex < 2; targetIndex++) {
        float delay = calculate_delay(targetIndex == 0 ? distance1 : distance2); // 获取当前目标的延时
        float amplitude = calculateAmplitude(targetIndex == 0 ? distance1 : distance2); // 根据距离计算振幅值

        // 创建目标回波信号
        vsip_vview_f* echoSignal = vsip_vcreate_f(length, VSIP_MEM_NONE);
        if (echoSignal == NULL) {
            fprintf(stderr, "Error: Failed to create echo signal vector.\n");
            return;
        }

        // 初始化为余弦信号
    for (int i = 0; i < length; i++) {
    float time = i * timeResolution;
    float phase = 2 * M_PI * SPEED_OF_LIGHT * delay / timeResolution;
    float value = amplitude * cos(2 * M_PI * SPEED_OF_LIGHT * time / timeResolution - phase);
    vsip_vput_f(echoSignal, i, value);
}

        // 将回波信号叠加到总信号上
        vsip_vadd_f(signal_vec, echoSignal, signal_vec);

        // 释放 echoSignal
        vsip_vdestroy_f(echoSignal);
    }

    // 创建噪声向量并生成噪声
    vsip_vview_f* noise_vec = vsip_vcreate_f(length, VSIP_MEM_NONE);
    if (noise_vec == NULL) {
        fprintf(stderr, "Error: Failed to create noise vector.\n");
        return;
    }
    generate_gaussian_noise(noise_vec, length);
    // 添加高斯白噪声使得信噪比为0dB
    add_gaussian_noise(signal_vec, noise_vec, 0.0f);

    // 释放 noise_vec
    vsip_vdestroy_f(noise_vec);
}



// 计算距离对应的延迟时间，单位为秒
float calculate_delay(float distance) {
    return distance / SPEED_OF_LIGHT;
}

// 设计一个希尔特变换滤波器，长度不超过10阶
void design_hilbert(vsip_vview_f* filter, int length) {
    if (length > 10) {
        length = 10; // 限制滤波器阶不超过10
    }

    // 创建一个长度为length的复数向量用于存放希尔伯特变换系数
    vsip_cvview_f* hilbertCoeff = vsip_cvcreate_f(length, VSIP_MEM_NONE);
    if (hilbertCoeff == NULL) {
        fprintf(stderr, "Error: Failed to create Hilbert coefficients vector.\n");
        return;
    }

    // 填充滤波器系数
    for (int n = 1; n <= length / 2; n++) {
        float coeff = 2.0f / (M_PI * n); // 希尔伯特变换系数
        vsip_cscalar_f complexCoeff = {coeff, 0.0f};
        vsip_cvput_f(hilbertCoeff, n, complexCoeff);
        vsip_cvput_f(hilbertCoeff, length - n, complexCoeff); // 对称填充
    }

    // 设置偶数索引（不包括0）的系数为0
    for (int i = 2; i < length; i += 2) {
        vsip_cscalar_f zeroCoeff = {0.0f, 0.0f};
        vsip_cvput_f(hilbertCoeff, i, zeroCoeff);
    }

    vsip_cscalar_f dcCoeff = {1.0f, 0.0f}; // 直流分量系数设为1
    vsip_cvput_f(hilbertCoeff, 0, dcCoeff);

    // 将复数向量的实部复制到实数滤波器向量
    for (int i = 0; i < length; ++i) {
        vsip_cscalar_f coeff = vsip_cvget_f(hilbertCoeff, i);
        vsip_vput_f(filter, i, coeff.r);
    }

    vsip_cvdestroy_f(hilbertCoeff); // 清理系数向量
}

Targets detect_targets(vsip_vview_f* signal, int length) {
    Targets result = {0, 0.0f}; // 初始化结果结构体

    if (signal == NULL || length <= 0) {
        return result; // 参数错误
    }

    // 找到峰值
    int peakIndices[2]; // 假设最多有2个峰值
    int numPeaks = 0;
    for (int i = 0; i < length; i++) {
        // 检查当前元素是否大于前一个和后一个元素
        if (i == 0) { // 如果是第一个元素
            if (vsip_vget_f(signal, i) >= PEAKED_THRESHOLD &&
                vsip_vget_f(signal, i) >= vsip_vget_f(signal, i + 1)) {
                if (numPeaks < 2) {
                    peakIndices[numPeaks++] = i;
                    printf("Peak detected at index %d with value %.6f\n", i, vsip_vget_f(signal, i));
                }
            }
        } else if (i == length - 1) { // 如果是最后一个元素
            if (vsip_vget_f(signal, i) >= PEAKED_THRESHOLD &&
                vsip_vget_f(signal, i) >= vsip_vget_f(signal, i - 1)) {
                if (numPeaks < 2) {
                    peakIndices[numPeaks++] = i;
                    printf("Peak detected at index %d with value %.6f\n", i, vsip_vget_f(signal, i));
                }
            }
        } else { // 如果是中间的元素
            if (vsip_vget_f(signal, i) >= PEAKED_THRESHOLD &&
                vsip_vget_f(signal, i) >= vsip_vget_f(signal, i - 1) &&
                vsip_vget_f(signal, i) >= vsip_vget_f(signal, i + 1)) {
                if (numPeaks < 2) {
                    peakIndices[numPeaks++] = i;
                    printf("Peak detected at index %d with value %.6f\n", i, vsip_vget_f(signal, i));
                }
            }
        }
    }

    // 计算目标数量和距离
    result.num_targets = numPeaks;
    if (result.num_targets == 2) {
        result.distance = 20.0*(peakIndices[1] - peakIndices[0]) * SPEED_OF_LIGHT / (2.0f * ADC_RATE);
    }

    return result;
}