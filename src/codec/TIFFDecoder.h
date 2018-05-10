#ifndef __TIFF_DECODER_H__
#define __TIFF_DECODER_H__

#include <unordered_map>
using namespace std;

#include "DecoderBase.h"

class TIFFDecoder: public DecoderBase {
public:
    static unique_ptr<TIFFDecoder> create(const char name[]) {
        unique_ptr<FileInStream> stream = FileInStream::create(name);
        if (stream == nullptr) {
            return nullptr;
        }

        return unique_ptr<TIFFDecoder>(new TIFFDecoder(stream));
    }

    virtual ~TIFFDecoder() {
    }

    template<typename T>
    kDecodeStatus decode(Image<T> &image, size_t width, size_t height) {
        kDecodeStatus status;
 
        status = decodeHeader();
        if (status != kDecodeSuccess) {
            return status;
        }

        //status = decodePixel(temp);
        //if (status != kDecodeSuccess) {
        //    return status;
        //}
        
        return kDecodeSuccess;
    }
    
private:
    enum Tag {
        NewSubfileType = 254,
        SubfileType = 255,
        ImageWidth = 256,
        ImageLength = 257,
        BitsPerSample = 258,
        Compression = 259,
        PhotometricInterpretation = 262,
        Threshholding = 263,
        CellWidth = 264,
        CellLength = 265,
        FillOrder = 266,
        DocumentName = 269,
        ImageDescription = 270,
        Make = 271,
        Model = 272,
        StripOffsets = 273,
        Orientation = 274,
        SamplesPerPixel = 277,
        RowsPerStrip = 278,
        StripByteCounts = 279,
        MinSampleValue = 280,
        MaxSampleValue = 281,
        XResolution = 282,
        YResolution = 283,
        PlanarConfiguration = 284,
        PageName = 285,
        XPosition = 286,
        YPosition = 287,
        FreeOffsets = 288,
        FreeByteCounts = 289,
        GrayResponseUnit = 290,
        GrayResponseCurve = 291,
        T4Options = 292,
        T6Options = 293,
        ResolutionUnit = 296,
        PageNumber = 297,
        TransferFunction = 301,
        Software = 305,
        DateTime = 306,
        Artist = 315,
        HostComputer = 316,
        Predictor = 317,
        WhitePoint = 318,
        PrimaryChromaticities = 319,
        ColorMap = 320,
        HalftoneHints = 321,
        TileWidth = 322,
        TileLength = 323,
        TileOffsets = 324,
        TileByteCounts = 325,
        InkSet = 332,
        InkNames = 333
    };

    TIFFDecoder(unique_ptr<FileInStream>& stream)
        : stream_(move(stream)) {
    }

    class TIFFEntry {
    public:
        static TIFFEntry* create(unique_ptr<FileInStream> &stream) {
            uint16_t tag;
            uint16_t type;
            uint32_t count;
            uint32_t value;

            stream->readU16(tag);
            stream->readU16(type);
            stream->readU32(count);
            stream->readU32(value);

            printf("Tag: %d, type %d, count: %d, value: %d\n",
                tag, type, count, value);
            
            return new TIFFEntry(tag, type, count, value);
        }

        uint32_t getTag() const {
            return tag_;
        }
        
    private:
        TIFFEntry(uint16_t tag,
                  uint16_t type,
                  uint16_t count,
                  uint16_t value)
            : tag_(tag),
              type_(type),
              count_(count),
              value_(value) {
        }
    
        uint16_t tag_;
        uint16_t type_;
        uint32_t count_;
        uint32_t value_;
    };
    
    class TIFFIFD {
    public:
        static TIFFIFD* create(unique_ptr<FileInStream> &stream) {
            TIFFIFD *curIFD = new TIFFIFD();
            
            uint16_t entry;

            stream->readU16(entry);
            while(entry--) {
                TIFFEntry *entry = TIFFEntry::create(stream);
                curIFD->addEntry(entry);
            }
            return curIFD;
        }

    private:
        TIFFIFD() {
        }

        void addEntry(TIFFEntry *entry) {
            entry_[entry->getTag()] = entry;
        }

        unordered_map<uint32_t, TIFFEntry*> entry_;
    };

    kDecodeStatus decodeIFD() {
        uint32_t offset;
        stream_->readU32(offset);
        printf("Root IFD offset 0x%08x\n", offset);

        while(offset) {
            stream_->seekOffset(offset);

            TIFFIFD *curr = TIFFIFD::create(stream_);
            IFDTable_.push_back(curr);

            stream_->readU32(offset);
            printf("Sub IFD offset 0x%08x\n", offset);
        }

        return kDecodeSuccess;
    }

    kDecodeStatus decodeHeader() {
        printf("decodeHeader\n");
        
        uint8_t bytes[4];        
        stream_->readBytes(bytes, 4);
        
        uint32_t magic = (bytes[0] << 24) |
                         (bytes[1] << 16) |
                         (bytes[2] <<  8) |
                         (bytes[3] <<  0);
        if (magic == 0x49492A00) {
            little_ = true;
        } else if (magic == 0x4D4D002A) {
            little_ = false;
        } else {
            printf("Invalid TIFF file\n");
            return kIncorrectType;
        }
        
        stream_->setEndian(little_);
        
        return decodeIFD();
    }

    kDecodeStatus decodePixel(Image<RGB_888> &image) {
        return kDecodeSuccess;
    }

    unique_ptr<FileInStream> stream_;
    bool                     little_;
    vector<TIFFIFD*>         IFDTable_;
};

#endif  // __TIFF_DECODER_H__
