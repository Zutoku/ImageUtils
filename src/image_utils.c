#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "image.h"
#include "image_utils.h"
#include "stb_image.h"
#include "stb_image_write.h"

// void apply_orientation_transform(void *data, int *width, int *height,
//                                  int channels, int orientation);

void rotate_image_clockwise(unsigned char *data, int *w, int *h, int channels) {
  int width = *w;
  int height = *h;

  int new_width = height;
  int new_height = width;

  unsigned char *rotated = malloc(width * height * channels);
  if (!rotated) {
    fprintf(stderr,
            "Attempted to allocate memory for 'rotated' but resulted in NULL");
    return;
  }

  for (int old_y = 0; old_y < height; old_y++) {
    for (int old_x = 0; old_x < width; old_x++) {

      int new_x = new_width - 1 - old_y;
      int new_y = old_x;

      unsigned char *src_px = data + (old_y * width + old_x) * channels;

      unsigned char *dst_px = rotated + (new_y * new_width + new_x) * channels;

      memcpy(dst_px, src_px, channels);
    }
  }

  memcpy(data, rotated, width * height * channels);

  free(rotated);

  // overwrite original width and height values
  *w = new_width;
  *h = new_height;
}

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
      img.channels = channels;
      return img;
    } else {
      requested_channels = 0; // keep original
      data = stbi_load_from_file(src_file, &width, &height, &channels,
                                 requested_channels);
      img.data = data;
      img.width = width;
      img.height = height;
      img.channels = channels;
      return img;
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
}

void apply_operation(const char *operation, Image img) {
  if (strcmp(operation, "rotate-clockwise") == 0) {
    rotate_image_clockwise(img.data, img.width, img.height, img.channels);
  } else {
    return;
  }
}
void write_image(Image img, const char *dest_path) {

  int output_channels =
      (requested_channels == 0) ? img.channels : requested_channels;
  int ok = stbi_write_png(dest_path, img.width, img.height, output_channels,
                          img.data, img.width * output_channels);
  if (!ok) {
    fprintf(stderr, "Failed to write PNG\n");
  }

  stbi_image_free(img.data);
}

void process_image(const char *operation, FILE *src_file,
                   const char *dest_path) {
  Image img = {0};
  img = load_image(operation, src_file);
  apply_operation(operation, img);
  write_image(img, dest_path);
}

// void image_utils(const char *operation, FILE *src_file,
//                  const char *dest_path) {
//   int width, height, channels, file_valid = 0;
//   file_valid = stbi_info_from_file(src_file, &width, &height, &channels);
//   fseek(src_file, 0, SEEK_SET); // Rewind file pointer to beginning of file
//
//   unsigned char *data = NULL;
//   int requested_channels = 0; // 0 preserves original channels
//   if (file_valid) {
//     if (strcmp(operation, "greyscale") == 0) {
//       requested_channels = 1; // force single channel for greyscale
//       data = stbi_load_from_file(src_file, &width, &height, &channels,
//                                  requested_channels);
//     } else {
//       requested_channels = 0; // keep original
//       data = stbi_load_from_file(src_file, &width, &height, &channels,
//                                  requested_channels);
//     }
//     if (strcmp(operation, "rotate-clockwise") == 0) {
//       rotate_image_clockwise(data, &width, &height, channels);
//     }
//   } else {
//     fprintf(stderr, "Failed to read image information.");
//     return;
//   }
//   if (!data) {
//     fprintf(stderr, "Failed to load data from source file: %s\n",
//             stbi_failure_reason());
//     return;
//   }
//
//   // Image img = {0};
//   // img.width = width;
//   // img.height = height;
//   // img.channels = channels;
//
//   int output_channels =
//       (requested_channels == 0) ? channels : requested_channels;
//   int ok = stbi_write_png(dest_path, width, height, output_channels, data,
//                           width * output_channels);
//   if (!ok) {
//     fprintf(stderr, "Failed to write PNG\n");
//   }
//
//   stbi_image_free(data);
// }
