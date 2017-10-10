#ifndef __PIXEL_H__
#define __PIXEL_H__

#include "Common.h"

template<typename T>
struct pixel_value_info {
    static constexpr size_t value_bytes = sizeof(T);
    static constexpr T      min_value   = numeric_limits<T>::min();
    static constexpr T      max_value   = numeric_limits<T>::max();
};

template<>
struct pixel_value_info<double> {
    static constexpr size_t value_bytes = 8;
    static constexpr double min_value   = 0.0;
    static constexpr double max_value   = 65536.0;
};

template<>
struct pixel_value_info<uint8_t> {
    static const size_t  value_bytes = 1;
    static const uint8_t min_value   = 0;
    static const uint8_t max_value   = 0xFF;
};

template<>
struct pixel_value_info<uint32_t> {
    static const size_t   value_bytes = 1;
    static const uint32_t min_value   = 0;
    static const uint32_t max_value   = 0xFFFFFFFF;
};

template<typename T>
struct pixel_value_clamp {
    static T clamp(const T& value) {
        if (value < pixel_value_info<T>::min_value)
            return pixel_value_info<T>::min_value;
        else if (value > pixel_value_info<T>::max_value)
            return pixel_value_info<T>::max_value;
        else
            return value;
    }
};

enum class color: std::uint8_t { MONO, RGB, BGR, RGBA, BGRA, YUYV, UYVY, OTHER };

template<color C, typename V, typename P, size_t N>
struct pixel_trait_base {
    typedef V value_type;
    typedef P pixel_type;

    static constexpr color  color_space     = C;
    static constexpr size_t num_channels    = N;
    static constexpr size_t bytes_per_pixel = sizeof(V) * N;

    pixel_type operator+(const value_type& value) const {
        return static_cast<const pixel_type*>(this)->add_value(value);
    }

    pixel_type operator+(const pixel_type& value) const {
        return static_cast<const pixel_type*>(this)->add_value(value);
    }

    pixel_type operator-(const value_type& value) const {
        return static_cast<const pixel_type*>(this)->sub_value(value);
    }

    pixel_type operator-(const pixel_type& value) const {
        return static_cast<const pixel_type*>(this)->sub_value(value);
    }
    
    pixel_type operator*(const value_type& value) const {
        return static_cast<const pixel_type*>(this)->mul_value(value);
    }
    
    pixel_type operator*(const pixel_type& value) const {
        return static_cast<const pixel_type*>(this)->mul_value(value);
    }

    pixel_type operator/(const value_type& value) const {
        return static_cast<const pixel_type*>(this)->div_value(value);
    }
    
    pixel_type operator/(const pixel_type& value) const {
        return static_cast<const pixel_type*>(this)->div_value(value);
    }
    
    pixel_type& operator+=(const value_type& value) {
        return static_cast<pixel_type*>(this)->add_assign(value);
    }

    pixel_type& operator+=(const pixel_type& value) {
        return static_cast<pixel_type*>(this)->add_assign(value);
    }

    pixel_type& operator-=(const value_type& value) {
        return static_cast<pixel_type*>(this)->sub_assign(value);
    }

    pixel_type& operator-=(const pixel_type& value) {
        return static_cast<pixel_type*>(this)->sub_assign(value);
    }
    
    pixel_type& operator*=(const value_type& value) {
        return static_cast<pixel_type*>(this)->mul_assign(value);
    }

    pixel_type& operator*=(const pixel_type& value) {
        return static_cast<pixel_type*>(this)->mul_assign(value);
    }
    
    pixel_type& operator/=(const value_type& value) {
        return static_cast<pixel_type*>(this)->div_assign(value);
    }

    pixel_type& operator/=(const pixel_type& value) {
        return static_cast<pixel_type*>(this)->div_assign(value);
    }
};

/*
 * The following are for 1-channel pixel format
 */
template <color format, typename T>
struct pixel1_type_trait;

#define define_pixel1_type(pixel_name, format, T, channel1)                                                             \
template<>                                                                                                              \
struct pixel1_type_trait<format, T>: public pixel_trait_base<format, T, pixel1_type_trait<format, T>, 1>  {             \
    static constexpr color pixel_format = format;                                                                       \
                                                                                                                        \
    T channel1;                                                                                                         \
                                                                                                                        \
    pixel1_type_trait()                                                                                                 \
        : channel1(0) {                                                                                                 \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait(T v)                                                                                              \
        : channel1(v) {                                                                                                 \
    }                                                                                                                   \
                                                                                                                        \
    template<typename S>                                                                                                \
    pixel1_type_trait(const pixel1_type_trait<format, S> &other) {                                                      \
        channel1 = pixel_value_clamp<T>::clamp(static_cast<T>(other.channel1));                                         \
    }                                                                                                                   \
                                                                                                                        \
    template<typename S>                                                                                                \
    pixel1_type_trait<format, T>& operator=(const pixel1_type_trait<format, S>& other) {                                \
        pixel1_type_trait<format, T> temp(other);                                                                       \
        swap(channel1, other.channel1);                                                                                 \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T> add_value(const T& other) const {                                                      \
        return pixel1_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 + other));                             \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T> add_value(const pixel1_type_trait<format, T>& other) const {                           \
        return pixel1_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 + other.channel1));                    \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T> sub_value(const T& other) const {                                                      \
        return pixel1_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 - other));                             \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T> sub_value(const pixel1_type_trait<format, T>& other) const {                           \
        return pixel1_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 - other.channel1));                    \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T> mul_value(const T& other) const {                                                      \
        return pixel1_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 * other));                             \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T> mul_value(const pixel1_type_trait<format, T>& other) const {                           \
        return pixel1_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 * other.channel1));                    \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T> div_value(const T& other) const {                                                      \
        return pixel1_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 / other));                             \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T> div_value(const pixel1_type_trait<format, T>& other) const {                           \
        return pixel1_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 / other.channel1));                    \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T>& add_assign(const T& other) {                                                          \
        channel1 = pixel_value_clamp<T>::clamp(channel1 + other);                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T>& add_assign(const pixel1_type_trait<format, T>& other) {                               \
        channel1 = pixel_value_clamp<T>::clamp(channel1 + other.channel1);                                              \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T>& sub_assign(const T& other) {                                                          \
        channel1 = pixel_value_clamp<T>::clamp(channel1 - other);                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T>& sub_assign(const pixel1_type_trait<format, T>& other) {                               \
        channel1 = pixel_value_clamp<T>::clamp(channel1 - other.channel1);                                              \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T>& mul_assign(const T& other) {                                                          \
        channel1 = pixel_value_clamp<T>::clamp(channel1 * other);                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T>& mul_assign(const pixel1_type_trait<format, T>& other) {                               \
        channel1 = pixel_value_clamp<T>::clamp(channel1 * other.channel1);                                              \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T>& div_assign(const T& other) {                                                          \
        channel1 = pixel_value_clamp<T>::clamp(channel1 / other);                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel1_type_trait<format, T>& div_assign(const pixel1_type_trait<format, T>& other) {                               \
        channel1 = pixel_value_clamp<T>::clamp(channel1 / other.channel1);                                              \
        return *this;                                                                                                   \
    }                                                                                                                   \
};                                                                                                                      \
                                                                                                                        \
using pixel_name = pixel1_type_trait<format, T>;

define_pixel1_type(GRAY_8,   color::MONO, uint8_t, value);
define_pixel1_type(GRAY_F32, color::MONO, float, value);
define_pixel1_type(GRAY_F64, color::MONO, double, value);

/*
 * The following are for 3-channel pixel format.
 */
template <color format, typename T>
struct pixel3_type_trait;

#define define_pixel3_type(pixel_name, format, T, channel1, channel2, channel3)                                         \
template<>                                                                                                              \
struct pixel3_type_trait<format, T>: public pixel_trait_base<format, T, pixel3_type_trait<format, T>, 3>  {             \
    static constexpr color pixel_format = format;                                                                       \
                                                                                                                        \
    T channel1;                                                                                                         \
    T channel2;                                                                                                         \
    T channel3;                                                                                                         \
                                                                                                                        \
    pixel3_type_trait()                                                                                                 \
        : channel1(0),                                                                                                  \
          channel2(0),                                                                                                  \
          channel3(0) {                                                                                                 \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait(T value)                                                                                          \
        : channel1(value),                                                                                              \
          channel2(value),                                                                                              \
          channel3(value) {                                                                                             \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait(T v1, T v2, T v3)                                                                                 \
        : channel1(v1),                                                                                                 \
          channel2(v2),                                                                                                 \
          channel3(v3) {                                                                                                \
    }                                                                                                                   \
                                                                                                                        \
    template<typename S>                                                                                                \
    pixel3_type_trait(const pixel3_type_trait<format, S> &other) {                                                      \
        channel1 = pixel_value_clamp<T>::clamp(static_cast<T>(other.channel1));                                         \
        channel2 = pixel_value_clamp<T>::clamp(static_cast<T>(other.channel2));                                         \
        channel3 = pixel_value_clamp<T>::clamp(static_cast<T>(other.channel3));                                         \
        printf("%d %d %d\n", channel1, channel2, channel3); \
    }                                                                                                                   \
                                                                                                                        \
    template<typename S>                                                                                                \
    pixel3_type_trait<format, T>& operator=(const pixel3_type_trait<format, S>& other) {                                \
        pixel3_type_trait<format, T> temp(other);                                                                       \
        swap(channel1, other.channel1);                                                                                 \
        swap(channel2, other.channel2);                                                                                 \
        swap(channel3, other.channel3);                                                                                 \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T> add_value(const T& value) const {                                                      \
        return pixel3_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 + value),                              \
                                            pixel_value_clamp<T>::clamp(channel2 + value),                              \
                                            pixel_value_clamp<T>::clamp(channel3 + value));                             \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T> add_value(const pixel3_type_trait<format, T>& other) const {                           \
        return pixel3_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 + other.channel1),                     \
                                            pixel_value_clamp<T>::clamp(channel2 + other.channel2),                     \
                                            pixel_value_clamp<T>::clamp(channel3 + other.channel3));                    \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T> sub_value(const T& value) const {                                                      \
        return pixel3_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 - value),                              \
                                            pixel_value_clamp<T>::clamp(channel2 - value),                              \
                                            pixel_value_clamp<T>::clamp(channel3 - value));                             \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T> sub_value(const pixel3_type_trait<format, T>& other) const {                           \
        return pixel3_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 - other.channel1),                     \
                                            pixel_value_clamp<T>::clamp(channel2 - other.channel2),                     \
                                            pixel_value_clamp<T>::clamp(channel3 - other.channel3));                    \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T> mul_value(const T& value) const {                                                      \
        return pixel3_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 * value),                              \
                                            pixel_value_clamp<T>::clamp(channel2 * value),                              \
                                            pixel_value_clamp<T>::clamp(channel3 * value));                             \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T> mul_value(const pixel3_type_trait<format, T>& other) const {                           \
        return pixel3_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 * other.channel1),                     \
                                            pixel_value_clamp<T>::clamp(channel2 * other.channel2),                     \
                                            pixel_value_clamp<T>::clamp(channel3 * other.channel3));                    \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T> div_value(const T& value) const {                                                      \
        return pixel3_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 / value),                              \
                                            pixel_value_clamp<T>::clamp(channel2 / value),                              \
                                            pixel_value_clamp<T>::clamp(channel3 / value));                             \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T> div_value(const pixel3_type_trait<format, T>& other) const {                           \
        return pixel3_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 / other.channel1),                     \
                                            pixel_value_clamp<T>::clamp(channel2 / other.channel2),                     \
                                            pixel_value_clamp<T>::clamp(channel3 / other.channel3));                    \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T>& add_assign(const T& value) {                                                          \
        channel1 = pixel_value_clamp<T>::clamp(channel1 + value);                                                       \
        channel2 = pixel_value_clamp<T>::clamp(channel2 + value);                                                       \
        channel3 = pixel_value_clamp<T>::clamp(channel3 + value);                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T>& add_assign(const pixel3_type_trait<format, T>& other) {                               \
        channel1 = pixel_value_clamp<T>::clamp(channel1 + other.channel1);                                              \
        channel2 = pixel_value_clamp<T>::clamp(channel2 + other.channel2);                                              \
        channel3 = pixel_value_clamp<T>::clamp(channel3 + other.channel3);                                              \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T>& sub_assign(const T& value) {                                                          \
        channel1 = pixel_value_clamp<T>::clamp(channel1 - value);                                                       \
        channel2 = pixel_value_clamp<T>::clamp(channel2 - value);                                                       \
        channel3 = pixel_value_clamp<T>::clamp(channel3 - value);                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T>& sub_assign(const pixel3_type_trait<format, T>& other) {                               \
        channel1 = pixel_value_clamp<T>::clamp(channel1 - other.channel1);                                              \
        channel2 = pixel_value_clamp<T>::clamp(channel2 - other.channel2);                                              \
        channel3 = pixel_value_clamp<T>::clamp(channel3 - other.channel3);                                              \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T>& mul_assign(const T& value) {                                                          \
        channel1 = pixel_value_clamp<T>::clamp(channel1 * value);                                                       \
        channel2 = pixel_value_clamp<T>::clamp(channel2 * value);                                                       \
        channel3 = pixel_value_clamp<T>::clamp(channel3 * value);                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T>& mul_assign(const pixel3_type_trait<format, T>& other) {                               \
        channel1 = pixel_value_clamp<T>::clamp(channel1 * other.channel1);                                              \
        channel2 = pixel_value_clamp<T>::clamp(channel2 * other.channel2);                                              \
        channel3 = pixel_value_clamp<T>::clamp(channel3 * other.channel3);                                              \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T>& div_assign(const T& value) {                                                          \
        channel1 = pixel_value_clamp<T>::clamp(channel1 / value);                                                       \
        channel2 = pixel_value_clamp<T>::clamp(channel2 / value);                                                       \
        channel3 = pixel_value_clamp<T>::clamp(channel3 / value);                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel3_type_trait<format, T>& div_assign(const pixel3_type_trait<format, T>& other) {                               \
        channel1 = pixel_value_clamp<T>::clamp(channel1 / other.channel1);                                              \
        channel2 = pixel_value_clamp<T>::clamp(channel2 / other.channel2);                                              \
        channel3 = pixel_value_clamp<T>::clamp(channel3 / other.channel3);                                              \
        return *this;                                                                                                   \
    }                                                                                                                   \
};                                                                                                                      \
                                                                                                                        \
using pixel_name = pixel3_type_trait<format, T>;

define_pixel3_type(RGB_888, color::RGB, uint8_t, red, green, blue);
define_pixel3_type(RGB_F32, color::RGB, float, red, green, blue);
define_pixel3_type(RGB_F64, color::RGB, double, red, green, blue);
define_pixel3_type(BGR_888, color::BGR, uint8_t, blue, green, red);
define_pixel3_type(BGR_F32, color::BGR, float, blue, green, red);
define_pixel3_type(BGR_F64, color::BGR, double, blue, green, red);

/*
 * The following are for 4-channel pixel format.
 */
template <color format, typename T>
struct pixel4_type_trait;

#define define_pixel4_type(pixel_name, format, T, channel1, channel2, channel3, channel4)                               \
template<>                                                                                                              \
struct pixel4_type_trait<format, T>: public pixel_trait_base<format, T, pixel4_type_trait<format, T>, 4>  {             \
    static constexpr color pixel_format = format;                                                                       \
                                                                                                                        \
    T channel1;                                                                                                         \
    T channel2;                                                                                                         \
    T channel3;                                                                                                         \
    T channel4;                                                                                                         \
                                                                                                                        \
    pixel4_type_trait()                                                                                                 \
        : channel1(0),                                                                                                  \
          channel2(0),                                                                                                  \
          channel3(0),                                                                                                  \
          channel4(0) {                                                                                                 \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait(T value)                                                                                          \
        : channel1(value),                                                                                              \
          channel2(value),                                                                                              \
          channel3(value),                                                                                              \
          channel4(value) {                                                                                             \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait(T v1, T v2, T v3, T v4)                                                                           \
        : channel1(v1),                                                                                                 \
          channel2(v2),                                                                                                 \
          channel3(v3),                                                                                                 \
          channel4(v4) {                                                                                                \
    }                                                                                                                   \
                                                                                                                        \
    template<typename S>                                                                                                \
    pixel4_type_trait(const pixel4_type_trait<format, S> &other) {                                                      \
        channel1 = pixel_value_clamp<T>::clamp(static_cast<T>(other.channel1));                                         \
        channel2 = pixel_value_clamp<T>::clamp(static_cast<T>(other.channel2));                                         \
        channel3 = pixel_value_clamp<T>::clamp(static_cast<T>(other.channel3));                                         \
        channel4 = pixel_value_clamp<T>::clamp(static_cast<T>(other.channel4));                                         \
    }                                                                                                                   \
                                                                                                                        \
    template<typename S>                                                                                                \
    pixel4_type_trait<format, T>& operator=(const pixel4_type_trait<format, S>& other) {                                \
        pixel4_type_trait<format, T> temp(other);                                                                       \
        swap(channel1, other.channel1);                                                                                 \
        swap(channel2, other.channel2);                                                                                 \
        swap(channel3, other.channel3);                                                                                 \
        swap(channel4, other.channel4);                                                                                 \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T> add_value(const T& value) const {                                                      \
        return pixel4_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 + value),                              \
                                            pixel_value_clamp<T>::clamp(channel2 + value),                              \
                                            pixel_value_clamp<T>::clamp(channel3 + value),                              \
                                            channel4);                                                                  \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T> add_value(const pixel4_type_trait<format, T>& other) const {                           \
        return pixel4_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 + other.channel1),                     \
                                            pixel_value_clamp<T>::clamp(channel2 + other.channel2),                     \
                                            pixel_value_clamp<T>::clamp(channel3 + other.channel3),                     \
                                            channel4);                                                                  \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T> sub_value(const T& value) const {                                                      \
        return pixel4_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 - value),                              \
                                            pixel_value_clamp<T>::clamp(channel2 - value),                              \
                                            pixel_value_clamp<T>::clamp(channel3 - value),                              \
                                            channel4);                                                                  \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T> sub_value(const pixel4_type_trait<format, T>& other) const {                           \
        return pixel4_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 - other.channel1),                     \
                                            pixel_value_clamp<T>::clamp(channel2 - other.channel2),                     \
                                            pixel_value_clamp<T>::clamp(channel3 - other.channel3),                     \
                                            channel4);                                                                  \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T> mul_value(const T& value) const {                                                      \
        return pixel4_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 * value),                              \
                                            pixel_value_clamp<T>::clamp(channel2 * value),                              \
                                            pixel_value_clamp<T>::clamp(channel3 * value),                              \
                                            channel4);                                                                  \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T> mul_value(const pixel4_type_trait<format, T>& other) const {                           \
        return pixel4_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 * other.channel1),                     \
                                            pixel_value_clamp<T>::clamp(channel2 * other.channel2),                     \
                                            pixel_value_clamp<T>::clamp(channel3 * other.channel3),                     \
                                            channel4);                                                                  \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T> div_value(const T& value) const {                                                      \
        return pixel4_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 / value),                              \
                                            pixel_value_clamp<T>::clamp(channel2 / value),                              \
                                            pixel_value_clamp<T>::clamp(channel3 / value),                              \
                                            channel4);                                                                  \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T> div_value(const pixel4_type_trait<format, T>& other) const {                           \
        return pixel4_type_trait<format, T>(pixel_value_clamp<T>::clamp(channel1 / other.channel1),                     \
                                            pixel_value_clamp<T>::clamp(channel2 / other.channel2),                     \
                                            pixel_value_clamp<T>::clamp(channel3 / other.channel3),                     \
                                            channel4);                                                                  \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T>& add_assign(const T& value) {                                                          \
        channel1 = pixel_value_clamp<T>::clamp(channel1 + value);                                                       \
        channel2 = pixel_value_clamp<T>::clamp(channel2 + value);                                                       \
        channel3 = pixel_value_clamp<T>::clamp(channel3 + value);                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T>& add_assign(const pixel4_type_trait<format, T>& other) {                               \
        channel1 = pixel_value_clamp<T>::clamp(channel1 + other.channel1);                                              \
        channel2 = pixel_value_clamp<T>::clamp(channel2 + other.channel2);                                              \
        channel3 = pixel_value_clamp<T>::clamp(channel3 + other.channel3);                                              \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T>& sub_assign(const T& value) {                                                          \
        channel1 = pixel_value_clamp<T>::clamp(channel1 - value);                                                       \
        channel2 = pixel_value_clamp<T>::clamp(channel2 - value);                                                       \
        channel3 = pixel_value_clamp<T>::clamp(channel3 - value);                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T>& sub_assign(const pixel4_type_trait<format, T>& other) {                               \
        channel1 = pixel_value_clamp<T>::clamp(channel1 - other.channel1);                                              \
        channel2 = pixel_value_clamp<T>::clamp(channel2 - other.channel2);                                              \
        channel3 = pixel_value_clamp<T>::clamp(channel3 - other.channel3);                                              \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T>& mul_assign(const T& value) {                                                          \
        channel1 = pixel_value_clamp<T>::clamp(channel1 * value);                                                       \
        channel2 = pixel_value_clamp<T>::clamp(channel2 * value);                                                       \
        channel3 = pixel_value_clamp<T>::clamp(channel3 * value);                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T>& mul_assign(const pixel4_type_trait<format, T>& other) {                               \
        channel1 = pixel_value_clamp<T>::clamp(channel1 * other.channel1);                                              \
        channel2 = pixel_value_clamp<T>::clamp(channel2 * other.channel2);                                              \
        channel3 = pixel_value_clamp<T>::clamp(channel3 * other.channel3);                                              \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T>& div_assign(const T& value) {                                                          \
        channel1 = pixel_value_clamp<T>::clamp(channel1 / value);                                                       \
        channel2 = pixel_value_clamp<T>::clamp(channel2 / value);                                                       \
        channel3 = pixel_value_clamp<T>::clamp(channel3 / value);                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    pixel4_type_trait<format, T>& div_assign(const pixel4_type_trait<format, T>& other) {                               \
        channel1 = pixel_value_clamp<T>::clamp(channel1 / other.channel1);                                              \
        channel2 = pixel_value_clamp<T>::clamp(channel2 / other.channel2);                                              \
        channel3 = pixel_value_clamp<T>::clamp(channel3 / other.channel3);                                              \
        return *this;                                                                                                   \
    }                                                                                                                   \
};                                                                                                                      \
                                                                                                                        \
using pixel_name = pixel4_type_trait<format, T>;

define_pixel4_type(RGBA_8888, color::RGBA, uint8_t, red, green, blue, alpha);
define_pixel4_type(RGBA_F32, color::RGBA, float, red, green, blue, alpha);
define_pixel4_type(RGBA_F64, color::RGBA, double, red, green, blue, alpha);
define_pixel4_type(BGRA_8888, color::BGRA, uint8_t, blue, green, red, alpha);
define_pixel4_type(BGRA_F32, color::BGRA, float, blue, green, red, alpha);
define_pixel4_type(BGRA_F64, color::BGRA, double, blue, green, red, alpha);
define_pixel4_type(UYVY_8888, color::UYVY, uint8_t, u, y1, v, y2);
define_pixel4_type(UYVY_F32, color::UYVY, float, u, y1, v, y2);
define_pixel4_type(UYVY_F64, color::UYVY, double, u, y1, v, y2);
define_pixel4_type(YUYV_8888, color::YUYV, uint8_t, y1, u, y2, v);
define_pixel4_type(YUYV_F32, color::YUYV, float, y1, u, y2, v);
define_pixel4_type(YUYV_F64, color::YUYV, double, y1, u, y2, v);

#endif  // __PIXEL_H__
