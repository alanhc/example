// dot.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N (1024 * 1024)
#define REPEAT 100

float *a;
float *b;

__attribute__((noinline))
float dot_product(const float *a, const float *b, int n) {
    float result = 0.0f;
    for (int i = 0; i < n; i++) {
        result += a[i] * b[i];
    }
    return result;
}

int main() {
    // Allocate large arrays on heap
    a = (float *)malloc(sizeof(float) * N);
    b = (float *)malloc(sizeof(float) * N);

    // Ensure pages are committed
    memset(a, 0, sizeof(float) * N);
    memset(b, 0, sizeof(float) * N);

    for (int i = 0; i < N; i++) {
        a[i] = (float)i / 255.0f;
        b[i] = (float)(N - i) / 255.0f;
    }

    float result = 0.0f;
    for (int j = 0; j < 1; j++) {               // outer loop for longer runtime
        for (int i = 0; i < REPEAT; i++) {
            result += dot_product(a, b, N);
        }
    }

    printf("a \u00b7 b (repeated) = %f\n", result);

    free(a);
    free(b);
    return 0;
}