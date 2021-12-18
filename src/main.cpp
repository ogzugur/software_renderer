#include <iostream>
#include "frame.h"
#include "model.h"

#define IMG_WIDHT 512  
#define IMG_HEIGHT 512

Frame *frame;
Model dummy_model;

int main()
{
    frame = new Frame(IMG_WIDHT, IMG_HEIGHT);
    dummy_model = Model("resources/models/african_head.obj");
    dummy_model.draw(*frame);
    frame->write_file("final_image.png");
}
