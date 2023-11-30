#include <stdio.h>
#include <stdlib.h>
#include "donnell.h"


int main() {	
	DonnellImageBuffer* buffer;
	DonnellPixel* pixel;
	DonnellPixel* cpixel;
	DonnellSize rect;

	Donnell_Init();

	pixel = Donnell_Pixel_CreateEasy(255, 255, 255, 255);

	cpixel = Donnell_Pixel_CreateEasy(30, 56, 12, 255);

	buffer = Donnell_ImageBuffer_Create(300, 300);
	Donnell_ImageBuffer_Clear(buffer, cpixel);

	Donnell_GraphicsPrimitives_DrawTextLine(buffer, pixel, "اللغة الفارسية", 0, 0, 34, DONNELL_FONT_SANS_SERIF);
	Donnell_GraphicsPrimitives_GetTextLineExtents(&rect, "اللغة الفارسية", 34, DONNELL_FONT_SANS_SERIF);

	printf("%d %d\n", rect.w, rect.h);

	Donnell_ImageBuffer_DumpAsBitmap(buffer, "test.bmp");
	Donnell_ImageBuffer_Free(buffer);
	free(pixel);
	free(cpixel);

	Donnell_Cleanup();
	return 0;
}