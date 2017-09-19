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

enum class color: std::uint8_t { MONO, RGB, OTHER };

template<typename V, typename P, size_t channels>
struct pixel_trait_base {
    typedef V value_type;
    typedef P pixel_type;

    //static constexpr uint8_t color_space     = color; 
    static constexpr size_t  channel_count   = channels;
    static constexpr size_t  bytes_per_pixel = sizeof(V) * channels;

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

template<typename T>
struct mono_pixel_trait: public pixel_trait_base<T, mono_pixel_trait<T>, 1>
{
    T value;

    mono_pixel_trait()
        : value(0) {
    }

    mono_pixel_trait(T v)
        : value(v) {
    }

    template<typename S>
    mono_pixel_trait(const mono_pixel_trait<S> &other) {
        value = pixel_value_clamp<T>::clamp(other.value);
    }

    template<typename S>
    mono_pixel_trait<T>& operator=(const mono_pixel_trait<S>& other) {
        mono_pixel_trait<T> temp(other);
        swap(value, other.value);
        return *this;
    }

    mono_pixel_trait<T> add_value(const T &other) const {
        return mono_pixel_trait<T>(pixel_value_clamp<T>::clamp(value + other));
    }
    
    mono_pixel_trait<T> add_value(const mono_pixel_trait<T> &other) const {
        return mono_pixel_trait<T>(pixel_value_clamp<T>::clamp(value + other.value));
    }

    mono_pixel_trait<T> sub_value(const T &other) const {
        return mono_pixel_trait<T>(pixel_value_clamp<T>::clamp(value - other));
    }
    
    mono_pixel_trait<T> sub_value(const mono_pixel_trait<T> &other) const {
        return mono_pixel_trait<T>(pixel_value_clamp<T>::clamp(value - other.value));
    }

    mono_pixel_trait<T> mul_value(const T &other) const {
        return mono_pixel_trait<T>(pixel_value_clamp<T>::clamp(value * other));
    }

    mono_pixel_trait<T> mul_value(const mono_pixel_trait<T> &other) const {
        return mono_pixel_trait<T>(pixel_value_clamp<T>::clamp(value * other.value));
    }

    mono_pixel_trait<T> div_value(const T &other) const {
        return mono_pixel_trait<T>(pixel_value_clamp<T>::clamp(value / other));
    }

    mono_pixel_trait<T> div_value(const mono_pixel_trait<T> &other) const {
        return mono_pixel_trait<T>(pixel_value_clamp<T>::clamp(value / other.value));
    }
    
    mono_pixel_trait<T>& add_assign(const T &other) {
        value = pixel_value_clamp<T>::clamp(value + other);
        return *this;
    }
    
    mono_pixel_trait<T>& add_assign(const mono_pixel_trait<T> &other) {
        value = pixel_value_clamp<T>::clamp(value + other.value);
        return *this;
    }

    mono_pixel_trait<T>& sub_assign(const T &other) {
        value = pixel_value_clamp<T>::clamp(value - other);
        return *this;
    }
    
    mono_pixel_trait<T>& sub_assign(const mono_pixel_trait<T> &other) {
        value = pixel_value_clamp<T>::clamp(value - other.value);
        return *this;
    }
    
    mono_pixel_trait<T>& mul_assign(const T &other) {
        value = pixel_value_clamp<T>::clamp(value * other);
        return *this;
    }

    mono_pixel_trait<T>& mul_assign(const mono_pixel_trait<T> &other) {
        value = pixel_value_clamp<T>::clamp(value * other.value);
        return *this;
    }

    mono_pixel_trait<T>& div_assign(const T &other) {
        value = pixel_value_clamp<T>::clamp(value / other);
        return *this;
    }
    
    mono_pixel_trait<T>& div_assign(const mono_pixel_trait<T> &other) {
        value = pixel_value_clamp<T>::clamp(value / other.value);
        return *this;
    }
};

typedef mono_pixel_trait<uint8_t>  GRAY8;
typedef mono_pixel_trait<uint32_t> GRAY32;

template<typename T>
struct rgb_pixel_trait: public pixel_trait_base<T, rgb_pixel_trait<T>, 3> 
{
    T red;
    T green;
    T blue;

    rgb_pixel_trait()
        : red(0),
          green(0),
          blue(0) {
    }

    rgb_pixel_trait(T value)
        : red(value),
          green(value),
          blue(value) {
    }

    rgb_pixel_trait(T r, T g, T b)
        : red(r),
          green(g),
          blue(b) {
    }

    template<typename S>
    rgb_pixel_trait(const rgb_pixel_trait<S> &other) {
        red   = pixel_value_clamp<T>::clamp(other.red);
        green = pixel_value_clamp<T>::clamp(other.green);
        blue  = pixel_value_clamp<T>::clamp(other.blue);
    }

    template<typename S>
    rgb_pixel_trait<T>& operator=(const rgb_pixel_trait<S>& other) {
        rgb_pixel_trait<T> temp(other);
        swap(red, other.red);
        swap(green, other.green);
        swap(blue, other.blue);
        return *this;
    }

    rgb_pixel_trait<T> add_value(const T& value) const {
        return rgb_pixel_trait<T>(pixel_value_clamp<T>::clamp(red   + value),
                                  pixel_value_clamp<T>::clamp(green + value),
                                  pixel_value_clamp<T>::clamp(blue  + value));
    }

    rgb_pixel_trait<T> add_value(const rgb_pixel_trait<T>& other) const {
        return rgb_pixel_trait<T>(pixel_value_clamp<T>::clamp(red   + other.value),
                                  pixel_value_clamp<T>::clamp(green + other.value),
                                  pixel_value_clamp<T>::clamp(blue  + other.value));
    }
    
    rgb_pixel_trait<T> sub_value(const T& value) const {
        return rgb_pixel_trait<T>(pixel_value_clamp<T>::clamp(red   - value),
                                  pixel_value_clamp<T>::clamp(green - value),
                                  pixel_value_clamp<T>::clamp(blue  - value));
    }

    rgb_pixel_trait<T> sub_value(const rgb_pixel_trait<T>& other) const {
        return rgb_pixel_trait<T>(pixel_value_clamp<T>::clamp(red   - other.value),
                                  pixel_value_clamp<T>::clamp(green - other.value),
                                  pixel_value_clamp<T>::clamp(blue  - other.value));
    }

    rgb_pixel_trait<T> mul_value(const T& value) const {
        return rgb_pixel_trait<T>(pixel_value_clamp<T>::clamp(red   * value),
                                  pixel_value_clamp<T>::clamp(green * value),
                                  pixel_value_clamp<T>::clamp(blue  * value));
    }

    rgb_pixel_trait<T> mul_value(const rgb_pixel_trait<T>& other) const {
        return rgb_pixel_trait<T>(pixel_value_clamp<T>::clamp(red   * other.value),
                                  pixel_value_clamp<T>::clamp(green * other.value),
                                  pixel_value_clamp<T>::clamp(blue  * other.value));
    }

    rgb_pixel_trait<T> div_value(const T& value) const {
        return rgb_pixel_trait<T>(pixel_value_clamp<T>::clamp(red   / value),
                                  pixel_value_clamp<T>::clamp(green / value),
                                  pixel_value_clamp<T>::clamp(blue  / value));
    }

    rgb_pixel_trait<T> div_value(const rgb_pixel_trait<T>& other) const {
        return rgb_pixel_trait<T>(pixel_value_clamp<T>::clamp(red   / other.value),
                                  pixel_value_clamp<T>::clamp(green / other.value),
                                  pixel_value_clamp<T>::clamp(blue  / other.value));
    }

    rgb_pixel_trait<T>& add_assign(const T& other) {
        red   = pixel_value_clamp<T>::clamp(red   + other);
        green = pixel_value_clamp<T>::clamp(green + other);
        blue  = pixel_value_clamp<T>::clamp(blue  + other);
        return *this;
    }

    rgb_pixel_trait<T>& add_assign(const rgb_pixel_trait<T>& other) {
        red   = pixel_value_clamp<T>::clamp(red   + other.value);
        green = pixel_value_clamp<T>::clamp(green + other.value);
        blue  = pixel_value_clamp<T>::clamp(blue  + other.value);
        return *this;
    }

    rgb_pixel_trait<T>& sub_assign(const T& other) {
        red   = pixel_value_clamp<T>::clamp(red   - other);
        green = pixel_value_clamp<T>::clamp(green - other);
        blue  = pixel_value_clamp<T>::clamp(blue  - other);
        return *this;
    }
    
    rgb_pixel_trait<T>& sub_assign(const rgb_pixel_trait<T>& other) {
        red   = pixel_value_clamp<T>::clamp(red   - other.value);
        green = pixel_value_clamp<T>::clamp(green - other.value);
        blue  = pixel_value_clamp<T>::clamp(blue  - other.value);
        return *this;
    }

    rgb_pixel_trait<T>& mul_assign(const T& other) {
        red   = pixel_value_clamp<T>::clamp(red   * other);
        green = pixel_value_clamp<T>::clamp(green * other);
        blue  = pixel_value_clamp<T>::clamp(blue  * other);
        return *this;
    }
    
    rgb_pixel_trait<T>& mul_assign(const rgb_pixel_trait<T>& other) {
        red   = pixel_value_clamp<T>::clamp(red   * other.value);
        green = pixel_value_clamp<T>::clamp(green * other.value);
        blue  = pixel_value_clamp<T>::clamp(blue  * other.value);
        return *this;
    }

    rgb_pixel_trait<T>& div_assign(const T& other) {
        red   = pixel_value_clamp<T>::clamp(red   / other);
        green = pixel_value_clamp<T>::clamp(green / other);
        blue  = pixel_value_clamp<T>::clamp(blue  / other);
        return *this;
    }
    
    rgb_pixel_trait<T>& div_assign(const rgb_pixel_trait<T>& other) {
        red   = pixel_value_clamp<T>::clamp(red   / other.value);
        green = pixel_value_clamp<T>::clamp(green / other.value);
        blue  = pixel_value_clamp<T>::clamp(blue  / other.value);
        return *this;
    }
};

typedef rgb_pixel_trait<uint8_t> RGB888;

#endif  // __PIXEL_H__
