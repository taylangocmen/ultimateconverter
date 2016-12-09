#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <png.h>

void convert_jpg_to_png() {
  FILE* image = NULL;
  FILE* infile = fopen("test200by200.jpg", "r");
  assert(infile != NULL);

  unsigned char storage[10];
  int writing = 0;
  char name[50];
  char* extension = ".png";

  while (fread(&storage, sizeof (storage), 1, infile)) {

    if (storage == NULL) break;

    if (storage[0] == 0xff && storage[1] == 0xd8 
      && storage[2] == 0xff && (storage[3] == 0xe0 || storage[3] == 0xe1)) {
      
      if (image != NULL)  fclose(image);

      sprintf(name, "test200by200%s", extension);
      image = fopen(name, "w");
      writing = 1;

      if (writing == 1 && storage != NULL)
        fwrite(storage, sizeof (storage), 1, image);
    }

    if (writing == 1 && storage != NULL)
      fwrite(storage, sizeof (storage), 1, image);
    
    if (storage == NULL)  fclose(image);
  }

  fclose(image);
  fclose(infile);

  return;
}

int main(int argc, char* argv[]) {
  convert_jpg_to_png();
  return 0;
}




