#include <iostream>
#include "frame.h"
#include "model.h"
#include "texture.h"

#define IMG_WIDHT 1024
#define IMG_HEIGHT 1024

Frame *frame;
Model dummy_model;
Texture texture;

int main()
{
    frame = new Frame(IMG_WIDHT, IMG_HEIGHT);
    texture = Texture("resources/models/diablo3_pose_diffuse.tga");
    dummy_model = Model("resources/models/diablo.obj");
    dummy_model.draw(*frame, texture);
    frame->writeFile("final_image.png");
}
