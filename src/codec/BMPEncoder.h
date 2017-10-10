#ifndef __BMP_ENC_H__
#define __BMP_ENC_H__

#include "FileOutStream.h"
#include "Image.h"

class BMPEncoder
{
    typedef struct BitmapFileHeader {
        uint16_t bfType;
        uint32_t bfSize;
        uint16_t bfReserved1;
        uint16_t bfReserved2;
        uint32_t bfOffBits;
    } BitmapFileHeader;

public:
    virtual ~BMPEncoder() {
    }

    static unique_ptr<BMPEncoder> create(const char name[]) {
        unique_ptr<FileOutStream> stream = FileOutStream::create(name);
        if (stream == nullptr) {
            return nullptr;
        }

        return unique_ptr<BMPEncoder>(new BMPEncoder(stream));
    }

    template<typename T>
    int32_t encode(const Image<T> &image) {
        size_t width  = image.width();
        size_t height = image.height();
        size_t depth  = 24;
        size_t stride = ((depth * width + 31) / 32) * 4;
        size_t padded = stride - depth * width / 8; 
        
        BitmapFileHeader header;
        header.bfType = 0x4d42;
        header.bfSize = 14 + 40 + stride * height;
        header.bfReserved1 = 0;
        header.bfReserved2 = 0;
        header.bfOffBits = 14 + 40;

        stream_->writeU16(header.bfType);
        stream_->writeU32(header.bfSize);
        stream_->writeU16(header.bfReserved1);
        stream_->writeU16(header.bfReserved2);
        stream_->writeU32(header.bfOffBits);
        stream_->writeU32(40);
        stream_->writeU32(width);
        stream_->writeU32(height);
        stream_->writeU16(1);
        stream_->writeU16(24);
        stream_->writeU32(0);
        stream_->writeU32(0);
        stream_->writeU32(300);
        stream_->writeU32(300);
        stream_->writeU32(0);
        stream_->writeU32(0);

        if (typeid(T) == typeid(GRAY_8)) {
            return writeGray(image, width, height, padded); 
        }     
        else if (typeid(T) == typeid(RGB_888)) {
            return writeRGB(image, width, height, padded);
        }

        return -1;
    }

private:
    BMPEncoder(unique_ptr<FileOutStream>& stream)
        : stream_(move(stream)) {
    }

    int32_t writeGray(const Image<GRAY_8> &image,
                      size_t             width,
                      size_t             height,
                      size_t             padded) {
        for (int32_t y = height - 1; y >= 0; y--) {
            for (int32_t x = 0; x < width; x++) {
                GRAY_8 pixel = image.getPixel(x, y);
                stream_->writeU8(pixel.value);
                stream_->writeU8(pixel.value);
                stream_->writeU8(pixel.value);
            }

            if (padded) {
                uint8_t dummy[padded];
                stream_->writeBytes(dummy, padded);
            }
        }
    }

    int32_t writeRGB(const Image<RGB_888> &image,
                     size_t              width,
                     size_t              height,
                     size_t              padded) {
        for (int32_t y = height - 1; y >= 0; y--) {
            for (int32_t x = 0; x < width; x++) {
                RGB_888 pixel = image.getPixel(x, y);
                stream_->writeU8(pixel.blue);
                stream_->writeU8(pixel.green);
                stream_->writeU8(pixel.red);
            }

            if (padded) {
                uint8_t dummy[padded];
                stream_->writeBytes(dummy, padded);
            }
        }
    }

    unique_ptr<FileOutStream> stream_;
};

#endif  // __BMP_ENC_H__
