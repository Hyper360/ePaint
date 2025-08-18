#include "layer.h"
#include <math.h>

int between(int x, int low, int high){
    if (x < low) return low;
    if (x > high) return high;
    return x;
}

void layer_create(Layer * layer, size_t rows, size_t cols, int tilesize){
    grid_create(&layer->grid, rows, cols, tilesize);
    
    layer->canvas = LoadRenderTexture(rows*tilesize, cols*tilesize);
    layer->width = layer->canvas.texture.width;
    layer->height = layer->canvas.texture.height;
    BeginTextureMode(layer->canvas);
    ClearBackground(BLANK);
    EndTextureMode();
}

void layer_add_point(Layer * layer, Vector2 point, Color color){
    int row = point.y;
    int col = point.x;

    grid_add_point(&layer->grid, row, col, color);

    BeginTextureMode(layer->canvas);
    if (color.a != 255){
        BeginScissorMode(col*layer->grid.tilesize, row*layer->grid.tilesize, layer->grid.tilesize, layer->grid.tilesize);
        ClearBackground(color);
        EndScissorMode();
    }
    else{
        DrawRectangle(col*layer->grid.tilesize, row*layer->grid.tilesize, layer->grid.tilesize, layer->grid.tilesize, color);
    }
    EndTextureMode();
    
}

void layer_add_rectangle(Layer * layer, Vector2 start, Vector2 end, Color color){
    int rowStart = between(fmin(start.y, end.y), 0, layer->grid.rows);
    int rowEnd = between(fmax(start.y, end.y), 0, layer->grid.rows);
    int colStart = between(fmin(start.x, end.x), 0, layer->grid.cols);
    int colEnd = between(fmax(start.x, end.x), 0, layer->grid.cols);

    for (int row = rowStart; row < rowEnd; ++row){
        for (int col = colStart; col < colEnd; ++col){
            grid_add_point(&layer->grid, row, col, color);
            layer_add_point(layer, (Vector2){col, row}, color);
        }
    }
}

void layer_fill_color(Layer * layer, Vector2 point, Color color){
    int row = point.y;
    int col = point.x;

    BeginTextureMode(layer->canvas);
    grid_fill_color(&layer->grid, row, col, color, grid_get_point(&layer->grid, row, col), true);
    EndTextureMode();
}

void layer_draw(Layer * layer){
    float width = layer->canvas.texture.width;
    float height = layer->canvas.texture.height;
    DrawTextureRec(layer->canvas.texture, (Rectangle){0, 0, width, -height}, (Vector2){0, 0}, WHITE);
}

void layer_clear(Layer * layer){
    BeginTextureMode(layer->canvas);
    ClearBackground(BLANK);
    EndTextureMode();

    grid_clear(&layer->grid);
}

void layer_delete(Layer * layer){
    UnloadRenderTexture(layer->canvas);
    grid_delete(&layer->grid);
}

void export_image(eList * layers, int outputSizeX, int outputSizeY, const char * filename){
    Layer * referenceLayer = (Layer*)elist_get(layers, 0); // istg, if you use an uninitialized or empty list, go kys 
    RenderTexture2D texture = LoadRenderTexture(referenceLayer->width, referenceLayer->height);

    BeginTextureMode(texture);
    ClearBackground(BLANK);
    size_t size = layers->size;
    for (size_t i = 0; i < size; ++i){
        Layer * curLayer = (Layer*)elist_get(layers, 0);
        DrawTextureRec(curLayer->canvas.texture, (Rectangle){0, 0, curLayer->width, -curLayer->height}, (Vector2){0, 0}, WHITE);
    }
    EndTextureMode();
    
    
    Image image = LoadImageFromTexture(texture.texture);
    ImageResizeNN(&image, outputSizeX, outputSizeY); // Usign nearest neighbor created the sharp edges needed in pixel art :D
    //ImageRotate(&img, 180); // Render texture draws items to teh texture upside down, and backwards so the image needs to be rotated...
    ImageFlipVertical(&image); // ...and flipped
    ImageFlipHorizontal(&image); // ...and flipped
    if (ExportImage(image, filename) == true){
        printf("image exported successfully to %s\n", filename);
    }
    else{
        printf("image failed to export\n");
    }
    UnloadRenderTexture(texture);
    UnloadImage(image);
}

Rectangle get_grid_rectangle(int rows, int cols, int tilesize, Vector2 mousePos){
    int col = (mousePos.x/tilesize);
    int row = (mousePos.y/tilesize);

    return (Rectangle){col*tilesize, row*tilesize, tilesize, tilesize};
}