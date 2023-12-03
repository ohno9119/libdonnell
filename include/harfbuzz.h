#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef USE_HARFBUZZ_HEADERS
#include <hb.h>
#endif

#include "bidi.h"
#include "donnell.h"
#include "fontconfig.h"
#include "freetype.h"

#ifndef DONNELL_HARFBUZZ
#define DONNELL_HARFBUZZ

#ifdef USE_HARFBUZZ_HEADERS
typedef hb_buffer_t *HarfBuzzBuffer;
typedef hb_font_t *HarfBuzzFont;

typedef hb_glyph_info_t HarfBuzzGlyphInfo;
typedef hb_glyph_position_t HarfBuzzGlyphPos;
#else
typedef void *HarfBuzzBuffer;
typedef void *HarfBuzzFont;

typedef union {
    FT_UInt32 u32;
    FT_Int32 i32;
    FT_UInt16 u16[2];
    FT_Int16 i16[2];
    unsigned char u8[4];
    char i8[4];
} HarfBuzzVarInt;

typedef struct {
    FT_Int32 x_advance;
    FT_Int32 y_advance;
    FT_Int32 x_offset;
    FT_Int32 y_offset;
    HarfBuzzVarInt var;
} HarfBuzzGlyphPos;

typedef struct {
    FT_UInt32 codepoint;
    FT_UInt32 mask;
    FT_UInt32 cluster;
    HarfBuzzVarInt var1;
    HarfBuzzVarInt var2;
} HarfBuzzGlyphInfo;
#endif

typedef HarfBuzzBuffer (*HarfBuzzBufferCreate)(void);
typedef void (*HarfBuzzBufferAdd)(HarfBuzzBuffer, const FT_UInt32 *, int, unsigned int, int);
typedef void (*HarfBuzzBufferAddUTF8)(HarfBuzzBuffer, const char *, int, unsigned int, int);
typedef void (*HarfBuzzBufferDestroy)(HarfBuzzBuffer);
typedef void (*HarfBuzzBufferGuess)(HarfBuzzBuffer);

typedef HarfBuzzFont (*HarfBuzzFontCreate)(FT_Face, void *);
typedef void (*HarfBuzzFontSetup)(HarfBuzzFont);
typedef void (*HarfBuzzFontDestroy)(HarfBuzzFont);

typedef void (*HarfBuzzShape)(HarfBuzzFont, HarfBuzzBuffer, void *, unsigned int);

typedef HarfBuzzGlyphInfo *(*HarfBuzzGetGlyphInfos)(HarfBuzzBuffer, unsigned int *);
typedef HarfBuzzGlyphPos *(*HarfBuzzGetGlyphPositions)(HarfBuzzBuffer, unsigned int *);

typedef struct {
    HarfBuzzBufferCreate buffer_create;
    HarfBuzzBufferAdd buffer_add;
    HarfBuzzBufferAddUTF8 buffer_add_utf8;
    HarfBuzzBufferGuess buffer_guess;
    HarfBuzzBufferDestroy buffer_destroy;

    HarfBuzzFontCreate font_create;
    HarfBuzzFontSetup font_setup;
    HarfBuzzFontDestroy font_destroy;

    HarfBuzzShape shape;

    HarfBuzzGetGlyphInfos get_glyph_infos;
    HarfBuzzGetGlyphPositions get_glyph_positions;

    void *library;
} HarfBuzzLibrary;

void HarfBuzz_Init(void);
void HarfBuzz_MeasureAndRender(DonnellImageBuffer *buffer, DonnellSize *size, DonnellPixel *color, FriBidiString *string, unsigned int x, unsigned int y, unsigned int pixel_size, DonnellFont req_font, FT_Bool return_max_asc, DonnellFontStyle font_style);
void HarfBuzz_Cleanup(void);
HarfBuzzLibrary *HarfBuzz_GetLibrary(void);

#endif
