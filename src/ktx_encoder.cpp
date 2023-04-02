#include "ktx_encoder.h"

#include "ktx.h"
#include "image.hpp"

#include <stdio.h>

unsigned char* ktxEncode(const char* raw_data, int width, int height, int* encoded_data_size) {
    printf("Hello\n");
    int componentCount = 1;
    int w = 512;
    int h = 512;
    unsigned char * imageData = (unsigned char *)malloc(w * h * componentCount * sizeof(unsigned char));
    Image* image = nullptr;
    switch (componentCount) {
      case 1: {
        image = new r8image(w, h, (r8color*)imageData);
        image->setColortype(Image::eLuminance);
        break;
      } case 3: {
        image = new rgb8image(w, h, (rgb8color*)imageData);
        image->setColortype(Image::eRGB);
        break;
      }
    }

    // All JPEG images are sRGB.
    image->setOetf(KHR_DF_TRANSFER_SRGB);
    return nullptr;
}
