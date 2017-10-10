#ifndef __STORAGE_H__
#define __STORAGE_H__

#include "Alloc.h"

class PixelBuffer {
public:
    PixelBuffer()
        : width_(0),
          height_(0),
          stride_(0),
          size_(0),
          base_(nullptr),
          end_(nullptr),
          bytes_(0) {        
    }

    PixelBuffer(size_t width,
                size_t height,
                size_t stride,
                size_t bytes)
        : width_(width),
          height_(height),
          stride_(stride),
          size_(stride * height),
          base_(nullptr),
          end_(nullptr),
          bytes_(bytes) {
        assert(bytes_ != 0);
        
        stride_ = (stride_ == 0)? width_ * bytes: stride_;
        size_   = stride_ * height;

        printf("Create pixel buffer %d %d %d\n", width_, height_, stride_);
        
        base_ = alloc_.allocate(size_);
        end_  = base_ + (height - 1) * stride_ + width * bytes;

        printf("Create pixel buffer %x %x\n", base_, end_);
        
        assert(base_ != 0);
        assert(end_ != 0);
    }

    PixelBuffer(const PixelBuffer &other) {
        width_ = other.width_;
        height_ = other.height_;
        stride_ = other.stride_;
        size_ = other.size_;
        bytes_ = other.bytes_;
        base_ = alloc_.allocate(size_);
        end_  = base_ + (height_ - 1) * stride_ + width_ * bytes_;
            
        memcpy(base_, other.base_, size_);
    }

    virtual ~PixelBuffer() {
        alloc_.deallocate(base_, size_);
    }

    size_t width() const {
        return width_;
    }
    
    size_t height() const {
        return height_;
    }

    size_t stride() const {
        return stride_;
    }

    uint8_t* rowAddress(int32_t y) {
        return &base_[y * stride_];
    }

    const uint8_t* rowAddress(int32_t y) const {
        return const_cast<PixelBuffer*>(this)->rowAddress(y);
    }

    const uint8_t* getPixel(int32_t x, int32_t y) const {
        return rowAddress(y) + x * bytes_;
    }

    void setPixel(int32_t x, int32_t y, const uint8_t *pSrc) {
        uint8_t *pDst = rowAddress(y) + x * bytes_;
        memcpy(pDst, pSrc, bytes_);
    }

    PixelBuffer& operator=(const PixelBuffer& other) {
        PixelBuffer temp(other);
        swap(temp);
    }

protected:
    Alloc<uint8_t> alloc_;
    size_t         width_;
    size_t         height_;
    size_t         stride_;
    size_t         size_;
    uint8_t        *base_;
    uint8_t        *end_;
    size_t         bytes_;

private:
    void swap(PixelBuffer& other)
    {
        ::swap(width_, other.width_);
        ::swap(height_, other.height_);
        ::swap(stride_, other.stride_);
        ::swap(size_, other.size_);
        ::swap(base_, other.base_);
        ::swap(end_, other.end_);
        ::swap(bytes_, other.bytes_);
    }
};

template<class T> class PixelIterator
{
public:
    PixelIterator(T       *base, 
                  int32_t width,
                  int32_t stride)
        : base_(base),
          curP_(base),
          curX_(0),
          curY_(0),
          width_(width_),
          stride_(stride) {
    }

    ~PixelIterator() {
    }

    PixelIterator& operator++()
    {
        curX_++;
        curP_++;
        if (curX_ >= width_) {
            curX_ = 0;
            curY_++;
            curP_ = reinterpret_cast<T*>(base_ + curY_ * stride_);
        }
        return *this;
    }
    
private:
    uint8_t *base_;
    T       *curP_;
    int32_t curX_;
    int32_t curY_;
    size_t  width_;
    size_t  stride_;
};

template<typename T>
class PixelStorage: public PixelBuffer {
public:
    typedef PixelIterator<T> iterator;

    PixelStorage() {
    }

    PixelStorage(size_t width,
                 size_t height,
                 size_t stride)
        : PixelBuffer(width,
                      height,
                      stride,
                      T::bytes_per_pixel) {
    }

    virtual ~PixelStorage() {
    }

    T* rowAddress(int32_t y) {
        return reinterpret_cast<T*>(PixelBuffer::rowAddress(y));
    }

    const T* rowAddress (int32_t y) const {
        return reinterpret_cast<const T*>(PixelBuffer::rowAddress(y));
    }
    
    const T& getPixel(int32_t x, int32_t y) const {
        static T pixel;

        const uint8_t* p = PixelBuffer::getPixel(x, y);
        memcpy(&pixel, p, sizeof(T));

        return pixel;
    }

    void setPixel(int x, int y, const T& pixel) {
        const uint8_t* p = reinterpret_cast<const uint8_t*>(&pixel);
        PixelBuffer::setPixel(x, y, p);
    }

    iterator begin () {
        return iterator(reinterpret_cast<T*>(base_), width(), stride());
    }

    const iterator begin () const {
        return iterator(reinterpret_cast<T*>(base_), width(), stride());
    }

    iterator end () 
    {
        return iterator(reinterpret_cast<T*>(end_), width(), stride());
    }

    const iterator end () const
    {
        return iterator(reinterpret_cast<T*>(end_), width(), stride());
    }
};

#endif  // __STORAGE_H__
