#include <stdio.h>

int conv_xy_to_px_coord(int x, int y, int width, int height, int channels) {
  if (y > height - 1) {
    fprintf(stderr, "y-value is too high, the maximum height is: %i", height);
    return -1;
  } else if (x > width - 1) {
    fprintf(stderr, "x-value is too high, the maximum width is: %i", width);
    return -1;
  }

  return (x + y * width) * channels;

  //  012
  // 0xxx
  // 1xxx
  // 2xxx
}
