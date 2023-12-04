#include "donnell.h"

#include "symvis.h"
#include "textrenderer.h"

DONNELL_EXPORT void Donnell_Init(void) {
    TextRenderer_Init();
}

DONNELL_EXPORT void Donnell_Cleanup(void) {
    TextRenderer_Cleanup();
}
