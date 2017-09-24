#ifndef __BMP_DEC_H__
#define __BMP_DEC_H__

#include "FileInStream.h"
#include "Image.h"
#include "Convert.h"

class BMPDec
{
    typedef struct BitmapCoreHeader {
        uint32_t bcSize;
        int16_t  bcWidth;
        int16_t  bcHeight;
        int16_t  bcPlanes;
        int16_t  bcBitCount;
    } BitmapCoreHeader;
    
    typedef struct BitmapInfoHeader {
        uint32_t biSize;
        int32_t  biWidth;
        int32_t  biHeight;
        int16_t  biPlanes;
        int16_t  biBitCount;
        int32_t  biCompression;
        int32_t  biSizeImage;
        int32_t  biXPelsPerMeter;
        int32_t  biYPelsPerMeter;
        int32_t  biClrUsed;
        int32_t  biClrImportant;
    } BitmapInfoHeader;

public:
    virtual ~BMPDec() {
    }

    static unique_ptr<BMPDec> create(const char name[]) {
        unique_ptr<FileInStream> stream = FileInStream::create(name);
        if (stream == nullptr) {
            return nullptr;
        }
        
        return unique_ptr<BMPDec>(new BMPDec(stream));
    }

    template<typename T>
    Image<T> decode() {
        if (readHeader() >= 0) {
            int32_t width  = header_.biWidth;
            int32_t height = header_.biHeight;
            int32_t depth  = header_.biBitCount;
            int32_t stride = ((depth * width + 31) / 32) * 4;
            int32_t padded = stride - depth * width / 8;
            if (depth == 8) {
                return readGray(width, height, stride, padded);
            } else if (depth == 24){
                return readRGB(width, height, stride, padded);
            }
        }

        return Image<T>();
    }

private:
    BMPDec(unique_ptr<FileInStream>& stream)
        : stream_(move(stream)) {
    }

    int32_t readHeader() {
        uint16_t type;
        stream_->readU16(type);
        if (type != 0x4d42) {
            return -1;
        }

        uint32_t head;
        stream_->readU32(head);

        // Skip reserved1 and reserved2
        stream_->skipBytes(4);

        uint32_t offset;
        stream_->readU32(offset);
      
        uint32_t size;
        stream_->readU32(size);

        bool palet = false;
        if (size == 40 || size == 12) {
            header_.biSize = size;
            if (size == sizeof(BitmapCoreHeader)) {
                palet = true;
            } else {
                stream_->readS32(header_.biWidth);
                stream_->readS32(header_.biHeight);
                stream_->readS16(header_.biPlanes);
                stream_->readS16(header_.biBitCount);
            }
            if (!palet) {
                stream_->readS32(header_.biCompression);
                stream_->readS32(header_.biSizeImage);
                stream_->readS32(header_.biXPelsPerMeter);
                stream_->readS32(header_.biYPelsPerMeter);
                stream_->readS32(header_.biClrUsed);
                stream_->readS32(header_.biClrImportant);
            }
        } else {
            printf("Corrupt bitmap file\n", size);
            return -1;
        }

        int32_t depth  = header_.biBitCount;
        if (depth != 8 && depth != 24) {
            printf("Unsupported depth %d\n", depth);
            return -1;
        }

        if (header_.biCompression != 0) {
            printf("Unsupported compressed format %d\n", header_.biCompression);
            return -1;
        }

        int32_t colors;
        if (header_.biClrUsed == 0) {
            switch (header_.biBitCount) {
            case 1:
                colors = 2;
                break;
            case 4:
                colors = 16;
                break;
            case 8:
                colors = 256;
                break;
            default:
                colors = 0;
                break;
           }
        } else {
            colors = header_.biClrUsed;
        }

        if (!palet) {
            printf("TBD Not palet %d \n", colors); 
        } else {
            printf("TBD With palet\n");    
        }
        
        return 0;
    }

    Image<GRAY8> readGray(size_t width, size_t height, size_t stride, size_t padded) {
        Image<GRAY8> image(width, height, stride);
        for (int32_t y = height - 1; y >= 0; y--) {
            for (int32_t x = 0; x < width; x++) {
                uint8_t gray;
                gray = stream_->readU8(gray);
                image.setPixel(x, y, GRAY8(gray));
            }
            if (padded) {
                stream_->skipBytes(padded);
            }
        }
        return image;
    }

    Image<RGB888> readRGB(size_t width, size_t height, size_t stride, size_t padded) {
        Image<RGB888> image(width, height, stride);
        for (int32_t y = height - 1; y >= 0; y--) {
            for (int32_t x = 0; x < width; x++) {
                uint8_t red, green, blue;
                stream_->readU8(blue);
                stream_->readU8(green);
                stream_->readU8(red);
                image.setPixel(x, y, RGB888(red, green, blue));
            }
            if (padded) {
                stream_->skipBytes(padded);
            }
        }
        return image;
    }
    
    unique_ptr<FileInStream> stream_;
    BitmapInfoHeader         header_;
};

#endif  // __BMP_DEC_H__
