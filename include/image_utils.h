#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include "image.h"
#include <stdio.h>

void process_image(const char *operation, FILE *src_file,
                   const char *dest_path);
Image load_image(const char *operation, FILE *src_file);
void apply_operation(const char *operation, Image img);
void write_image(Image img, const char *dest_path);

#endif // IMAGE_UTILS_H
