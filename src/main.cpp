#include <iostream>
#include "image.h"
#define IMG_WIDHT  8
#define IMG_HEIGHT 8

int main()
{
    Image img1 = Image(IMG_WIDHT, IMG_HEIGHT);
    for (size_t i = 0; i <IMG_HEIGHT; i++)
    {
        for (size_t j = 0; j < IMG_WIDHT; j++)
        {
            img1.set_pixel(i,j,(float)i/(IMG_WIDHT-1) * 255,(float)j/(IMG_WIDHT-1) * 255,255);
        }
    }

    img1.write_file("red_image.jpeg");
}
