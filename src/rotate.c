#include "rotate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rotate_image_clockwise(Image *img) {
  int width = img->width;
  int height = img->height;
  int channels = img->channels;

  int new_width = height;
  int new_height = width;

  unsigned char *rotated = malloc(width * height * channels);
  if (!rotated) {
    fprintf(stderr, "Failed to allocate memory for rotated image\n");
    return;
  }

  for (int old_y = 0; old_y < height; old_y++) {
    for (int old_x = 0; old_x < width; old_x++) {

      int new_x = new_width - 1 - old_y;
      int new_y = old_x;

      unsigned char *src_px = img->data + (old_y * width + old_x) * channels;

      unsigned char *dst_px = rotated + (new_y * new_width + new_x) * channels;

      memcpy(dst_px, src_px, channels);
    }
  }

  memcpy(img->data, rotated, width * height * channels);
  free(rotated);

  img->width = new_width;
  img->height = new_height;
}
