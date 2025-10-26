#ifndef IMAGE_H
#define IMAGE_H

typedef struct {
  unsigned char *data; // Pointer to pixel data
  int width;
  int height;
  int channels; // e.g., 3 for RGB, 4 for RGBA
  char type[8]; // File type like "png", "jpg", etc.
} Image;

#endif // IMAGE_H
