#include <windows.h>  // Windows API 计时
#include <iostream>

using namespace std;

const int n = 10000;
int a[n][n], b[n], sum[n];

int main() {
    LARGE_INTEGER head, tail, freq;

    // 初始化矩阵 a 和向量 b
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = i + j;  // 行优先存储，提高 Cache 命中率
        }
        b[i] = i;
    }

    int gap = 100;
    for (int k = 100; k <= n; k += gap) {
        // 重置 sum 数组
        for (int i = 0; i < k; i++) {
            sum[i] = 0;
        }

        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&head);

        // 计算矩阵-向量乘法（无多线程）
        for (int i = 0; i < k; i++) {
            int temp_sum = 0;
            int j = 0;

            // 4 次循环展开（Unrolling 4x）
            for (; j + 3 < k; j += 4) {
                temp_sum += a[i][j] * b[j];
                temp_sum += a[i][j + 1] * b[j + 1];
                temp_sum += a[i][j + 2] * b[j + 2];
                temp_sum += a[i][j + 3] * b[j + 3];
            }

            // 处理剩余的元素
            for (; j < k; j++) {
                temp_sum += a[i][j] * b[j];
            }

            sum[i] = temp_sum;
        }

        QueryPerformanceCounter(&tail);
        cout << "n=" << k << " time: " << (tail.QuadPart - head.QuadPart) * 1000.0 / freq.QuadPart << "ms" << endl;

        if (k == 1000) gap = 1500;
        if (k == 5500) gap = 4500;
    }

    return 0;
}
