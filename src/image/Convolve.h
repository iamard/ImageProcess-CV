#ifndef __CONVOLVE_H__
#define __CONVOLVE_H__

#include "Common.h"
#include "Functor.h"
#include "Precision.h"

size_t reflect(size_t size, int32_t x) {
    if (x < 0)
        return -x - 1;
    else if(x >= size)
        return 2 * size - x - 1;
    return x;
}

size_t circular(size_t size, int32_t x) {
    if (x < 0)
        return x + size;
    else if(x >= size)
        return x - size;
    return x;
}

template <typename T, size_t policy(size_t, int32_t) = reflect>
Image<T> convolve_generic(const Image<T> &image, const FracType<T> *kernel, size_t n) {
    size_t width  = image.width();
    size_t height = image.height();
    
    Image<T> output(width, height);
    for(int32_t y = 0; y < height; y++) {
        for(int32_t x = 0; x < width; x++) {
            PixelType<T> sumVal = { 0 };
            for(int32_t ky = 0; ky < n; ky++) {
                for(int32_t kx = 0; kx < n; kx++ ) {
                    size_t y1 = policy(height, y - ky);
                    size_t x1 = policy(width,  x - kx);
                    sumVal += static_cast<PixelType<T>>(image.getPixel(x1, y1)) * kernel[ky * n + kx];
                }
            }

            // Clamp the intermediate value
            output.setPixel(x, y, static_cast<T>(sumVal));
        }
    }
    
    return output;
}

/* Assume the kernel is symmetric */
template <typename T, size_t policy(size_t, int32_t) = reflect>
Image<T> convolve(const Image<T> &image, const FracType<T> *kernel, size_t n) {
    Convolver<T> device(convolve_generic);
    return device.dispatch(image, kernel, n);
}

#endif  // __CONVOLVE_H__
