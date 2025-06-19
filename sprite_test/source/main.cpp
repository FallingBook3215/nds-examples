#include <nds.h>
#include <stdio.h>

int main(void) {
    // Setup bottom screen for 16-bit bitmap
    videoSetModeSub(MODE_5_2D);
    vramSetBankC(VRAM_C_SUB_BG);
    SUB_BG2_CR = BG_BMP16_256x256 | BG_BMP_BASE(0);

    // Copy image to VRAM (assuming 256x192, otherwise add centering logic)
    dmaCopy(Sniff_CatBitmap, BG_BMP_RAM_SUB(0), Sniff_CatBitmapLen);

    while (1) {
        swiWaitForVBlank();
    }
}
