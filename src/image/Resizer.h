#ifndef __RESIZER_H__
#define __RESIZER_H__

#include "Precision.h"

template<typename T>
inline PixelType<T> bilinear(const Image<T> &image, 
                             FracType<T>    inputX,
                             FracType<T>    inputY) {
    int32_t     intX  = (int32_t)inputX;
    int32_t     intY  = (int32_t)inputY;
    FracType<T> fracX = inputX - intX;
    FracType<T> fracY = inputY - intY;
    
    PixelType<T> pixel1 = image.getPixel(intX + 0, intY + 0);
    PixelType<T> pixel2 = image.getPixel(intX + 0, intY + 1);
    PixelType<T> pixel3 = image.getPixel(intX + 1, intY + 0);
    PixelType<T> pixel4 = image.getPixel(intX + 1, intY + 1);

    PixelType<T> lower = pixel1 * (1.0 - fracX) + pixel3 * fracX;
    PixelType<T> upper = pixel2 * (1.0 - fracX) + pixel4 * fracX;
    PixelType<T> value = lower  * (1.0 - fracY) + upper * fracY;

    value += 0.5;
    return value;
}

template<typename T>
Image<T> resize(const Image<T> &image, 
                size_t         outputW,
                size_t         outputH) {
    FracType<T> inputW = image.width();
    FracType<T> inputH = image.height();
    FracType<T> ratioW = inputW / (FracType<T>)outputW;
    FracType<T> ratioH = inputH / (FracType<T>)outputH;

    Image<T> output(outputW, outputH);
    for (int32_t outY = 0; outY < outputH; outY++) {
        for (int32_t outX = 0; outX < outputW; outX++) {
            FracType<T>  inX = outX * ratioW;
            FracType<T>  inY = outY * ratioH;
            PixelType<T> val = bilinear(image, inX, inY);
            output.setPixel(outX, outY, val);
        }
    }
    return output;
}

#endif  // __RESIZER_H__
