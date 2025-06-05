#include <nds.h>
#include <stdio.h>
#include "Sniff-Cat.h"  // Include the sprite header

int main(void) {
    // Initialize DS screens
    consoleDemoInit();  // Optional: for debugging with printf
    videoSetMode(MODE_0_2D);      // Mode 0: background + sprites
    vramSetBankA(VRAM_A_MAIN_SPRITE);  // Allocate VRAM for sprites

    // Initialize sprite engine (2D mode)
    oamInit(&oamMain, SpriteMapping_1D_128, false);

    // Set up sprite graphics in memory
    // Reserve space in graphics memory and copy sprite data
    u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_64x64, SpriteColorFormat_256Color);
    dmaCopy(Sniff_CatTiles, gfx, Sniff_CatTilesLen);
    dmaCopy(Sniff_CatPal, SPRITE_PALETTE, Sniff_CatPalLen);

    // Initial position
    int x = 96;
    int y = 64;

    while (1) {
        scanKeys();
        int keys = keysHeld();

        // Move with D-pad
        if (keys & KEY_UP) y--;
        if (keys & KEY_DOWN) y++;
        if (keys & KEY_LEFT) x--;
        if (keys & KEY_RIGHT) x++;

        // Prevent out-of-bounds
        if (x < 0) x = 0;
        if (x > 256 - 64) x = 256 - 64;
        if (y < 0) y = 0;
        if (y > 192 - 64) y = 192 - 64;

        // Draw the sprite
        oamSet(&oamMain,             // Which OAM (main screen)
               0,                    // Sprite ID
               x, y,                 // X/Y position
               0,                    // Priority
               0,                    // Palette index
               SpriteSize_64x64,
               SpriteColorFormat_256Color,
               gfx,                 // Graphics pointer
               -1, false, false, false, false, false);

        swiWaitForVBlank();
        oamUpdate(&oamMain);        // Refresh OAM
    }

    return 0;
}
