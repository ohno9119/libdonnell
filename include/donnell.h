#include <stdint.h>

/* Public API for libdonnell */

#ifndef DONNELL
#define DONNELL

typedef struct {
    unsigned int w;
    unsigned int h;
} DonnellSize;

typedef struct {
    unsigned int w;
    unsigned int h;
    unsigned int x;
    unsigned int y;
} DonnellRect;

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} DonnellPixel;

typedef struct {
    DonnellPixel ***pixels;
    unsigned int width;
    unsigned int height;
} DonnellImageBuffer;

typedef enum
{
    DONNELL_FONT_SERIF,
    DONNELL_FONT_SANS_SERIF,
    DONNELL_FONT_MONOSPACE,
} DonnellFont;

void Donnell_Init(void);
void Donnell_Cleanup(void);

DonnellPixel *Donnell_Pixel_Create();
DonnellPixel *Donnell_Pixel_CreateEasy(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

DonnellImageBuffer *Donnell_ImageBuffer_Create(unsigned int width, unsigned int height);
void Donnell_ImageBuffer_SetPixel(DonnellImageBuffer *buffer, unsigned int x, unsigned int y, DonnellPixel *pixel);
void Donnell_ImageBuffer_BlendPixel(DonnellImageBuffer *buffer, unsigned int x, unsigned int y, DonnellPixel *pixel);
void Donnell_ImageBuffer_Clear(DonnellImageBuffer *buffer, DonnellPixel *pixel);
void Donnell_ImageBuffer_DumpAsBitmap(DonnellImageBuffer *buffer, char *name);
void Donnell_ImageBuffer_Free(DonnellImageBuffer *buffer);

void Donnell_GraphicsPrimitives_DrawTextLine(DonnellImageBuffer *buffer, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font);
void Donnell_GraphicsPrimitives_MeasureTextLine(DonnellSize *size, char *utf8string, unsigned int pixel_size, DonnellFont req_font);
void Donnell_GraphicsPrimitives_DrawText(DonnellImageBuffer *buffer, DonnellPixel *color, char *utf8string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font);
void Donnell_GraphicsPrimitives_MeasureText(DonnellSize *size, char *utf8string, unsigned int pixel_size, DonnellFont req_font);

#endif
