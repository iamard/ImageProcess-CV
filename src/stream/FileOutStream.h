#ifndef __FILE_OUT_STREAM_H_
#define __FILE_OUT_STREAM_H_

#include "Common.h"

class FileOutStream {
public:
    static unique_ptr<FileOutStream> create(const char name[]) {
        FILE *file = fopen(name, "wb");
        if (file == NULL) {
            return 0;
        }
        
        return unique_ptr<FileOutStream>(new FileOutStream(file));
    }

    virtual ~FileOutStream() {
        if (file_) {
            fclose(file_);
            file_ == nullptr;
        }
    }

    int32_t writeS8(int8_t value) {
        return fwrite(&value, sizeof(value), 1, file_);
    }

    int32_t writeS16(int16_t value) {
        return fwrite(&value, sizeof(value), 1, file_);
    }
    
    int32_t writeS32(int32_t value) {
        return fwrite(&value, sizeof(value), 1, file_);
    }

    int32_t writeU8(uint8_t value) {
        return fwrite(&value, sizeof(value), 1, file_);
    }

    int32_t writeU16(uint16_t value) {
        return fwrite(&value, sizeof(value), 1, file_);
    }

    int32_t writeU32(uint32_t value) {
        return fwrite(&value, sizeof(value), 1, file_);
    }

    int32_t writeBytes(void *data, size_t bytes) {
        return fwrite(data, bytes, 1, file_);
    }

private:
    FileOutStream() {    
    }

    FileOutStream(const FileOutStream &other) {
    }
    
    FileOutStream(FILE *file)
        : file_(file) {     
    }
    
    FILE *file_;
};

#endif  // __FILE_OUT_STREAM_H_
