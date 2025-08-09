#ifndef GRID_H
#define GRID_H
#include "raylib.h"
#include <stdlib.h>

typedef struct{
    size_t rows;
    size_t cols;
    int tilesize;
    Color ** grid;
} Grid;

bool ColorIsEqual(Color c1, Color c2);
void grid_create(Grid * grid, size_t rows, size_t cols, int tilesize);
void grid_add_point(Grid * grid, int row, int col, Color color);
Color grid_get_point(Grid * grid, int row, int col);
void grid_fill_color(Grid * grid, int row, int col, Color color, Color replacedColor, bool firstRender);
void grid_create_texture(Grid * grid, RenderTexture2D * texture);
void grid_clear(Grid * grid);
void grid_delete(Grid * grid);

#endif