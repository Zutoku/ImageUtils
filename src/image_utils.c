#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include "image.h"
#include "image_utils.h"

void image_utils(const char *operation, FILE *src_file, FILE *dest_file) {
  int width, height, channels, file_valid = 0;
  file_valid = stbi_info_from_file(src_file, &width, &height, &channels);

  unsigned char *data = NULL;
  if (file_valid && strcmp(operation, "greyscale") == 0) {
    data = stbi_load_from_file(src_file, &width, &height, &channels, 1);
  } else if (file_valid) {
    data = stbi_load_from_file(src_file, &width, &height, &channels, 0);
  } else {
    fprintf(stderr, "Failed to read image info: %s\n: ", stbi_failure_reason());
  }
  // Read RGB of specific pixel for testing:
  // printf("Width=%d, Height=%d, Channels=%d\n", width, height, channels);
  //
  // // Read pixel at coordinate x, y:
  // int x = 3000;
  // int y = 2000;
  // int index = (y * width + x) * channels;
  //
  // printf("Pixel (%d,%d): R=%d G=%d B=%d", x, y, data[index], data[index + 1],
  //        data[index + 2]);

  Image img = {0};
  img.width = width;
  img.height = height;
  img.channels = channels;

  stbi_write_png_to_func();
  stbi_image_free(data);
}
