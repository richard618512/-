#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <immintrin.h>  // ���� AVX2 ָ�

using namespace std;

const int n = 64;
int a[n], sum;

void recur_simd(int* a, int n) {
    if (n == 1) return;

    while (n > 1) {
        int half_n = n / 2;
        int i = 0;

        // ʹ�� AVX2 ���� SIMD �ӷ�
        for (; i + 7 < half_n; i += 8) {
            __m256i v1 = _mm256_loadu_si256((__m256i*)&a[i]);             // ����ǰ�벿��
            __m256i v2 = _mm256_loadu_si256((__m256i*)&a[n - i - 8]);     // ���غ�벿��
            __m256i v_sum = _mm256_add_epi32(v1, v2);                     // SIMD �ӷ�
            _mm256_storeu_si256((__m256i*)&a[i], v_sum);                  // ��ؽ��
        }

        // ����ʣ���Ԫ��
        for (; i < half_n; i++) {
            a[i] += a[n - i - 1];
        }

        n = half_n;  // ���� n ������һ��
    }
}

int main() {
    long long head, tail, freq;

    for (int k = 2; k <= n; k *= 2) {
        for (int i = 0; i < k; i++) {
            a[i] = 1;  // ��ʼ������
        }

        QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
        QueryPerformanceCounter((LARGE_INTEGER*)&head);

        // ʹ�� SIMD �������
        recur_simd(a, k);
        sum = a[0];

        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        cout << "n=" << k << " time:" << (tail - head) * 1000.0 / freq << "ms" << endl;
    }

    return 0;
}

