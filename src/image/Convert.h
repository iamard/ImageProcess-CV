#ifndef __CONVERT_H__
#define __CONVERT_H__

#include "Image.h"

template <typename From, typename To>
Image<To> convert(const Image<From> &image) {
    // TBD
}

template<>
Image<gray> convert(const Image<rgb> &image) {
    size_t width  = image.width();
    size_t height = image.height();
 
    Image<gray> output(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            transform<gray, rgb>(output[x][y], image[x][y]);
        }        
    }
    return output;
}

#endif  // __CONVERT_H__
