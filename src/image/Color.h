#ifndef __COLOR_H__
#define __COLOR_H__

#include "Pixel.h"

template<typename S, typename T>
void convert(const S &src, T &dst);

template<>
void convert(const GRAY_8 &src, RGB_888 &dst) {
    dst = RGB_888(src.value, src.value, src.value);
}

template<>
void convert(const RGB_888 &src, GRAY_8 &dst) {
    dst = GRAY_8((src.red + src.green + src.blue) / 3);
}

#endif  // __COLOR_H__
