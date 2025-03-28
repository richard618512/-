#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <immintrin.h>  // SIMD 指令集
#include <omp.h>        // OpenMP 并行计算
using namespace std;

const int n = 10000;
int a[n][n], b[n], sum[n];

int main() {
    long long head, tail, freq;
    
    // 初始化矩阵 a 和向量 b
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = i + j;
        }
        b[i] = i;  // 简单初始化
    }

    int gap = 100;
    for (int k = 100; k <= n; k += gap) {
        // 重置 sum 数组
        for (int i = 0; i < k; i++) {
            sum[i] = 0;
        }

        QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
        QueryPerformanceCounter((LARGE_INTEGER*)&head);

        // 并行计算矩阵-向量乘法
        #pragma omp parallel for
        for (int i = 0; i < k; i++) {
            __m256i sum_vec = _mm256_setzero_si256();  // 初始化 SIMD 向量
            int j = 0;

            // 使用 AVX2 进行 SIMD 计算，每次处理 8 个元素
            for (; j + 7 < k; j += 8) {
                __m256i a_vec = _mm256_loadu_si256((__m256i*)&a[j][i]);
                __m256i b_vec = _mm256_loadu_si256((__m256i*)&b[j]);
                __m256i prod = _mm256_mullo_epi32(a_vec, b_vec); // SIMD 乘法
                sum_vec = _mm256_add_epi32(sum_vec, prod); // SIMD 加法
            }

            // 将 SIMD 结果存回普通变量
            int temp[8];
            _mm256_storeu_si256((__m256i*)temp, sum_vec);
            for (int t = 0; t < 8; t++) {
                sum[i] += temp[t];
            }

            // 处理剩余元素
            for (; j < k; j++) {
                sum[i] += a[j][i] * b[j];
            }
        }

        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        cout << "n=" << k << " time: " << (tail - head) * 1000.0 / freq << "ms" << endl;

        if (k == 1000) gap = 1500;
        if (k == 5500) gap = 4500;
    }

    return 0;
}

