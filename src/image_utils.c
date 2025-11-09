#include <stdio.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include "image.h"
#include "image_utils.h"

void write_func(void *context, void *data, int size) {
  // Context here could be a pointer to anything, here we cast it to FILE*
  FILE *file = (FILE *)context;
  // We write one byte at a time, since the data is already encoded and we don't
  // care about chunks or size of elements, just that everything is written
  int written = fwrite(data, 1, size, file);
  if (written != size) {
    fprintf(stderr, "Failed to write processed image data.");
    return;
  }
}

void image_utils(const char *operation, FILE *src_file, FILE *dest_file) {
  int width, height, channels, file_valid = 0;
  file_valid = stbi_info_from_file(src_file, &width, &height, &channels);
  fseek(src_file, 0, SEEK_SET); // Rewind file pointer to beginning of file

  unsigned char *data = NULL;
  int requested_channels = 0; // 0 preserves original channels
  if (file_valid && strcmp(operation, "greyscale") == 0) {
    requested_channels = 1; // force single channel for greyscale
    data = stbi_load_from_file(src_file, &width, &height, &channels,
                               requested_channels);
  } else if (file_valid) {
    requested_channels = 0; // keep original
    data = stbi_load_from_file(src_file, &width, &height, &channels,
                               requested_channels);
  } else {
    fprintf(stderr, "Failed to read image information.");
    return;
  }
  if (!data) {
    fprintf(stderr, "Failed to load data from source file: %s\n",
            stbi_failure_reason());
    return;
  }

  Image img = {0};
  img.width = width;
  img.height = height;
  img.channels = channels;

  int output_channels =
      (requested_channels == 0) ? channels : requested_channels;
  int ok =
      stbi_write_png_to_func(write_func, dest_file, width, height,
                             output_channels, data, width * output_channels);
  if (!ok) {
    fprintf(stderr, "Failed to write PNG\n");
  }

  stbi_image_free(data);
}
