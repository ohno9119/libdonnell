#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "donnell.h"
#include "misc.h"
#include "symvis.h"

DONNELL_EXPORT DonnellImageBuffer *Donnell_ImageBuffer_Create(unsigned int width, unsigned int height) {
    DonnellImageBuffer *buffer;
    unsigned int i;
    unsigned int ci;

    buffer = malloc(sizeof(DonnellImageBuffer));
    if (!buffer) {
        return NULL;
    }

    buffer->width = width;
    buffer->height = height;

    buffer->pixels = calloc(height, sizeof(DonnellPixel **));
    if (!buffer->pixels) {
        return NULL;
    }

    for (i = 0; i < height; i++) {
        buffer->pixels[i] = calloc(width, sizeof(DonnellPixel *));
        if (!buffer->pixels[i]) {
            return NULL;
        }
        for (ci = 0; ci < width; ci++) {
            buffer->pixels[i][ci] == NULL;
        }
    }

    return buffer;
}

DONNELL_EXPORT void Donnell_ImageBuffer_Clear(DonnellImageBuffer *buffer, DonnellPixel *pixel) {
    unsigned int i;
    unsigned int ci;

    if ((!pixel) || (!buffer)) {
        return;
    }

    for (i = 0; i < buffer->height; i++) {
        for (ci = 0; ci < buffer->width; ci++) {
            Donnell_ImageBuffer_SetPixel(buffer, ci, i, pixel);
        }
    }
}

DONNELL_EXPORT void Donnell_ImageBuffer_Free(DonnellImageBuffer *buffer) {
    unsigned int i;
    unsigned int ci;

    if (!buffer) {
        return;
    }

    for (i = 0; i < buffer->height; i++) {
        for (ci = 0; ci < buffer->width; ci++) {
            if (buffer->pixels[i][ci]) {
                free(buffer->pixels[i][ci]);
            }
        }
        free(buffer->pixels[i]);
    }

    free(buffer->pixels);
    free(buffer);
}

DONNELL_EXPORT DonnellPixel *Donnell_ImageBuffer_GetPixel(DonnellImageBuffer *buffer, unsigned int x, unsigned int y) {
    if ((y < 0) || (x < 0) || (!buffer)) {
        return NULL;
    }

    if ((y >= buffer->height) || (x >= buffer->width)) {
        return Donnell_Pixel_CreateEasy(0, 0, 0, 255);
    }

    if (!buffer->pixels[y][x]) {
        return Donnell_Pixel_CreateEasy(0, 0, 0, 255);
    }

    return Misc_MemDup(buffer->pixels[y][x], sizeof(DonnellPixel));
}

DONNELL_EXPORT void Donnell_ImageBuffer_SetPixel(DonnellImageBuffer *buffer, unsigned int x, unsigned int y, DonnellPixel *pixel) {
    if ((y < 0) || (x < 0) || (!pixel) || (!buffer)) {
        return;
    }

    if ((y >= buffer->height) || (x >= buffer->width)) {
        return;
    }

    if (buffer->pixels[y][x]) {
        free(buffer->pixels[y][x]);
    }

    buffer->pixels[y][x] = Misc_MemDup(pixel, sizeof(DonnellPixel));
}

DONNELL_EXPORT void Donnell_ImageBuffer_BlendPixel(DonnellImageBuffer *buffer, unsigned int x, unsigned int y, DonnellPixel *pixel) {
    if ((y < 0) || (x < 0) || (!pixel) || (!buffer)) {
        return;
    }

    if ((y >= buffer->height) || (x >= buffer->width)) {
        return;
    }

    if (buffer->pixels[y][x]) {
        DonnellPixel *cpixel;

        cpixel = Donnell_Pixel_Blend(buffer->pixels[y][x], pixel);

        Donnell_ImageBuffer_SetPixel(buffer, x, y, cpixel);

        free(cpixel);
    } else {
        Donnell_ImageBuffer_SetPixel(buffer, x, y, pixel);
    }
}

DONNELL_EXPORT void Donnell_ImageBuffer_DumpAsBitmap(DonnellImageBuffer *buffer, char *name) {
    FILE *file;
    unsigned char *bitmap_data;
    unsigned int file_size;
    unsigned int i;
    unsigned int j;
    unsigned int x;
    unsigned int y;

    unsigned char file_header[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    unsigned char info_header[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
    unsigned char padding[3] = {0, 0, 0};

    if ((!buffer) || (!name)) {
        return;
    }

    file_size = 54 + 3 * buffer->width * buffer->height;

    bitmap_data = calloc(3, buffer->width * buffer->height);
    memset(bitmap_data, 0, 3 * buffer->width * buffer->height);

    for (i = 0; i < buffer->width; i++) {
        for (j = 0; j < buffer->height; j++) {
            x = i;
            y = (buffer->height - 1) - j;

            if (buffer->pixels[y][x]) {
                bitmap_data[(x + y * buffer->width) * 3 + 2] = buffer->pixels[y][x]->red;
                bitmap_data[(x + y * buffer->width) * 3 + 1] = buffer->pixels[y][x]->green;
                bitmap_data[(x + y * buffer->width) * 3 + 0] = buffer->pixels[y][x]->blue;
            } else {
                bitmap_data[(x + y * buffer->width) * 3 + 2] = 0;
                bitmap_data[(x + y * buffer->width) * 3 + 1] = 0;
                bitmap_data[(x + y * buffer->width) * 3 + 0] = 0;
            }
        }
    }

    file_header[2] = (unsigned char)(file_size);
    file_header[3] = (unsigned char)(file_size >> 8);
    file_header[4] = (unsigned char)(file_size >> 16);
    file_header[5] = (unsigned char)(file_size >> 24);

    info_header[4] = (unsigned char)(buffer->width);
    info_header[5] = (unsigned char)(buffer->width >> 8);
    info_header[6] = (unsigned char)(buffer->width >> 16);
    info_header[7] = (unsigned char)(buffer->width >> 24);
    info_header[8] = (unsigned char)(buffer->height);
    info_header[9] = (unsigned char)(buffer->height >> 8);
    info_header[10] = (unsigned char)(buffer->height >> 16);
    info_header[11] = (unsigned char)(buffer->height >> 24);

    file = fopen(name, "wb");
    fwrite(file_header, 1, 14, file);
    fwrite(info_header, 1, 40, file);

    for (i = 0; i < buffer->height; i++) {
        fwrite(bitmap_data + (buffer->width * (buffer->height - i - 1) * 3), 3, buffer->width, file);
        fwrite(padding, 1, (4 - (buffer->width * 3) % 4) % 4, file);
    }

    free(bitmap_data);
    fclose(file);
}

DonnellImageBuffer *ImageBuffer_ScaleNN(DonnellImageBuffer *buffer, unsigned int width, unsigned int height) {
    DonnellImageBuffer *ret;
    double scale_h;
    double scale_w;
    unsigned int closest_h;
    unsigned int closest_w;
    unsigned int i;
    unsigned int j;

    ret = Donnell_ImageBuffer_Create(width, height);
    scale_w = buffer->width / (double)width;
    scale_h = buffer->height / (double)height;

    for (i = 0; i < height; ++i) {
        closest_h = (int)(scale_h * i + 0.5);
        for (j = 0; j < width; ++j) {
            DonnellPixel *pixel;
            unsigned int ch;
            unsigned int cw;

            closest_w = (int)(scale_w * j + 0.5);
            if (closest_h >= buffer->height) {
                ch = buffer->height - 1;
            } else {
                ch = closest_h;
            }

            if (closest_w >= buffer->width) {
                cw = buffer->width - 1;
            } else {
                cw = closest_w;
            }

            pixel = Donnell_ImageBuffer_GetPixel(buffer, cw, ch);

            Donnell_ImageBuffer_SetPixel(ret, j, i, pixel);

            Donnell_Pixel_Free(pixel);
        }
    }

    return ret;
}

float ImageBuffer_ScaleBL_LinearOp(float a, float b, float c) {
    return a + (b - a) * c;
}

float ImageBuffer_ScaleBL_BiLinearOp(float a, float b, float c, float d, float x, float y) {
    return ImageBuffer_ScaleBL_LinearOp(ImageBuffer_ScaleBL_LinearOp(a, b, x), ImageBuffer_ScaleBL_LinearOp(c, d, x), y);
}

DonnellImageBuffer *ImageBuffer_ScaleBL(DonnellImageBuffer *buffer, unsigned int width, unsigned int height) {
    DonnellImageBuffer *ret;
    float scale_h;
    float scale_w;
    unsigned int i;
    unsigned int j;

    ret = Donnell_ImageBuffer_Create(width, height);
    scale_w = buffer->width / (float)width;
    scale_h = buffer->height / (float)height;

    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            float gx;
            float gy;
            int ix;
            int iy;
            DonnellPixel *a;
            DonnellPixel *b;
            DonnellPixel *c;
            DonnellPixel *d;
            DonnellPixel *e;

            gx = j / (float)(width) * (buffer->width - 1);
            gy = i / (float)(height) * (buffer->height - 1);

            ix = (int)gx;
            iy = (int)gy;

            e = Donnell_Pixel_Create();
            a = Donnell_ImageBuffer_GetPixel(buffer, ix, iy);
            b = Donnell_ImageBuffer_GetPixel(buffer, ix + 1, iy);
            c = Donnell_ImageBuffer_GetPixel(buffer, ix, iy + 1);
            d = Donnell_ImageBuffer_GetPixel(buffer, ix + 1, iy + 1);

            e->red = (DonnellUInt8)ImageBuffer_ScaleBL_BiLinearOp(a->red, b->red, c->red, d->red, (gx - ix), (gy - iy));
            e->green = (DonnellUInt8)ImageBuffer_ScaleBL_BiLinearOp(a->green, b->green, c->green, d->green, (gx - ix), (gy - iy));
            e->blue = (DonnellUInt8)ImageBuffer_ScaleBL_BiLinearOp(a->blue, b->blue, c->blue, d->blue, (gx - ix), (gy - iy));
            e->alpha = (DonnellUInt8)ImageBuffer_ScaleBL_BiLinearOp(a->alpha, b->alpha, c->alpha, d->alpha, (gx - ix), (gy - iy));

            Donnell_ImageBuffer_SetPixel(ret, j, i, e);

            Donnell_Pixel_Free(e);
        }
    }

    return ret;
}

DONNELL_EXPORT DonnellImageBuffer *Donnell_ImageBuffer_Scale(DonnellImageBuffer *buffer, unsigned int width, unsigned int height, DonnellScalingAlgorithm algo) {
    if ((width < 0) || (height < 0) || (!buffer)) {
        return NULL;
    }

    switch (algo) {
    case DONNELL_SCALING_ALGORITHM_BILINEAR:
        return ImageBuffer_ScaleBL(buffer, width, height);
        break;
    default:
        return ImageBuffer_ScaleNN(buffer, width, height);
    }
}

DONNELL_EXPORT void Donnell_ImageBuffer_BlendBufferContents(DonnellImageBuffer *buffer, DonnellImageBuffer *cbuffer, DonnellRect *srect, DonnellRect *drect) {
    unsigned int i;
    unsigned int j;

    for (i = 0; i < drect->h; ++i) {
        for (j = 0; j < drect->w; ++j) {
            DonnellPixel *pixel;

            pixel = Donnell_ImageBuffer_GetPixel(cbuffer, j, i);

            Donnell_ImageBuffer_BlendPixel(buffer, drect->x + j, drect->y + i, pixel);

            Donnell_Pixel_Free(pixel);
        }
    }
}
