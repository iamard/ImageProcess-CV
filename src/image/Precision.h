#ifndef __PRECISION_H__
#define __PRECISION_H__

#include "Pixel.h"

template<typename T>
struct Precision;

template<>
struct Precision<GRAY8> {
    typedef mono_pixel_trait<float> pixel_type;
    typedef float                   value_type;
    typedef float                   frac_type;
};

template<>
struct Precision<RGB888> {
    typedef rgb_pixel_trait<float> pixel_type;
    typedef float                  value_type;
    typedef float                  frac_type;
};

template <typename T>
using PixelType = typename Precision<T>::pixel_type;

template <typename T>
using ValueType = typename Precision<T>::value_type;

template <typename T>
using FracType = typename Precision<T>::frac_type;

#endif  // __PRECISION_H__
