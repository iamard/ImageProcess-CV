#ifndef __IMAGE_H__
#define __IMAGE_H__

/*
 * After reading the following book, I separate storage and image 
 * manipulation into separate files. (i.e. Image.h and Storage.h)
 *
 * "Applied C++ Practical Techniques for Building Better Software"
 */

#include "Pixel.h"
#include "Storage.h"

template<typename T, typename A>
class Image;

template <class S1, class S2, class A1, class A2>
Image<S2, A2> convert(const Image<S1, A1> &image);

template<typename T, typename A = PixelStorage<T>>
class Image {
    typedef typename T::value_type pixel_type;
    typedef typename T::value_type value_type;
    typedef typename A::iterator   iterator;

public:
    Image() {
    }

    Image(size_t width,
          size_t height,
          size_t stride = 0)
        : storage_(width, height, stride) {
        assert(width >= 0);
        assert(height >= 0);
    }

    Image(const Image &other) {
        storage_ = other.storage_;
    }

    template<class From>
    Image(const Image<From>& other) {
        *this = convert<From, T>(other);
    }

    virtual ~Image() {
    }

    size_t width() const {
        return storage_.width();
    }

    size_t height() const {
        return storage_.height();
    }

    size_t stride() const  {
        return storage_.stride();
    }
    
    const T& getPixel(int x, int y) const {
        return storage_.getPixel (x, y);
    }

    void setPixel(int32_t x, int32_t y, const T& pixel) {
        storage_.setPixel (x, y, pixel);
    }

    iterator begin() {
        return storage_.begin();
    }

    const iterator begin() const {
        return storage_.begin();
    }

    iterator end() {
        return storage_.end();
    }

    const iterator end() const {
        return storage_.end();
    }
    
    template<typename S>
    Image<T>& operator=(const Image<S> &other) {
        Image<T> temp(other);
        return temp;
    }

private:
    A storage_;
};

#endif  // __IMAGE_H__
