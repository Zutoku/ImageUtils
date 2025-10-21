#include <stdio.h>
#include <string.h>

void print_help(void);
void print_help(void) {
  printf(
      "Usage: PLACEHOLDERNAME <operation> <source-file> optional:<destination-file>\n \
      Valid operations:\n \
      - grayscale\n \
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

  return 0;
}
