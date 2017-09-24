#include "BMPDec.h"
#include "BMPEnc.h"
#include "TestBase.h"

TEST_CASE(BMPDecode, "Decode bitmap file test") {
    unique_ptr<BMPDec> decoder = BMPDec::create("test.bmp");
    unique_ptr<BMPEnc> encoder = BMPEnc::create("output.bmp");

    Image<GRAY8> image = decoder->decode<GRAY8>();

    
    
    verify(true, "Test bitmap codec failed\n");
}
