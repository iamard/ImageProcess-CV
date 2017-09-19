#ifndef __GAUSSIAN_H__
#define __GAUSSIAN_H__

#include "Convolve.h"

template <typename T>
Image<T> gaussian(const Image<T>& image, FracType<T> sigma, size_t n) {
    // Gaussian kernel: e^{ -(x^2+y^2) / (2sigma^2) } / (M_PI * sigma)
    FracType<T> sumVal = 0;
    FracType<T> kernel[n][n];
    for(int32_t x = 0; x < n; x++) {
        for(int32_t y = 0; y < n; y++) {
            int32_t x1 = x - n / 2;
            int32_t y1 = y - n / 2;
            kernel[x][y] = (exp(-(x1 * x1 + y1 * y1) / (2.0 * sigma * sigma))) / (M_PI * sigma);
            sumVal += kernel[x][y];
        }
    }

    // Kernel normalization
    for(int32_t x = 0; x < n; x++) {
        for(int32_t y = 0; y < n; y++) {
            kernel[x][y] /= sumVal;
        }
    }

    return convolve(image, reinterpret_cast<const FracType<T>*>(kernel), n);
}
#endif  // __CAUSSIAN_H__
