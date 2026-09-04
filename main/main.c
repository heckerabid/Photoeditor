#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iup.h>

#include "openbmp.h"
#include "grayscale.h"
#include "brightness.h"
#include "invert.h"
#include "flip.h"
#include "rotate.h"
#include "crop.h"
#include "blur.h"
#include "sharpen.h"


BMPImage *image = NULL;
Ihandle *imageBox = NULL;



void updateImageDisplay(void);




/* =========================
   UNDO (single step only)
   ========================= */

unsigned char *previousPixels = NULL;
int previousWidth = 0;
int previousHeight = 0;
int hasUndo = 0;


void clearUndo(void)
{
    if (previousPixels != NULL) {
        free(previousPixels);
        previousPixels = NULL;
    }

    hasUndo = 0;
}


void saveUndo(void)
{
    if (image == NULL)
        return;

    /*
       Throw away whatever was remembered
       before, and remember the CURRENT
       image instead. Only ever one
       previous version is kept.
    */

    if (previousPixels != NULL) {
        free(previousPixels);
        previousPixels = NULL;
    }

    int size = image->width * image->height * 3;

    previousPixels = malloc(size);

    if (previousPixels == NULL)
        return;

    memcpy(
        previousPixels,
        image->pixels,
        size
    );

    previousWidth = image->width;
    previousHeight = image->height;

    hasUndo = 1;
}


void undo(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage("Undo", "No image is open.");
        return;
    }

    if (!hasUndo) {
        IupMessage("Undo", "Nothing to undo.");
        return;
    }

    int size = previousWidth * previousHeight * 3;

    unsigned char *pixels = malloc(size);

    if (pixels == NULL) {
        IupMessage("Error", "Not enough memory for undo.");
        return;
    }

    memcpy(
        pixels,
        previousPixels,
        size
    );

    free(image->pixels);

    image->pixels = pixels;
    image->width = previousWidth;
    image->height = previousHeight;

    image->infoheader.width = previousWidth;
    image->infoheader.height = previousHeight;

    /*
       Once used, the remembered version
       is gone - undo can't be pressed
       again until a new change is made.
    */

    free(previousPixels);
    previousPixels = NULL;
    hasUndo = 0;

    updateImageDisplay();
}


/* =========================
   DISPLAY IMAGE
   ========================= */

void updateImageDisplay(void)
{
    if (image == NULL || imageBox == NULL)
        return;

    int width = image->width;
    int height = image->height;

    int size = width * height * 3;

    unsigned char *rgb = malloc(size);

    if (rgb == NULL)
        return;

    /*
       image->pixels is already stored as
       RGB in memory (openBMP converts it
       from the file's BGR order), so we
       can copy it directly.
    */

    for (int i = 0; i < width * height * 3; i++) {
        rgb[i] = image->pixels[i];
    }

    Ihandle *iupImage =
        IupImageRGB(width, height, rgb);

    if (iupImage != NULL) {

        IupSetAttributeHandle(
            imageBox,
            "IMAGE",
            iupImage
        );

        char rasterSize[64];

        sprintf(
            rasterSize,
            "%dx%d",
            width,
            height
        );

        IupSetAttribute(
            imageBox,
            "RASTERSIZE",
            rasterSize
        );

        IupRefresh(IupGetDialog(imageBox));

        IupUpdate(imageBox);
    }

    free(rgb);
}


/* =========================
   OPEN
   ========================= */

int open_cb(Ihandle *ih)
{
    (void)ih;

    Ihandle *dialog = IupFileDlg();

    IupSetAttribute(
        dialog,
        "DIALOGTYPE",
        "OPEN"
    );

    IupSetAttribute(
        dialog,
        "FILTER",
        "*.bmp"
    );

    IupSetAttribute(
        dialog,
        "FILTERINFO",
        "BMP Files"
    );

    IupSetAttribute(
        dialog,
        "TITLE",
        "Open BMP Image"
    );

    IupPopup(
        dialog,
        IUP_CENTER,
        IUP_CENTER
    );

    char *status =
        IupGetAttribute(dialog, "STATUS");

    if (status != NULL &&
        strcmp(status, "-1") != 0) {

        char *filename =
            IupGetAttribute(dialog, "VALUE");

        if (filename != NULL) {

            BMPImage *newImage =
                openBMP(filename);

            if (newImage != NULL) {

                if (image != NULL)
                    freeBMP(image);

                image = newImage;

                clearUndo();

                updateImageDisplay();
            }
            else {
                IupMessage(
                    "Error",
                    "Could not open BMP image."
                );
            }
        }
    }

    IupDestroy(dialog);

    return IUP_DEFAULT;
}


/* =========================
   GRAYSCALE
   ========================= */

int grayscale_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    grayscale(image);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* =========================
   BRIGHTNESS
   ========================= */

int brightness1_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    brightness(image, 30);

    updateImageDisplay();

    return IUP_DEFAULT;
}


int brightness2_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    brightness(image, 60);

    updateImageDisplay();

    return IUP_DEFAULT;
}


int brightness3_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    brightness(image, 90);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* =========================
   INVERT
   ========================= */

int invert_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    invert(image);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* =========================
   HORIZONTAL FLIP
   ========================= */

int horizontal_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    horizontalFlip(image);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* =========================
   VERTICAL FLIP
   ========================= */

int vertical_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    verticalFlip(image);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* =========================
   ROTATE
   ========================= */

int rotate_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    Ihandle *clockwise =
        IupToggle("Clockwise", NULL);

    Ihandle *counterClockwise =
        IupToggle("Counterclockwise", NULL);

    Ihandle *direction =
        IupRadio(
            IupVbox(
                clockwise,
                counterClockwise,
                NULL
            )
        );

    IupSetAttribute(
        clockwise,
        "VALUE",
        "ON"
    );


    Ihandle *angle90 =
        IupToggle("90 Degrees", NULL);

    Ihandle *angle180 =
        IupToggle("180 Degrees", NULL);

    Ihandle *angle270 =
        IupToggle("270 Degrees", NULL);

    Ihandle *angle =
        IupRadio(
            IupVbox(
                angle90,
                angle180,
                angle270,
                NULL
            )
        );

    IupSetAttribute(
        angle90,
        "VALUE",
        "ON"
    );


    Ihandle *ok =
        IupButton("OK", NULL);

    Ihandle *cancel =
        IupButton("Cancel", NULL);


    Ihandle *box =
        IupVbox(

            IupLabel("Direction"),
            direction,

            IupLabel("Angle"),
            angle,

            IupHbox(
                ok,
                cancel,
                NULL
            ),

            NULL
        );


    Ihandle *dialog =
        IupDialog(box);

    IupSetAttribute(
        dialog,
        "TITLE",
        "Rotate Image"
    );

    IupSetAttribute(
        dialog,
        "RASTERSIZE",
        "250x220"
    );


    IupSetAttribute(
        ok,
        "PADDING",
        "10x5"
    );

    IupSetAttribute(
        cancel,
        "PADDING",
        "10x5"
    );


    /*
       Store the result using dialog status.
    */

    IupSetAttribute(
        dialog,
        "STATUS",
        "0"
    );

    IupSetCallback(
        ok,
        "ACTION",
        (Icallback)IupExitLoop
    );

    IupSetCallback(
        cancel,
        "ACTION",
        (Icallback)IupExitLoop
    );


    IupPopup(
        dialog,
        IUP_CENTER,
        IUP_CENTER
    );


    int selectedDirection = 1;
    int selectedAngle = 90;

    char *clockwiseValue =
        IupGetAttribute(
            clockwise,
            "VALUE"
        );

    char *counterValue =
        IupGetAttribute(
            counterClockwise,
            "VALUE"
        );

    if (counterValue != NULL &&
        strcmp(counterValue, "ON") == 0) {

        selectedDirection = 0;
    }


    char *angle180Value =
        IupGetAttribute(
            angle180,
            "VALUE"
        );

    char *angle270Value =
        IupGetAttribute(
            angle270,
            "VALUE"
        );

    if (angle180Value != NULL &&
        strcmp(angle180Value, "ON") == 0) {

        selectedAngle = 180;
    }

    if (angle270Value != NULL &&
        strcmp(angle270Value, "ON") == 0) {

        selectedAngle = 270;
    }


    (void)clockwiseValue;

    IupDestroy(dialog);


    saveUndo();

    rotate(
        image,
        selectedAngle,
        selectedDirection
    );

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* =========================
   CROP
   ========================= */

int crop_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }


    Ihandle *xText =
        IupText(NULL);

    Ihandle *yText =
        IupText(NULL);

    Ihandle *widthText =
        IupText(NULL);

    Ihandle *heightText =
        IupText(NULL);


    IupSetAttribute(
        xText,
        "VALUE",
        "0"
    );

    IupSetAttribute(
        yText,
        "VALUE",
        "0"
    );


    char value[32];


    sprintf(
        value,
        "%d",
        image->width / 2
    );

    IupSetAttribute(
        widthText,
        "VALUE",
        value
    );


    sprintf(
        value,
        "%d",
        image->height / 2
    );

    IupSetAttribute(
        heightText,
        "VALUE",
        value
    );


    IupSetAttribute(
        xText,
        "RASTERSIZE",
        "100x25"
    );

    IupSetAttribute(
        yText,
        "RASTERSIZE",
        "100x25"
    );

    IupSetAttribute(
        widthText,
        "RASTERSIZE",
        "100x25"
    );

    IupSetAttribute(
        heightText,
        "RASTERSIZE",
        "100x25"
    );


    Ihandle *ok =
        IupButton("Crop", NULL);

    Ihandle *cancel =
        IupButton("Cancel", NULL);


    Ihandle *box =
        IupVbox(

            IupHbox(
                IupLabel("Start X:"),
                xText,
                NULL
            ),

            IupHbox(
                IupLabel("Start Y:"),
                yText,
                NULL
            ),

            IupHbox(
                IupLabel("Width:"),
                widthText,
                NULL
            ),

            IupHbox(
                IupLabel("Height:"),
                heightText,
                NULL
            ),

            IupHbox(
                ok,
                cancel,
                NULL
            ),

            NULL
        );


    Ihandle *dialog =
        IupDialog(box);


    IupSetAttribute(
        dialog,
        "TITLE",
        "Crop Image"
    );

    IupSetAttribute(
        dialog,
        "RASTERSIZE",
        "300x220"
    );


    IupSetCallback(
        ok,
        "ACTION",
        (Icallback)IupExitLoop
    );

    IupSetCallback(
        cancel,
        "ACTION",
        (Icallback)IupExitLoop
    );


    IupPopup(
        dialog,
        IUP_CENTER,
        IUP_CENTER
    );


    char *xValue =
        IupGetAttribute(
            xText,
            "VALUE"
        );

    char *yValue =
        IupGetAttribute(
            yText,
            "VALUE"
        );

    char *wValue =
        IupGetAttribute(
            widthText,
            "VALUE"
        );

    char *hValue =
        IupGetAttribute(
            heightText,
            "VALUE"
        );


    int startX = atoi(xValue);
    int startY = atoi(yValue);

    int cropWidth =
        atoi(wValue);

    int cropHeight =
        atoi(hValue);


    IupDestroy(dialog);


    if (startX < 0 ||
        startY < 0 ||
        cropWidth <= 0 ||
        cropHeight <= 0 ||
        startX + cropWidth > image->width ||
        startY + cropHeight > image->height) {

        IupMessage(
            "Error",
            "Invalid crop area."
        );

        return IUP_DEFAULT;
    }


    saveUndo();


    crop(
        image,
        startX,
        startY,
        cropWidth,
        cropHeight
    );


    updateImageDisplay();


    return IUP_DEFAULT;
}


/* =========================
   BLUR
   ========================= */

int blur_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    blur(image);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* =========================
   SHARPEN
   ========================= */

int sharpen_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    sharpen(image);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* =========================
   EXIT
   ========================= */

int exit_cb(Ihandle *ih)
{
    (void)ih;

    if (image != NULL) {
        freeBMP(image);
        image = NULL;
    }

    clearUndo();

    return IUP_CLOSE;
}


/* =========================
   MAIN
   ========================= */

int main(int argc, char **argv)
{
    IupOpen(&argc, &argv);


    /* =========================
       BUTTONS
       ========================= */

    Ihandle *openButton =
        IupButton("Open BMP", NULL);

    Ihandle *undoButton =
        IupButton("Undo", NULL);

    Ihandle *grayscaleButton =
        IupButton("Grayscale", NULL);

    Ihandle *brightness1Button =
        IupButton("Brightness 1", NULL);

    Ihandle *brightness2Button =
        IupButton("Brightness 2", NULL);

    Ihandle *brightness3Button =
        IupButton("Brightness 3", NULL);

    Ihandle *invertButton =
        IupButton("Invert", NULL);

    Ihandle *horizontalButton =
        IupButton("Horizontal Flip", NULL);

    Ihandle *verticalButton =
        IupButton("Vertical Flip", NULL);

    Ihandle *rotateButton =
        IupButton("Rotate", NULL);

    Ihandle *cropButton =
        IupButton("Crop", NULL);

    Ihandle *blurButton =
        IupButton("Blur", NULL);

    Ihandle *sharpenButton =
        IupButton("Sharpen", NULL);

    Ihandle *exitButton =
        IupButton("Exit", NULL);


    /* =========================
       CALLBACKS
       ========================= */

    IupSetCallback(
        openButton,
        "ACTION",
        (Icallback)open_cb
    );

    IupSetCallback(
        undoButton,
        "ACTION",
        (Icallback)undo
    );

    IupSetCallback(
        grayscaleButton,
        "ACTION",
        (Icallback)grayscale_cb
    );

    IupSetCallback(
        brightness1Button,
        "ACTION",
        (Icallback)brightness1_cb
    );

    IupSetCallback(
        brightness2Button,
        "ACTION",
        (Icallback)brightness2_cb
    );

    IupSetCallback(
        brightness3Button,
        "ACTION",
        (Icallback)brightness3_cb
    );

    IupSetCallback(
        invertButton,
        "ACTION",
        (Icallback)invert_cb
    );

    IupSetCallback(
        horizontalButton,
        "ACTION",
        (Icallback)horizontal_cb
    );

    IupSetCallback(
        verticalButton,
        "ACTION",
        (Icallback)vertical_cb
    );

    IupSetCallback(
        rotateButton,
        "ACTION",
        (Icallback)rotate_cb
    );

    IupSetCallback(
        cropButton,
        "ACTION",
        (Icallback)crop_cb
    );

    IupSetCallback(
        blurButton,
        "ACTION",
        (Icallback)blur_cb
    );

    IupSetCallback(
        sharpenButton,
        "ACTION",
        (Icallback)sharpen_cb
    );

    IupSetCallback(
        exitButton,
        "ACTION",
        (Icallback)exit_cb
    );


    /* =========================
       IMAGE DISPLAY
       ========================= */

    imageBox =
        IupLabel(NULL);

    IupSetAttribute(
        imageBox,
        "ALIGNMENT",
        "ACENTER"
    );

    IupSetAttribute(
        imageBox,
        "EXPAND",
        "YES"
    );

    /*
       Give the label a placeholder image
       BEFORE the dialog is shown, so IUP
       creates it as an image-type label
       from the start (Windows driver
       decides text-vs-image at map time).
    */

    unsigned char placeholderPixel[3] =
        { 240, 240, 240 };

    Ihandle *placeholderImage =
        IupImageRGB(1, 1, placeholderPixel);

    IupSetAttributeHandle(
        imageBox,
        "IMAGE",
        placeholderImage
    );


    /* =========================
       BUTTON ROWS
       ========================= */

    Ihandle *row1 =
        IupHbox(
            openButton,
            undoButton,
            exitButton,
            NULL
        );


    Ihandle *row2 =
        IupHbox(
            grayscaleButton,
            brightness1Button,
            brightness2Button,
            brightness3Button,
            invertButton,
            NULL
        );


    Ihandle *row3 =
        IupHbox(
            horizontalButton,
            verticalButton,
            rotateButton,
            cropButton,
            NULL
        );


    Ihandle *row4 =
        IupHbox(
            blurButton,
            sharpenButton,
            NULL
        );


    Ihandle *controls =
        IupVbox(
            row1,
            row2,
            row3,
            row4,
            NULL
        );


    IupSetAttribute(
        controls,
        "GAP",
        "5"
    );


    /* =========================
       MAIN LAYOUT
       ========================= */

    Ihandle *mainBox =
        IupVbox(
            controls,
            imageBox,
            NULL
        );


    IupSetAttribute(
        mainBox,
        "MARGIN",
        "10x10"
    );

    IupSetAttribute(
        mainBox,
        "GAP",
        "10"
    );


    Ihandle *dialog =
        IupDialog(mainBox);


    IupSetAttribute(
        dialog,
        "TITLE",
        "BMP Image Manipulation Software"
    );


    IupSetAttribute(
        dialog,
        "RASTERSIZE",
        "1000x700"
    );


    IupShowXY(
        dialog,
        IUP_CENTER,
        IUP_CENTER
    );


    IupMainLoop();


    if (image != NULL) {
        freeBMP(image);
        image = NULL;
    }

    clearUndo();


    IupClose();


    return 0;
}
