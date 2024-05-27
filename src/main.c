#include <stdio.h>
#include <stdlib.h>
#include "signal_processing.h"

int main() {
    vsip_init(NULL);
    int signalLength = 10;
    vsip_vview_f* signalVec = vsip_vcreate_f(signalLength, VSIP_MEM_NONE);
    vsip_vview_f* hFilter = vsip_vcreate_f(signalLength, VSIP_MEM_NONE);
    vsip_cvview_f* complexSignal = vsip_cvcreate_f(signalLength, VSIP_MEM_NONE);
    vsip_cvview_f* filteredComplex = vsip_cvcreate_f(signalLength, VSIP_MEM_NONE);

    if (signalVec == NULL || hFilter == NULL || complexSignal == NULL || filteredComplex == NULL) {
        fprintf(stderr, "Failed to create one or more vectors.\n");
        vsip_valldestroy_f(signalVec);
        vsip_valldestroy_f(hFilter);
        vsip_cvalldestroy_f(complexSignal);
        vsip_cvalldestroy_f(filteredComplex);
        return 1;
    }

    printf("Calling generate_radar_echo...\n");
    generate_radar_echo(signalVec, signalLength);
    
    printf("Radar echo signal:\n");
    print_vector(signalVec, signalLength);

    printf("Calling design_hilbert...\n");
    design_hilbert(hFilter, signalLength);
    printf("Hilbert filter:\n");
    print_vector(hFilter, signalLength);

    // 应用希尔伯特滤波器将实信号转换成复信号
    for (int i = 0; i < signalLength; i++) {
        vsip_cscalar_f complexSample;
        complexSample.r = vsip_vget_f(signalVec, i);
        complexSample.i = vsip_vget_f(hFilter, i);
        vsip_cvput_f(complexSignal, i, complexSample);
    }
    printf("Complex signal after Hilbert transform:\n");
    print_complex_vector(complexSignal, signalLength);

    // 进行脉冲压缩
    for (int i = 0; i < signalLength; i++) {
        vsip_cscalar_f complexSample = vsip_cvget_f(complexSignal, i);
        vsip_cscalar_f filterSample = vsip_cvget_f(complexSignal, i);
        vsip_cscalar_f result;
        result.r = complexSample.r * filterSample.r - complexSample.i * filterSample.i;
        result.i = complexSample.r * filterSample.i + complexSample.i * filterSample.r;
        vsip_cvput_f(filteredComplex, i, result);
    }
    printf("Filtered complex signal after pulse compression:\n");
    print_complex_vector(filteredComplex, signalLength);

    // 检测目标
    Targets detected = detect_targets(signalVec, signalLength);
    int numTargets = detected.num_targets;
    float distance = detected.distance;

    printf("Detected targets: %d, Distance to nearest target: %.2f meters\n", numTargets, distance);

    vsip_valldestroy_f(signalVec);
    vsip_valldestroy_f(hFilter);
    vsip_cvalldestroy_f(complexSignal);
    vsip_cvalldestroy_f(filteredComplex);

    vsip_finalize(NULL);
    return 0;
}