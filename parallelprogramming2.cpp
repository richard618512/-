#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <immintrin.h>  // 包含 AVX2 指令集

using namespace std;

const int n = 64;
int a[n], sum;

void recur_simd(int* a, int n) {
    if (n == 1) return;

    while (n > 1) {
        int half_n = n / 2;
        int i = 0;

        // 使用 AVX2 进行 SIMD 加法
        for (; i + 7 < half_n; i += 8) {
            __m256i v1 = _mm256_loadu_si256((__m256i*)&a[i]);             // 加载前半部分
            __m256i v2 = _mm256_loadu_si256((__m256i*)&a[n - i - 8]);     // 加载后半部分
            __m256i v_sum = _mm256_add_epi32(v1, v2);                     // SIMD 加法
            _mm256_storeu_si256((__m256i*)&a[i], v_sum);                  // 存回结果
        }

        // 处理剩余的元素
        for (; i < half_n; i++) {
            a[i] += a[n - i - 1];
        }

        n = half_n;  // 更新 n 进行下一轮
    }
}

int main() {
    long long head, tail, freq;

    for (int k = 2; k <= n; k *= 2) {
        for (int i = 0; i < k; i++) {
            a[i] = 1;  // 初始化数组
        }

        QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
        QueryPerformanceCounter((LARGE_INTEGER*)&head);

        // 使用 SIMD 进行求和
        recur_simd(a, k);
        sum = a[0];

        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        cout << "n=" << k << " time:" << (tail - head) * 1000.0 / freq << "ms" << endl;
    }

    return 0;
}

