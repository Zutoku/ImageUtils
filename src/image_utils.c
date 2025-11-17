#include <stdio.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include "image.h"
#include "image_utils.h"
void apply_orientation_transform(void *data, int *width, int *height,
                                 int channels, int orientation);

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
void rotate_image(unsigned char *data, int *w, int *h, int channels) {
  int width = *w;
  int height = *h;

  int new_width = height;
  int new_height = width;

  unsigned char *rotated = malloc(width * height * channels);

  for (int old_y = 0; old_y < height; old_y++) {
    for (int old_x = 0; old_x < width; old_x++) {

      int new_x = height - 1 - old_y; // rotated X
      int new_y = old_x;              // rotated Y

      unsigned char *src_px = data + (old_y * width + old_x) * channels;

      unsigned char *dst_px = rotated + (new_y * new_width + new_x) * channels;

      memcpy(dst_px, src_px, channels);
    }
  }

  memcpy(data, rotated, width * height * channels);

  free(rotated);

  *w = new_width;
  *h = new_height;
}
//
// void rotate_image(unsigned char *data, int *w, int *h, int channels) {
//   int new_width = *w;
//   int width = *w;
//   int height = *h;
//
//   int image_length = width * height * channels;
//   unsigned char *rotated_image = malloc(image_length);
//   unsigned char *src = data;
//
//   int column = 1;
//   int row = 1;
//   int new_pos;
//   int horizontal_source_pixels;
// /*
//                    1 2 3
// 0..width           a b c
// width+1 .. 2*width d e f
//
//
//             1 2
// 1*new_width d a
// 2*new_width e b
// 3*new_width f c
//
// 2,2 becomes 1,2
// 3,1 becomes 1,3
//
// old x becomes new y
// abc;def => da;eb;fc
// new x = new_width
//
// row 1 becomes last column of new width
// */
//
// new_column = new_width - old_row + 1
// new_row = old_column
// coordinates:
//   x = 0 .. width
//   y = row*width
//
// e.g. (3, 5)
//   x = 3
//   y = 5 * width
//
//     asdjfk;asjdflk;asdjflk;asdjfkl;1345;
//
//   for (; row <= height; row++) {
//     horizontal_source_pixels = row * width * channels - 1;
//     new_pos = column * new_width - channels * row;
//     column = 1;
//     for (; column <= width; column++) {
//       memcpy(&rotated_image[new_pos], &src[column - 1], channels);
//     }
//   }
//   int temp = *w;
//   *w = *h;
//   *h = temp;
//
//   memcpy(data, rotated_image, image_length);
//   free(rotated_image);
// }

void image_utils(const char *operation, FILE *src_file, FILE *dest_file) {
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
    } else {
      requested_channels = 0; // keep original
      data = stbi_load_from_file(src_file, &width, &height, &channels,
                                 requested_channels);
    }
    if (strcmp(operation, "rotate-clockwise") == 0) {
      rotate_image(data, &width, &height, channels);
    }
  } else {
    fprintf(stderr, "Failed to read image information.");
    return;
  }
  if (!data) {
    fprintf(stderr, "Failed to load data from source file: %s\n",
            stbi_failure_reason());
    return;
  }

  // Image img = {0};
  // img.width = width;
  // img.height = height;
  // img.channels = channels;

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
