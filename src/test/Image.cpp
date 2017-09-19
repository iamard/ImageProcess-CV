#if 0
#include "Pyramid.h"
#endif

#include "BMPDec.h"
#include "BMPEnc.h"
#include "Image.h"
#include "Convert.h"
#include "Gaussian.h"
#include "Resizer.h"
#include "Color.h"

int main(int argc, char **argv) {
    unique_ptr<BMPDec> decoder = BMPDec::create("test.bmp");
    unique_ptr<BMPEnc> encoder = BMPEnc::create("output.bmp");

    Image<GRAY8> image = decoder->decode<RGB888>();
    
    //encoder->encode<GRAY8>(image);
    
    Image<GRAY8> output = gaussian(image, 1.0, 5);

    encoder->encode<>(output);
    
    Image<GRAY8> output2 = resize(output, 200, 200);

    //encoder->encode<>(output2);

    //Image<rgb> output;
    //int32_t result = decoder->decode<rgb>(output);

    //printf("Convert to gray image\n");
    //Image<gray> output1 = convert<gray, gray>(image);
    
    //printf("Apply gaussian filter\n");
    //Image<gray> output2 = gaussian<gray>(output1, 0.5, 5);

    //printf("Scale image to 200x200\n");
    //Image<gray> output3 = resize<gray>(output2, 200, 200);

    //printf("Create gaussian pyramid\n");
    //Pyramid& pyramid1 = *GPyramid<gray>::create(output3, 0.5, 0.5, 4);

    //printf("Create scale space pyramid\n");
    //Pyramid& pyramid2 = *ScaleSpace<gray>::create(image, 2, 0.5, 0.5, 4);

    //printf("Create DoG pyramid\n");
    //Pyramid& pyramid3 = *DoGPyramid<gray>::create((GPyramid<gray>&)pyramid1);

    return 0;
}
