#ifndef __DECODER_BASE_H__
#define __DECODER_BASE_H__

#include "FileInStream.h"
#include "Image.h"

class DecoderBase {
public:
    enum kDecodeStatus {
        kDecodeSuccess = 0,
        kIncorrectType = -1,
        kInvalidStream = -2,
        kCorruptStream = -3,
        kUnknownFormat = -4
    };
};

#endif  // __DECODER_BASE_H__
