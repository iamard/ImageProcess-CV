#ifndef __CONVERT_H__
#define __CONVERT_H__

#include "Image.h"
#include "Color.h"

template <typename S, typename T>
Image<T> convert(const Image<S> &image) {
    printf("error\n");
}

template<>
Image<GRAY8> convert(const Image<RGB888> &image) {
    size_t width  = image.width();
    size_t height = image.height();
    size_t stride = image.stride();
 
    Image<GRAY8> output(width, height, stride);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            GRAY8 temp;
            convert(image.getPixel(x, y), temp);
            output.setPixel(x, y, temp);
        }
    }
    return output;
}
template<>
Image<GRAY32> convert(const Image<GRAY8> &image) {
    Image<GRAY32> output;
    for (int32_t y = 0; y < image.height(); y++) {
        for (int32_t x = 0; x < image.width(); x++) {
            output.setPixel(x, y, static_cast<GRAY32>(image.getPixel(x, y)));
        }
    }
    return output;
}

#endif  // __CONVERT_H__
