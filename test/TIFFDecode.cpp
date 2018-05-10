#include "TestBase.h"
#include "TIFFDecoder.h"

TEST_CASE(TIFF_test, "Decode TIFF file test") {
    unique_ptr<TIFFDecoder> decoder = 
        TIFFDecoder::create("cramps-tile.tif");
    verify(decoder != nullptr, "Create decoder failed\n");
        
        
    Image<RGB_888> image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    
    verify(true, "Test TIFF decoder failed\n");
}
