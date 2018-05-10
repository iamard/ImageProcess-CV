#ifndef __BMP_DECODER_H__
#define __BMP_DECODER_H__

#include "DecoderBase.h"
#include "Convert.h"

class BMPDecoder: public DecoderBase
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

    enum kBMPInfoSize {
        kBMPInfoV1Bytes = 40,
        kBMPInfoV2Bytes = 52,
        kBMPInfoV3Bytes = 56,
        kBMPInfoV4Bytes = 108,
        kBMPInfoV5Bytes = 124
    };

    enum Compression {
        BI_RGB       = 0x0000,
        BI_RLE8      = 0x0001,
        BI_RLE4      = 0x0002,
        BI_BITFIELDS = 0x0003,
        BI_JPEG      = 0x0004,
        BI_PNG       = 0x0005,
        BI_CMYK      = 0x000B,
        BI_CMYKRLE8  = 0x000C,
        BI_CMYKRLE4  = 0x000D
    };
    
public:
    static unique_ptr<BMPDecoder> create(const char name[]) {
        unique_ptr<FileInStream> stream = FileInStream::create(name);
        if (stream == nullptr) {
            return nullptr;
        }
        
        return unique_ptr<BMPDecoder>(new BMPDecoder(stream));
    }

    virtual ~BMPDecoder() {
        if (color_) {
            delete[] color_;
        }
    }
    
    template<typename T>
    kDecodeStatus decode(Image<T> &image, size_t width, size_t height) {
        kDecodeStatus status;

        // Create source stream
        status = decodeHeader();
        if (status != kDecodeSuccess) {
            return status;
        }
        
        // Create temp image
        Image<RGB_888> temp(header_.biWidth, header_.biHeight);
        
        printf("%d %d\n", header_.biWidth, header_.biHeight);
        status = decodePixel(temp);
        if (status != kDecodeSuccess) {
            return status;
        }

    #if 0
        if (temp.width() != width || temp.height() != height) {
            temp = resize<RGB_888>(image, width, height);
        }
    #endif // 0
        
        image = temp;

        return kDecodeSuccess;
    }

private:
    BMPDecoder(unique_ptr<FileInStream>& stream)
        : stream_(move(stream)),
          color_(nullptr) {
    }
    
    kDecodeStatus decodeHeader() {
        printf("decodeHeader\n");
        
        uint16_t type;
        stream_->readU16(type);

        if ((type != 0x4142) &&  // BA
            (type != 0x4d42) &&  // BM
            (type != 0x4943) &&  // CI
            (type != 0x5043) &&  // CP
            (type != 0x4349) &&  // IC
            (type != 0x5450)) {  // PT
            assert(0);
            return kIncorrectType;
        }

        uint32_t head;
        stream_->readU32(head);

        // Skip reserved1 and reserved2
        stream_->skipBytes(4);

        uint32_t offset;
        stream_->readU32(offset);

        uint32_t size;
        stream_->readU32(size);
        if (size != kBMPInfoV1Bytes &&
            size != kBMPInfoV2Bytes &&
            size != kBMPInfoV3Bytes &&
            size != kBMPInfoV4Bytes &&
            size != kBMPInfoV2Bytes) {
            return kCorruptStream;        
        }
        
        printf("===> size %d\n", size);
        
        header_.biSize = size;
        if (header_.biSize == 12) {
            int16_t value;

            stream_->readS16(value);
            header_.biWidth = value;
            
            stream_->readS16(value);
            header_.biHeight = value;

            stream_->readS16(header_.biPlanes);
            stream_->readS16(header_.biBitCount);

            header_.biCompression = BI_RGB;
        } else if (header_.biSize == 40) {
            stream_->readS32(header_.biWidth);
            stream_->readS32(header_.biHeight);
            stream_->readS16(header_.biPlanes);
            stream_->readS16(header_.biBitCount);
            
            if (header_.biSize > 16) {            
                stream_->readS32(header_.biCompression);
                stream_->readS32(header_.biSizeImage);
                stream_->readS32(header_.biXPelsPerMeter);
                stream_->readS32(header_.biYPelsPerMeter);
                if (header_.biSize > 32) {
                    stream_->readS32(header_.biClrUsed);
                    stream_->readS32(header_.biClrImportant);
                }
            }
        } else {
            return kCorruptStream;
        }

    if (header_.biWidth < 0 || header_.biCompression > 3) {
            printf("Unsupported compression type\n");
            return kCorruptStream;
        }
        
        if (header_.biHeight < 0) {
            header_.biHeight = -header_.biHeight;
        }
        
        int32_t count = 0;
        int32_t read  = 0;
        if (header_.biBitCount < 16) {
            if (header_.biClrUsed) {
                count = header_.biClrUsed;
            } else {
                count = 1 << header_.biBitCount;
            }

            if(count != 0) {
                color_ = new BGRA_8888[count];
                stream_->readBytes(color_, count * sizeof(BGRA_8888));
                read += count * sizeof(BGRA_8888);
            }
        } else if ((header_.biCompression == BI_BITFIELDS) &&
                   ((header_.biBitCount == 16) || (header_.biBitCount == 32))) {
            if (header_.biSize == 40) {
                stream_->readU32(mask_[0]);
                stream_->readU32(mask_[1]);
                stream_->readU32(mask_[2]);
                read += 12;
            } else {
                stream_->readU32(mask_[0]);
                stream_->readU32(mask_[1]);
                stream_->readU32(mask_[2]);
                stream_->readU32(mask_[3]);
                read += 16;
            }
        }

        // Offset to the pixel data
        offset_ = offset - 14 - size - read;

        return kDecodeSuccess;
    }

    kDecodeStatus decodePixel(Image<RGB_888> &image) {
        printf("decodePixel %d\n", offset_);
        
        stream_->skipBytes(offset_);
        
        if (header_.biBitCount == 1) {
            return decodePAL1(image);
        } else if (header_.biBitCount == 4) {
            if(header_.biCompression == BI_RLE4) {
                return decodeRLE4(image);
            } else {
                return decodePAL4(image);
            }
        } else if (header_.biBitCount == 8) {
            if (header_.biCompression == BI_RLE8) {
                return decodeRLE8(image);
            } else {
                return decodePAL8(image);
            }
        } else if (header_.biBitCount == 16) {
            if(header_.biCompression == BI_BITFIELDS) {
                return decodeMask16(image);
            } else {
                return decodeBGR16(image);
            }                
        } else if (header_.biBitCount == 24){
            if(header_.biCompression == BI_BITFIELDS) {
                assert(0);
            } else {
                return decodeBGR24(image);
            }
        } else if (header_.biBitCount == 32) {
            if(header_.biCompression == BI_BITFIELDS) {
                assert(0);
            } else {
                return decodeBGR32(image);
            }
        }

        return kDecodeSuccess;
    }

    kDecodeStatus decodePAL1(Image<RGB_888> &image) {
        size_t  width  = image.width();
        size_t  height = image.height();
        size_t  padded = ((width + 31) / 32) * 4 - (width + 7) / 8;
        uint8_t value;

        printf("decode PAL1 %d %d %d\n", width, height, padded);
        for (int32_t y = height - 1; y >= 0; y--) {
            for (int32_t x = 0; x < width;) {
                if (stream_->readU8(value) != 1) {
                    return kCorruptStream;
                }

                for (int32_t i = 0; i < 8 && x < width; i++, x++) {
                    uint32_t index = (value & 0x80)? 0x1: 0x0;
                    uint8_t  red   = color_[index].red;
                    uint8_t  green = color_[index].green;
                    uint8_t  blue  = color_[index].blue;
                    image.setPixel(x, y, RGB_888(red, green, blue));
                    value <<= 1;
                }
            }

            if (padded) {
                if (stream_->skipBytes(padded) != padded) {
                    return kCorruptStream;
                }
            }
        }

        return kDecodeSuccess;
    }

    kDecodeStatus decodeRLE4(Image<RGB_888> &image) {
        size_t  width  = image.width();
        size_t  height = image.height();
        int32_t curY   = image.height() - 1;
        int32_t curX;
        uint8_t count;
        uint8_t index;

        printf("decode RLE4 %d %d\n", width, height);
        while(curY >= 0) {
		    curX = 0;
		    while(curX < width) {
			    stream_->readU8(count);

			    if(0 != count) {
				    stream_->readU8(index);

				    while(count--) {
                        uint8_t red   = color_[(index >> 4) & 0x0F].red;
                        uint8_t green = color_[(index >> 4) & 0x0F].green;
                        uint8_t blue  = color_[(index >> 4) & 0x0F].blue;
                        image.setPixel(curX, curY, RGB_888(red, green, blue));
					    curX++;
					    if(count > 0) {
                            red   = color_[(index >> 0) & 0x0F].red;
                            green = color_[(index >> 0) & 0x0F].green;
                            blue  = color_[(index >> 0) & 0x0F].blue;
                            image.setPixel(curX, curY, RGB_888(red, green, blue));
						    curX++;
						    count--;
					    }
				    }
			    } else {
				    stream_->readU8(count);
				    switch(count) {
					    case 0:
						    curX = width;
						    curY--;
						    break;
					    case 1:
						    curX = width;
						    curY = -1;
						    break;
					    case 2:
						    stream_->readU8(count);
						    curX += count;
						    stream_->readU8(count);
						    curY -= count;
						    break;
					    default:
						    uint8_t skip = 0;
						    if((1 == (count & 3)) || (2 == (count & 3)))
							    skip = 1;

						    while(count--) {
                                stream_->readU8(index);
                                uint8_t red   = color_[(index >> 4) & 0x0F].red;
                                uint8_t green = color_[(index >> 4) & 0x0F].green;
                                uint8_t blue  = color_[(index >> 4) & 0x0F].blue;
                                image.setPixel(curX, curY, RGB_888(red, green, blue));
							    curX++;
							    if(0 < count) {
								    red   = color_[(index >> 0) & 0x0F].red;
                                    green = color_[(index >> 0) & 0x0F].green;
                                    blue  = color_[(index >> 0) & 0x0F].blue;
                                    image.setPixel(curX, curY, RGB_888(red, green, blue));
								    curX++;
								    count--;
							    }
						    }

						    if(skip)
							    stream_->skipBytes(1);
					 	    break;
				    }
			    }
		    }
	    }

        return kDecodeSuccess;
    }

    kDecodeStatus decodePAL4(Image<RGB_888> &image) {
        size_t  width  = image.width();
        size_t  height = image.height();
        size_t  padded = (4 - (((width % 8) + 1) / 2)) & 3;
        uint8_t value;

        printf("decode PAL4 %d %d %d\n", width, height, padded);
        for (int32_t y = height - 1; y >= 0; y--) {
            for (int32_t x = 0; x < width;) {
                if (stream_->readU8(value) != 1) {
                    return kCorruptStream;
                }
                
                uint32_t index = (value >> 4) & 0x0F;
                uint8_t  red   = color_[index].red;
                uint8_t  green = color_[index].green;
                uint8_t  blue  = color_[index].blue;
                uint8_t  alpha = color_[index].alpha;
                image.setPixel(x, y, RGB_888(red, green, blue)); 
                x++;

                if (x < width) {
                    index = value & 0x0F;
                    red   = color_[index].red;
                    green = color_[index].green;
                    blue  = color_[index].blue;
                    image.setPixel(x, y, RGB_888(red, green, blue));
                    x++;
                }
            }

            if (padded) {
                if (stream_->skipBytes(padded) != padded) {
                    return kCorruptStream;
                }
            }
        }

        return kDecodeSuccess;
    }

    kDecodeStatus decodeRLE8(Image<RGB_888> &image) {
        size_t  width  = image.width();
        size_t  height = image.height();
        int32_t curY   = image.height() - 1;
        int32_t curX;
        uint8_t count;
        uint8_t index;

        printf("decode RLE8 %d %d\n", width, height);
        while(curY >= 0) {
		    curX = 0;
		    while(curX < width) {
			    stream_->readU8(count);

			    if(0 != count) {
				    stream_->readU8(index);
				    while(count--) {
                        uint8_t red   = color_[index].red;
                        uint8_t green = color_[index].green;
                        uint8_t blue  = color_[index].blue;
					    image.setPixel(curX, curY, RGB_888(red, green, blue));
					    curX++;
				    }
			    } else {
				    stream_->readU8(count);
				    switch(count) {
					    case 0:
						    curX = width;
						    curY--;
						    break;
					    case 1:
						    curX = width;
						    curY = -1;
						    break;
					    case 2:
						    stream_->readU8(count);
						    curX += count;
						    stream_->readU8(count);
						    curY -= count;
					        break;
					    default:
                            uint8_t skip = count & 1;
						    while(count--) {
                                stream_->readU8(index);

                                uint8_t red   = color_[index].red;
                                uint8_t green = color_[index].green;
                                uint8_t blue  = color_[index].blue;
					            image.setPixel(curX, curY, RGB_888(red, green, blue));
					            curX++;
				            }
						    if(skip)
							    stream_->skipBytes(1);
						    break;
				    }
			    }
		    }
	    }
 
	    return kDecodeSuccess;
    }

    kDecodeStatus decodePAL8(Image<RGB_888> &image) {
        size_t  width  = image.width();
        size_t  height = image.height();
        size_t  padded = (4 - (width % 4)) & 3;
        uint8_t index;
 
        printf("decode PAL8 %d %d %d\n", width, height, padded);
        for (int32_t y = height - 1; y >= 0; y--) {
            for (int32_t x = 0; x < width; x++) {
                if (stream_->readU8(index) != 1) {
                    return kCorruptStream;
                }

                uint8_t  red   = color_[index].red;
                uint8_t  green = color_[index].green;
                uint8_t  blue  = color_[index].blue;
                image.setPixel(x, y, RGB_888(red, green, blue));
            }

            if (padded) {
                if (stream_->skipBytes(padded) != padded) {
                    return kCorruptStream;
                }
            }
        }
        
        return kDecodeSuccess;
    }

    kDecodeStatus decodeMask16(Image<RGB_888> &image) {
        size_t  width  = image.width();
        size_t  height = image.height();
        size_t  padded = 2; //(4 - (width % 4)) & 3;
        uint8_t value[2];
        int32_t shift[3];

        printf("decode MASK16 %d %d %d\n", width, height, padded);
        if (mask_[0] == 0xF800 && mask_[1] == 0x07E0 && mask_[2] == 0x001F) {
            shift[0] = 8;
            shift[1] = 3;
            shift[2] = -3;
        } else if (mask_[0] == 0x7C00 && mask_[1] == 0x03E0 && mask_[2] == 0x001F) {
            shift[0] = 7;
            shift[1] = 3;
            shift[2] = -3;
        } else if (mask_[0] == 0x0F00 && mask_[1] == 0x00F0 && mask_[2] == 0x000F) {
            shift[0] = 4;
            shift[1] = 0;
            shift[2] = -4;
        } else {
            assert(0);
            return kUnknownFormat;
        }

         for (int32_t y = height - 1; y >= 0; y--) {
            for (int32_t x = 0; x < width; x++) {
                if (stream_->readBytes(value, 2) != 2) {
                    assert(0);
                    return kCorruptStream;
                }
                uint16_t pixel = (value[1] << 8) | value[0];
                
                uint8_t red   = (pixel & mask_[0]) >> shift[0];
                uint8_t green = (pixel & mask_[1]) >> shift[1];
                uint8_t blue  = (shift[2] < 0)? (pixel & mask_[2]) << -shift[2]: (pixel & mask_[2]) >> shift[2];
                image.setPixel(x, y, RGB_888(red, green, blue));
            }

            if (padded) {
                if (stream_->skipBytes(padded) != padded) {
                    return kCorruptStream;
                }
            }
        }
 
        return kDecodeSuccess;
    }
    
    kDecodeStatus decodeBGR16(Image<RGB_888> &image) {
        size_t  width  = image.width();
        size_t  height = image.height();
        size_t  padded = 2; //(4 - (width % 4)) & 3;
        uint8_t value[2];
        
        printf("decode BGR16 %d %d %d\n", width, height, padded);
        for (int32_t y = height - 1; y >= 0; y--) {
            for (int32_t x = 0; x < width; x++) {
                if (stream_->readBytes(value, 2) != 2) {
                    return kCorruptStream;
                }

                uint8_t  blue  = ((value[0] << 3) & 0xF8);
                uint8_t  green = (((value[1] << 6) +
				                   (value[0] >> 2)) & 0xF8);
                uint8_t  red   = ((value[1] << 1) & 0xF8);
                image.setPixel(x, y, RGB_888(red, green, blue));
            }

            if (padded) {
                if (stream_->skipBytes(padded) != padded) {
                    return kCorruptStream;
                }
            }
        }

        return kDecodeSuccess;
    }

    kDecodeStatus decodeBGR24(Image<RGB_888> &image) {
        size_t  width  = image.width();
        size_t  height = image.height();
        size_t  padded = (4 - ((width * 3) % 4)) & 3;
        uint8_t value[3];
        
        printf("decode BGR24 %d %d %d\n", width, height, padded);
        for (int32_t y = height - 1; y >= 0; y--) {
            for (int32_t x = 0; x < width; x++) {
                if (stream_->readBytes(value, 3) != 3) {
                    return kCorruptStream;
                }
                image.setPixel(x, y, RGB_888(value[2], value[1], value[0]));
            }

            if (padded) {
                if (stream_->skipBytes(padded) != padded) {
                    return kCorruptStream;
                }
            }
        }
        return kDecodeSuccess;
    }

    kDecodeStatus decodeBGR32(Image<RGB_888> &image) {
        size_t  width  = image.width();
        size_t  height = image.height();
        uint8_t value[4];
        
        printf("decode BGR32 %d %d\n", width, height);
        for (int32_t y = height - 1; y >= 0; y--) {
            for (int32_t x = 0; x < width; x++) {
                if (stream_->readBytes(value, 4) != 4) {
                    return kCorruptStream;
                }
                image.setPixel(x, y, RGB_888(value[2], value[1], value[0]));
            }
        }
        return kDecodeSuccess;
    }

    unique_ptr<FileInStream> stream_;
    BitmapInfoHeader         header_;
    BGRA_8888                *color_;
    uint32_t                 mask_[4];
    uint32_t                 offset_;
};

#endif  // __BMP_DECODER_H__
