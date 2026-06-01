#include "sobel_filter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: in future the user should be able to choose the type of padding, now for simplicity
// I shall use zero-padding
void add_padding(unsigned char* wip_img, size_t img_size, size_t filter_size) {
  size_t padding_buffer = img_size + filter_size - 1;
  unsigned char* padded_img = calloc(padding_buffer, sizeof(*padded_img));

  if (!padded_img) {
    fprintf(stderr, "Failed to allocate memory for padded image\n");
    return;
  }

  memcpy(wip_img, padded_img, img_size);
  free(padded_img);
}

void apply_sobel(Image* img) {
  int width = img->width;
  int height = img->height;
  int channels = img->channels;

  unsigned char* filtered_img = malloc(width * height * channels);

  if (!filtered_img) {
    fprintf(stderr, "Failed to allocate memory for filtered image\n");
    return;
  }

  memcpy(img->data, filtered_img, width * height * channels);
  free(filtered_img);
}
