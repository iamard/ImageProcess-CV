#include "BMPDecoder.h"
#include "BMPEncoder.h"
#include "TestBase.h"

#if 0
TEST_CASE(BMP_PAL1, "Decode bitmap PAL1 file test") {
    unique_ptr<BMPDecoder> decoder = 
        BMPDecoder::create("pal1.bmp");
    unique_ptr<BMPEncoder> encoder = 
        BMPEncoder::create("pal1o.bmp");
    
    Image<RGB_888> image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);
    
    decoder = BMPDecoder::create("pal1bg.bmp");
    encoder = BMPEncoder::create("pal1o1.bmp");
    
    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);

    decoder = BMPDecoder::create("pal1wb.bmp");
    encoder = BMPEncoder::create("pal1o2.bmp");
    
    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);
    
    verify(true, "Test bitmap PAL1 failed\n");
}

TEST_CASE(BMP_RLE4, "Decode bitmap RLE4 file test") {
    unique_ptr<BMPDecoder> decoder = 
        BMPDecoder::create("pal4rle.bmp");
    unique_ptr<BMPEncoder> encoder = 
        BMPEncoder::create("pal4rleo.bmp");
    
    Image<RGB_888> image(127, 64);
    
    decoder->decode<RGB_888>(image, 127, 64);
    
    encoder->encode<RGB_888>(image);
    
    verify(true, "Test bitmap RLE4 failed\n");
}

TEST_CASE(BMP_PAL4, "Decode bitmap PAL4 file test") {
    unique_ptr<BMPDecoder> decoder = 
        BMPDecoder::create("pal4.bmp");
    unique_ptr<BMPEncoder> encoder = 
        BMPEncoder::create("pal4o.bmp");
    
    Image<RGB_888> image(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);

    decoder = BMPDecoder::create("pal4gs.bmp");
    encoder = BMPEncoder::create("pal4o1.bmp");
    
    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);

    verify(true, "Test bitmap PAL4 failed\n");
}

TEST_CASE(BMP_RLE8, "Decode bitmap RLE8 file test") {
    unique_ptr<BMPDecoder> decoder = 
        BMPDecoder::create("pal8rlecut.bmp");
    unique_ptr<BMPEncoder> encoder = 
        BMPEncoder::create("pal8rleo.bmp");
    
    Image<RGB_888> image(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);
    
    verify(true, "Test bitmap RLE8 failed\n");
}

TEST_CASE(BMP_PAL8, "Decode bitmap PAL8 file test") {
    unique_ptr<BMPDecoder> decoder = 
        BMPDecoder::create("pal8.bmp");
    unique_ptr<BMPEncoder> encoder = 
        BMPEncoder::create("pal8o.bmp");
    
    Image<RGB_888> image(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);
    
    decoder = BMPDecoder::create("pal8-0.bmp");
    encoder = BMPEncoder::create("pal8o1.bmp");
    
    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);

    printf("test1\n");
    decoder = BMPDecoder::create("pal8gs.bmp");
    encoder = BMPEncoder::create("pal8o2.bmp");

    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);

#if 1
    printf("test3\n");
    decoder = BMPDecoder::create("pal8os2.bmp");
    encoder = BMPEncoder::create("pal8o3.bmp");
    
    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);

    decoder = BMPDecoder::create("pal8topdown.bmp");
    encoder = BMPEncoder::create("pal8o4.bmp");

    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);
#endif // 0
    
    decoder = BMPDecoder::create("pal8v4.bmp");
    encoder = BMPEncoder::create("pal8o5.bmp");

    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);

    decoder = BMPDecoder::create("pal8v5.bmp");
    encoder = BMPEncoder::create("pal8o6.bmp");

    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);

    decoder = BMPDecoder::create("pal8w124.bmp");
    encoder = BMPEncoder::create("pal8o7.bmp");

    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);

    decoder = BMPDecoder::create("pal8w125.bmp");
    encoder = BMPEncoder::create("pal8o8.bmp");

    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);

    decoder = BMPDecoder::create("pal8w126.bmp");
    encoder = BMPEncoder::create("pal8o8.bmp");

    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);
    
    verify(true, "Test bitmap PAL8 failed\n");
}

TEST_CASE(BMP_BGR16, "Decode bitmap BGR16 file test") {
    unique_ptr<BMPDecoder> decoder = 
        BMPDecoder::create("rgb16.bmp");
    unique_ptr<BMPEncoder> encoder = 
        BMPEncoder::create("rgb16o.bmp");
    
    Image<RGB_888> image(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);

    decoder = BMPDecoder::create("rgb16-565.bmp");
    encoder = BMPEncoder::create("rgb16o1.bmp");
    
    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);

    decoder = BMPDecoder::create("rgb16-565pal.bmp");
    encoder = BMPEncoder::create("rgb16o2.bmp");
    
    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);

    decoder = BMPDecoder::create("rgb16bfdef.bmp");
    encoder = BMPEncoder::create("rgb16o3.bmp");
    
    image = Image<RGB_888>(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);
    
    verify(true, "Test bitmap BGR16 failed\n");
}

TEST_CASE(BMP_BGR24, "Decode bitmap BGR24 file test") {
    unique_ptr<BMPDecoder> decoder = 
        BMPDecoder::create("rgb24.bmp");
    unique_ptr<BMPEncoder> encoder = 
        BMPEncoder::create("rgb24o.bmp");
    
    Image<RGB_888> image(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);
    
    verify(true, "Test bitmap BGR24 failed\n");
}

TEST_CASE(BMP_BGR32, "Decode bitmap BGR32 file test") {
    unique_ptr<BMPDecoder> decoder = 
        BMPDecoder::create("rgb32.bmp");
    unique_ptr<BMPEncoder> encoder = 
        BMPEncoder::create("rgb32o.bmp");
    
    Image<RGB_888> image(127, 64);
    decoder->decode<RGB_888>(image, 127, 64);
    encoder->encode<RGB_888>(image);
    
    verify(true, "Test bitmap BGR32 failed\n");
}
#endif