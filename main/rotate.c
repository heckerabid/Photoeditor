#include <stdlib.h>
#include "rotate.h"

void rotate(BMPImage *image, int angle, int clockwise)
{
    if (angle != 90 && angle != 180 && angle != 270)
        return;

    int width = image->width;
    int height = image->height;

    int newWidth;
    int newHeight;

    if (angle == 90 || angle == 270) {
        newWidth = height;
        newHeight = width;
    }
    else {
        newWidth = width;
        newHeight = height;
    }

    unsigned char *rotated;

    rotated = malloc(newWidth * newHeight * 3);

    if (rotated == NULL)
        return;

    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {

            int newX;
            int newY;

            if (clockwise) {

                if (angle == 90) {
                    newX = height - 1 - y;
                    newY = x;
                }
                else if (angle == 180) {
                    newX = width - 1 - x;
                    newY = height - 1 - y;
                }
                else {
                    newX = y;
                    newY = width - 1 - x;
                }

            }
            else {

                if (angle == 90) {
                    newX = y;
                    newY = width - 1 - x;
                }
                else if (angle == 180) {
                    newX = width - 1 - x;
                    newY = height - 1 - y;
                }
                else {
                    newX = height - 1 - y;
                    newY = x;
                }
            }

            int source =
                (y * width + x) * 3;

            int destination =
                (newY * newWidth + newX) * 3;

            rotated[destination] =
                image->pixels[source];

            rotated[destination + 1] =
                image->pixels[source + 1];

            rotated[destination + 2] =
                image->pixels[source + 2];
        }
    }

    free(image->pixels);

    image->pixels = rotated;
    image->width = newWidth;
    image->height = newHeight;

    image->infoheader.width = newWidth;
    image->infoheader.height = newHeight;
}