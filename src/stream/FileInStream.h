#ifndef __FILE_IN_STREAM_H__
#define __FILE_IN_STREAM_H__

#include "Common.h"

class FileInStream {
public:
    static unique_ptr<FileInStream> create(const char name[]) {
        FILE *file = fopen(name, "rb");
        if (file == nullptr) {
            return nullptr;
        }
        
        return unique_ptr<FileInStream>(new FileInStream(file));
    }

    virtual ~FileInStream() {
        if (file_) {
            fclose(file_);
            file_ == nullptr;
        }
    }

    void setEndian(bool &little) {
        little_ = little;
    }

    int32_t readS8(int8_t &value) {
        return fread(&value, sizeof(value), 1, file_);
    }
    
    int32_t readS16(int16_t &value) {
        uint8_t data[2];
        size_t status = fread(&data, sizeof(uint8_t), 2, file_);
        if (status != 2) {
            return status;
        }
        
        if (little_) {
            value = ((data[1] << 8) | (data[0] << 0));
        } else {
            value = ((data[0] << 8) | (data[1] << 0));
        }

        return status;
    }
    
    int32_t readS32(int32_t &value) {
        uint8_t data[4];
        size_t status = fread(&data, sizeof(uint8_t), 4, file_);
        if (status != 4) {
            return status;
        }
        
        if (little_) {
            value = ((data[3] << 24) | 
                     (data[2] << 16) |
                     (data[1] <<  8) |
                     (data[0] <<  0));
        } else {
            value = ((data[0] << 24) | 
                     (data[1] << 16) |
                     (data[2] <<  8) |
                     (data[3] <<  0));
        }
        
        return status;
    }

    int32_t readU8(uint8_t &value) {
        return fread(&value, sizeof(value), 1, file_);
    }
    
    int32_t readU16(uint16_t &value) {
        uint8_t data[2];
        size_t status = fread(&data, sizeof(uint8_t), 2, file_);
        if (status != 2) {
            return status;
        }
        
        if (little_) {
            value = ((data[1] << 8) | (data[0] << 0));
        } else {
            value = ((data[0] << 8) | (data[1] << 0));
        }

        return status;
    }
    
    int32_t readU32(uint32_t &value) {
        uint8_t data[4];
        size_t status = fread(&data, sizeof(uint8_t), 4, file_);
        if (status != 4) {
            return status;
        }
        
        if (little_) {
            value = ((data[3] << 24) | 
                     (data[2] << 16) |
                     (data[1] <<  8) |
                     (data[0] <<  0));
        } else {
            value = ((data[0] << 24) | 
                     (data[1] << 16) |
                     (data[2] <<  8) |
                     (data[3] <<  0));
        }
        
        return status;
    }

    int32_t readBytes(void *pointer, size_t bytes) {
        return fread(pointer, 1, bytes, file_);
    }

    int32_t skipBytes(size_t bytes) {
        int32_t status = fseek(file_, bytes, SEEK_CUR);
        if (status != 0)
            return status;
        return bytes;
    }

    int32_t seekOffset(uint32_t offset) {
        int32_t status = fseek(file_, offset, SEEK_SET);
        if (status != 0)
            return status;
        return offset;
    }

private:
    FileInStream() {    
    }

    FileInStream(const FileInStream &other) {
    }
    
    FileInStream(FILE *file, bool little = true)
        : file_(file),
          little_(little) {     
    }

    FILE *file_;
    bool little_;
};

#endif  // __FILE_IN_STREAM_H__
