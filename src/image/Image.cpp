#include "Image.h"
#include "Convert.h"
#include "Gaussian.h"
#include "Resizer.h"

int main(int argc, char **argv) {
    Image<rgb> image = Image<rgb>(100, 100);

    printf("Convert to gray image\n");
    Image<gray> output1 = convert<rgb, gray>(image);
    
    printf("Apply gaussian filter\n");
    Image<gray> output2 = gaussian<gray, 5>(output1, 0.5);

    printf("Scale image to 200x200\n");
    Image<gray> output3 = resizeImiage<gray>(output2, 200, 200);

    //Pyramid pyramid = Gaussian<double>(image, 0.5, 0.5, 4);

    return 0;
}
