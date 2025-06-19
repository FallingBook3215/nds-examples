#include <nds.h>
#include <stdio.h>
#include "assets/Sniff-Cat.h" // Include the generated header

int main(void) {
    consoleDemoInit();

    // Example: draw the bitmap if it's 16-bit
    // (replace with your actual rendering code)
    int x = 0, y = 0;
    u16* vram = BG_BMP_RAM(0);
    for (int i = 0; i < Sniff_CatBitmapLen / 2; ++i) {
        vram[i] = Sniff_CatBitmap[i];
    }

    while (1) {
        swiWaitForVBlank();
        scanKeys();
        if (keysDown() & KEY_START) break;
    }

    return 0;
}
