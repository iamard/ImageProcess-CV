#ifndef __CONVOLVE_H__
#define __CONVOLVE_H__

size_t reflect(size_t size, int x) {
    if (x < 0)
        return -x - 1;
    else if(x >= size)
        return 2 * size - x - 1;
    return x;
}

size_t circular(size_t size, int x) {
    if (x < 0)
        return x + size;
    else if(x >= size)
        return x - size;
    return x;
}

/* Assume the kernel is symmetric */
template <typename T, size_t N, size_t policy(size_t, int)>
Image<T> convolve(const Image<T> &image, double kernel[N][N]) {
    size_t width  = image.width();
    size_t height = image.height();
    size_t bias  = N / 2;
    
    Image<T> output(width, height);
    for(int32_t y = 0; y < height; y++) {
        for(int32_t x = 0; x < width; x++) {
            double sumVal = (double)0.0f;
            for(int32_t ky = -bias; ky <= bias; ky++) {
                for(int32_t kx = -bias; kx <= bias; kx++ ) {
                    size_t y1 = policy(height, y - ky);
                    size_t x1 = policy(width,  x - kx);
                    // TBD: 2 or 3 channels
                    sumVal += kernel[kx + bias][ky + bias] * image[x1][y1].data[0];
                }
            }

            output[x][y].data[0] = sumVal;
        }
    }
    
    return output;
}

#endif  // __CONVOLVE_H__
