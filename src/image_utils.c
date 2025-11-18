#include <stdio.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include "image.h"
#include "image_utils.h"
#include "rotate.h"

Image load_image(const char *operation, FILE *src_file) {
  Image img = {0};
  int width, height, channels, file_valid = 0;
  file_valid = stbi_info_from_file(src_file, &width, &height, &channels);
  fseek(src_file, 0, SEEK_SET); // Rewind file pointer to beginning of file

  unsigned char *data = NULL;
  int requested_channels = 0; // 0 preserves original channels
  if (file_valid) {
    if (strcmp(operation, "greyscale") == 0) {
      requested_channels = 1; // force single channel for greyscale
      data = stbi_load_from_file(src_file, &width, &height, &channels,
                                 requested_channels);
      img.data = data;
      img.width = width;
      img.height = height;
      img.channels = requested_channels;
    } else {
      requested_channels = 0; // keep original
      data = stbi_load_from_file(src_file, &width, &height, &channels,
                                 requested_channels);
      img.data = data;
      img.width = width;
      img.height = height;
      img.channels = channels;
    }
  } else {
    fprintf(stderr, "Failed to read image information.");
    return img;
  }
  if (!data) {
    fprintf(stderr, "Failed to load data from source file: %s\n",
            stbi_failure_reason());
    return img;
  }
  return img;
}

void apply_operation(const char *operation, Image *img) {
  if (strcmp(operation, "rotate-clockwise") == 0) {
    rotate_image_clockwise(img);
  } else {
    return;
  }
}
void write_image(Image img, const char *dest_path) {

  int ok = stbi_write_png(dest_path, img.width, img.height, img.channels,
                          img.data, img.width * img.channels);
  if (!ok) {
    fprintf(stderr, "Failed to write PNG\n");
  }

  stbi_image_free(img.data);
}

void process_image(const char *operation, FILE *src_file,
                   const char *dest_path) {
  Image img = {0};
  img = load_image(operation, src_file);
  apply_operation(operation, &img);
  write_image(img, dest_path);
}
