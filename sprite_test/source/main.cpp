#include <nds.h>
#include <stdio.h>
#include "Sniff-Cat.h" // Make sure this is generated correctly

int main(void) {
    // Optional: For debug output
    consoleDemoInit();

    // Set video mode and VRAM for sprites
    videoSetMode(MODE_0_2D | DISPLAY_SPR_ACTIVE);
    vramSetBankA(VRAM_A_MAIN_SPRITE);
    oamInit(&oamMain, SpriteMapping_1D_128, false);

    // Load sprite graphics and palette
    u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_64x64, SpriteColorFormat_256Color);
    dmaCopy(Sniff_CatTiles, gfx, Sniff_CatTilesLen);
    dmaCopy(Sniff_CatPal, SPRITE_PALETTE, Sniff_CatPalLen);

    int x = 96, y = 64;

    while(pmMainLoop()) {
        scanKeys();
        int keys = keysHeld();

        // Move with D-pad
        if(keys & KEY_UP) y--;
        if(keys & KEY_DOWN) y++;
        if(keys & KEY_LEFT) x--;
        if(keys & KEY_RIGHT) x++;

        // Clamp position
        if(x < 0) x = 0;
        if(x > 256 - 64) x = 256 - 64;
        if(y < 0) y = 0;
        if(y > 192 - 64) y = 192 - 64;

        // Draw sprite
        oamSet(&oamMain, 0, x, y, 0, 0,
               SpriteSize_64x64, SpriteColorFormat_256Color,
               gfx, -1, false, false, false, false, false);

        swiWaitForVBlank();
        oamUpdate(&oamMain);

        // Optional: exit with START
        if(keysDown() & KEY_START) break;
    }

    return 0;
}
