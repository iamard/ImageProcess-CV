#ifndef __COLOR_H__
#define __COLOR_H__

#include "Pixel.h"

template<typename S, typename T>
void convert(const mono_pixel_trait<S> &src,
             rgb_pixel_trait<T>        &dst) {
    dst = rgb_pixel_trait<T>(src.value, src.vaue, src.value);
}

template<typename S, typename T>
void convert(const rgb_pixel_trait<S> &src,
             mono_pixel_trait<T>      &dst) {
    dst = mono_pixel_trait<T>((src.red + src.green + src.blue) / 3);
}

#endif  // __COLOR_H__
