#ifndef PALETTE_H
#define PALETTE_H
#include "raylib.h"
#include <stdlib.h>

#define COLORS_LENGTH 23

typedef struct{
    Color colors[COLORS_LENGTH];
    bool showColors;
    int currentColorID;
    int size;
} Palette;

//void palette_load_default_colors(Palette * palette);
void palette_load(Palette * palette);
void palette_show_colors(Palette * palette);
Color palette_get_color(Palette * palette);
void palette_add_color(Palette * palette, Color color);

#endif