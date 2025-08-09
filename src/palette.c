#include "palette.h"

// Color DEFAULT_COLORS[23] = {
//     LIGHTGRAY, GRAY, DARKGRAY, YELLOW,     
//     GOLD, ORANGE, PINK, RED,        
//     MAROON, GREEN, LIME, DARKGREEN,
//     SKYBLUE, BLUE, DARKBLUE, PURPLE,     
//     VIOLET, DARKPURPLE, BEIGE, BROWN,
//     DARKBROWN, BLACK, WHITE
// };

// void palette_load_default_colors(Palette * palette){
//     for (size_t i = 0; i < COLORS_LENGTH; ++i){
//         palette->colors[i] = DEFAULT_COLORS[i];
//     }
//     palette->showColors = false;
//     palette->currentColorID = 0;
// }

void palette_load(Palette * palette){
    for (size_t i = 0; i < COLORS_LENGTH; ++i){
        palette->colors[i] = BLANK;
    }
    palette->showColors = false;
    palette->currentColorID = 0;
    palette->size = 0;
}

void palette_show_colors(Palette * palette){
    Vector2 screenDim = {GetRenderWidth(), GetRenderHeight()};
    float tileSize = screenDim.x/COLORS_LENGTH;

    for (int i = 0; i < COLORS_LENGTH; ++i){
        if (i == palette->currentColorID){
            DrawRectangle(i*tileSize-2, screenDim.y-tileSize-2, tileSize+4, tileSize+4, MAGENTA);
            DrawRectangle(i*tileSize, screenDim.y-tileSize, tileSize, tileSize, palette->colors[i]);
        }
        else{
            DrawRectangle(i*tileSize, screenDim.y-tileSize, tileSize, tileSize, ColorAlpha(palette->colors[i], 0.2));
        }
    }
}

Color palette_get_color(Palette * palette){
    return palette->colors[palette->currentColorID];
};

void palette_add_color(Palette * palette, Color color){
    palette->colors[palette->size] = color;
    palette->size++;
    if (palette->size >= COLORS_LENGTH){palette->size = 0;}
}