#ifndef __GAUSSIAN_H__
#define __GAUSSIAN_H__

#include "Convolve.h"

template <typename T, size_t N>
Image<T> gaussian(const Image<T>& image, double sigma) {
    // Gaussian kernel: e^{ -(x^2+y^2) / (2sigma^2) }
    double sumVal = 0;
    double kernel[N][N];
    for(int32_t x = 0; x < N; x++) {
        for(int32_t y = 0; y < N; y++) {
            int32_t x1 = x - N / 2;
            int32_t y1 = y - N / 2;
            kernel[x][y] = exp(-(x1 * x1 + y1 * y1) / (2.0 * sigma * sigma));
            sumVal += kernel[x][y];
        }
    }

    // Kernel normalization
    for(int32_t x = 0; x < N; x++) {
        for(int32_t y = 0; y < N; y++) {
            kernel[x][y] /= sumVal;
        }
    }

    return convolve<T, N, reflect>(image, kernel);
}
#endif  // __CAUSSIAN_H__
