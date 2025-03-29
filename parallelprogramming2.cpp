#include <iostream>
#include <windows.h>

using namespace std;

const int n = 64;
int a[n], sum;

// 优化版：循环展开 + 迭代方式
void optimized_reduce(int *a, int n) {
    while (n > 1) {
        int half = n / 2;
        int i = 0;

        // **循环展开 4 次**
        for (; i + 3 < half; i += 4) {
            a[i]     += a[n - i - 1];
            a[i + 1] += a[n - i - 2];
            a[i + 2] += a[n - i - 3];
            a[i + 3] += a[n - i - 4];
        }
        // 处理剩余部分
        for (; i < half; i++) {
            a[i] += a[n - i - 1];
        }
        n = half;
    }
}

int main() {
    long long head, tail, freq;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

    for (int k = 2; k <= n; k *= 2) {
        for (int i = 0; i < k; i++) {
            a[i] = 1;
        }

        QueryPerformanceCounter((LARGE_INTEGER*)&head);
        
        // 调用优化后的归约函数
        optimized_reduce(a, k);
        sum = a[0];

        QueryPerformanceCounter((LARGE_INTEGER*)&tail);
        cout << "n=" << k << " time:" << (tail - head) * 1000.0 / freq << "ms" << endl;
    }

    return 0;
}
