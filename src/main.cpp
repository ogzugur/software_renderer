#include <iostream>
#include "frame.h"
#include "model.h"
#include "texture.h"
#include "renderer.h"

#define IMG_WIDHT 1024
#define IMG_HEIGHT 1024

int main()
{
    Camera camera = Camera(IMG_WIDHT,IMG_HEIGHT);
    Frame *frame = new Frame(IMG_WIDHT, IMG_HEIGHT);
    Renderer renderer = Renderer(camera,*frame);
    Texture texture = Texture("resources/models/african_head_diffuse.tga");
    Model dummy_model = Model("resources/models/african_head.obj");
    dummy_model.texture = &texture;
    renderer.drawModel(dummy_model);
    frame->writeFile("final_image.png", true);
    delete frame;
}
