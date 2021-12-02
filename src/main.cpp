#include <iostream>
#include "image.h"
#include "model.h"

#define IMG_WIDHT 800
#define IMG_HEIGHT 800

ColorRGB color_red = ColorRGB(255, 0, 0);
ColorRGB color_black = ColorRGB(0, 0, 0);
Image *img1;
Model dummy_model;



int main()
{
    img1 = new Image(IMG_WIDHT, IMG_HEIGHT);
    dummy_model = Model("african_head.obj");
    dummy_model.draw(img1,color_red);
    img1->write_file("red_image.jpeg");
}
