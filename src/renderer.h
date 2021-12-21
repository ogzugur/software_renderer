#pragma once
#include "utility_structs.h"
#include "texture.h"
#include "frame.h"

void drawTriangle(ogz_util::VertexData p0,
                  ogz_util::VertexData p1,
                  ogz_util::VertexData p2,
                  Frame &image,
                  Texture &texture);
