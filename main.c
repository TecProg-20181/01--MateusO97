#include <stdio.h>

typedef struct _pixel {
  unsigned short int red;
  unsigned short int green;
  unsigned short int blue;
} Pixel;

typedef struct _image {
  // [widthidtheight][heighteigheightt][rgb]
  // 0 -> r
  // 1 -> g
  // 2 -> b
  unsigned short int pixel[512][512][3];
  unsigned int width;
  unsigned int height;
} Image;


int max(int a, int b) {
  if (a > b)
  return a;
  return b;
}

int pixel_igual(Pixel p1, Pixel p2) {
  if (p1.red == p2.red &&
    p1.green == p2.green &&
    p1.blue == p2.blue)
    return 1;
    return 0;
  }


  Image scales_of_gray(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
        int average = img.pixel[i][j][0] +
        img.pixel[i][j][1] +
        img.pixel[i][j][2];
        average /= 3;
        img.pixel[i][j][0] = average;
        img.pixel[i][j][1] = average;
        img.pixel[i][j][2] = average;
      }
    }

    return img;
  }

  // int average()

  void blur(unsigned int height, unsigned short int pixel[512][512][3], int T, unsigned int width) {
    for (unsigned int i = 0; i < height; ++i) {
      for (unsigned int j = 0; j < width; ++j) {
        Pixel average = {0, 0, 0};

        int min_height = (height - 1 > i + T/2) ? i + T/2 : height - 1;
        int min_width = (width - 1 > j + T/2) ? j + T/2 : width - 1;
        for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= min_height; ++x) {
          for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_width; ++y) {
            average.red += pixel[x][y][0];
            average.green += pixel[x][y][1];
            average.blue += pixel[x][y][2];
          }
        }

        average.red /= T * T;
        average.green /= T * T;
        average.blue /= T * T;

        pixel[i][j][0] = average.red;
        pixel[i][j][1] = average.green;
        pixel[i][j][2] = average.blue;
      }
    }
  }

  Image rotate_rigth(Image img) {
    Image rotated;

    rotated.width = img.height;
    rotated.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
      for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {
        rotated.pixel[i][j][0] = img.pixel[x][y][0];
        rotated.pixel[i][j][1] = img.pixel[x][y][1];
        rotated.pixel[i][j][2] = img.pixel[x][y][2];
      }
    }

    return rotated;
  }

  void revert_colors(unsigned short int pixel[512][512][3],
    unsigned int width, unsigned int height) {
      for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
          pixel[i][j][0] = 255 - pixel[i][j][0];
          pixel[i][j][1] = 255 - pixel[i][j][1];
          pixel[i][j][2] = 255 - pixel[i][j][2];
        }
      }
    }

    Image cut_image(Image img, int x, int y, int width, int height) {
      Image cortada;

      cortada.width = width;
      cortada.height = height;

      for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
          cortada.pixel[i][j][0] = img.pixel[i + y][j + x][0];
          cortada.pixel[i][j][1] = img.pixel[i + y][j + x][1];
          cortada.pixel[i][j][2] = img.pixel[i + y][j + x][2];
        }
      }

      return cortada;
    }

    Image sepia (Image img) {
      Image sepia;

      for (unsigned int x = 0; x < sepia.height; ++x) {
        for (unsigned int j = 0; j < sepia.width; ++j) {
          unsigned short int pixel[3];
          pixel[0] = sepia.pixel[x][j][0];
          pixel[1] = sepia.pixel[x][j][1];
          pixel[2] = sepia.pixel[x][j][2];

          int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
          int min_r = (255 >  p) ? p : 255;
          sepia.pixel[x][j][0] = min_r;

          p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
          min_r = (255 >  p) ? p : 255;
          sepia.pixel[x][j][1] = min_r;

          p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
          min_r = (255 >  p) ? p : 255;
          sepia.pixel[x][j][2] = min_r;
        }
      }
      return sepia;
    }

  void print_images(Image img){
    // print type of image
    printf("P3\n");
    // print widthidtheight heighteigheightt and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
        printf("%hu %hu %hu ", img.pixel[i][j][0],
        img.pixel[i][j][1],
        img.pixel[i][j][2]);

      }
      printf("\n");
    }
  }

    int main() {
      Image img;

      // read type of image
      char p3[4];
      scanf("%s", p3);

      // read widthidtheight heighteigheightt and color of image
      int max_color;
      scanf("%u %u %d", &img.width, &img.height, &max_color);

      // read all pixels of image
      for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
          scanf("%hu %hu %hu", &img.pixel[i][j][0],
          &img.pixel[i][j][1],
          &img.pixel[i][j][2]);

        }
      }

      int n_options;
      scanf("%d", &n_options);

      for(int i = 0; i < n_options; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
          case 1: { // Escala de Cinza
            img = scales_of_gray(img);
            break;
          }
          case 2: { // Filtro Sepia
            img = sepia(img);
            break;
          }
          case 3: { // Blur
            int size = 0;
            scanf("%d", &size);
            blur(img.height, img.pixel, size, img.width);
            break;
          }
          case 4: { // Rotacao
            int how_many = 0;
            scanf("%d", &how_many);
            how_many %= 4;
            for (int j = 0; j < how_many; ++j) {
              img = rotate_rigth(img);
            }
            break;
          }
          case 5: { // Espelheightamento
            int horizontal = 0;
            scanf("%d", &horizontal);

            int width = img.width, height = img.height;

            if (horizontal == 1) width /= 2;
            else height /= 2;

            for (int i2 = 0; i2 < height; ++i2) {
              for (int j = 0; j < width; ++j) {
                int x = i2, y = j;

                if (horizontal == 1) y = img.width - 1 - j;
                else x = img.height - 1 - i2;

                Pixel aux1;
                aux1.red = img.pixel[i2][j][0];
                aux1.green = img.pixel[i2][j][1];
                aux1.blue = img.pixel[i2][j][2];

                img.pixel[i2][j][0] = img.pixel[x][y][0];
                img.pixel[i2][j][1] = img.pixel[x][y][1];
                img.pixel[i2][j][2] = img.pixel[x][y][2];

                img.pixel[x][y][0] = aux1.red;
                img.pixel[x][y][1] = aux1.green;
                img.pixel[x][y][2] = aux1.blue;
              }
            }
            break;
          }
          case 6: { // Inversao de Cores
            revert_colors(img.pixel, img.width, img.height);
            break;
          }
          case 7: { // Cortar Imagem
            int x, y;
            scanf("%d %d", &x, &y);
            int width, height;
            scanf("%d %d", &width, &height);

            img = cut_image(img, x, y, width, height);
            break;
          }
        }
        print_images(img);
      }

      return 0;
    }
