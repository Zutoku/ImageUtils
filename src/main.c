#include "image.h"
#include "image_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void print_help(void);
void print_help(void) {
  printf(
      "Usage: image-utils <operation> <source-file> optional:<destination-file>\n \
      Valid operations:\n \
      - greyscale\n \
      - horzizontal-mirror\n \
      - vertical-mirror\n \
      - canny-edge-detector\n");
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    print_help();
    return 0;
  }

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_help();
      return 0;
    }
  }

  if (argc < 3) {
    fprintf(stderr, "Error: missing arguments.\n");
    print_help();
    return 1;
  }
  const char *operation = argv[1];
  const char *source_filename = argv[2];
  bool overwrite_source = false;

  if (argc == 3) {
    printf("This operation will overwrite the source-file. Proceed? (y/n): ");
    bool input_invalid = true;
    char input[8];
    while (input_invalid) {
      // Checks for valid input
      if (!fgets(input, sizeof(input), stdin)) {
        break;
      }
      if (input[0] == 'y' || input[0] == 'Y') {
        overwrite_source = true;
        input_invalid = false;
      } else if (input[0] == 'n' || input[0] == 'N') {
        return 0;
      } else {
        input_invalid = true;
      }
      if (input_invalid) {
        printf("Invalid input. Try again (y/n): ");
      }
    }
  }

  FILE *destination_image = NULL;
  FILE *source_image = NULL;
  const char *destination_filename = NULL;

  if (overwrite_source) {
    source_image = fopen(source_filename, "rb");
    if (!source_image) {
      fprintf(stderr, "Error: could not open '%s' in read mode.\n",
              source_filename);
      return 1;
    }

    destination_image = fopen(source_filename, "wb");
    if (!destination_image) {
      fprintf(stderr, "Error: could not open '%s' in write mode.\n",
              source_filename);
      fclose(source_image);
      return 1;
    }

    image_utils(operation, source_image, destination_image);

    fclose(source_image);
    fclose(destination_image);
  } else {
    if (argc < 4) {
      fprintf(stderr, "Error: missing destination filename.\n");
      return 1;
    }

    destination_filename = argv[3];
    destination_image = fopen(destination_filename, "wb");
    if (!destination_image) {
      fprintf(stderr, "Error: could not open or create '%s' write mode.\n",
              destination_filename);
      return 1;
    }

    source_image = fopen(source_filename, "rb");
    if (!source_image) {
      fprintf(stderr, "Error: could not open source file '%s' in read mode.\n",
              source_filename);
      fclose(destination_image);
      return 1;
    }

    image_utils(operation, source_image, destination_image);

    if (source_image) {
      fclose(source_image);
    }
    if (destination_image) {
      fclose(destination_image);
    }
  }

  return 0;
}
