#ifndef __FILE_IN_STREAM_H__
#define __FILE_IN_STREAM_H__

#include "Common.h"

class FileInStream {
public:
    static unique_ptr<FileInStream> create(const char name[]) {
        FILE *file = fopen(name, "rb");
        if (file == NULL) {
            return 0;
        }
        
        return unique_ptr<FileInStream>(new FileInStream(file));
    }

    virtual ~FileInStream() {
        if (file_) {
            fclose(file_);
            file_ == nullptr;
        }
    }

    int32_t readS8(int8_t &value) {
        return fread(&value, sizeof(value), 1, file_);
    }
    
    int32_t readS16(int16_t &value) {
        return fread(&value, sizeof(value), 1, file_);
    }
    
    int32_t readS32(int32_t &value) {
        return fread(&value, sizeof(value), 1, file_);
    }

    int32_t readU8(uint8_t &value) {
        return fread(&value, sizeof(value), 1, file_);
    }
    
    int32_t readU16(uint16_t &value) {
        return fread(&value, sizeof(value), 1, file_);
    }
    
    int32_t readU32(uint32_t &value) {
        return fread(&value, sizeof(value), 1, file_);
    }

    int32_t readBytes(void *pointer, size_t bytes) {
        return fread(pointer, bytes, 1, file_);
    }

    int32_t skipBytes(size_t bytes) {
        return fseek(file_, bytes, SEEK_CUR);
    }

private:
    FileInStream() {    
    }

    FileInStream(const FileInStream &other) {
    }
    
    FileInStream(FILE *file)
        : file_(file) {     
    }
    
    FILE *file_;
};

#endif  // __FILE_IN_STREAM_H__
