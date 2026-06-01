#pragma once

#include <stddef.h>

#include "image.h"

void add_padding(unsigned char* wip_img, size_t img_size, size_t filter_size);
void apply_sobel(Image* img);
