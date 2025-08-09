#ifndef LAYER_H
#define LAYER_H
#include "grid.h"
#include "elist.h"
#include "raylib.h"
#include <stdio.h>

typedef struct 
{
    Grid grid;
    int width;
    int height;
    RenderTexture2D canvas;
} Layer;

void layer_create(Layer * layer, size_t rows, size_t cols, int tilesize);
void layer_add_point(Layer * layer, Vector2 point, Color color);
void layer_fill_color(Layer * layer, Vector2 point, Color color);
void layer_draw(Layer * layer);
void layer_clear(Layer * layer);
void layer_delete(Layer * layer);

void export_image(eList * layers, int outputSizeX, int outputSizeY, const char * filename);

#endif