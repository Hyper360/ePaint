#include "grid.h"
#include "string.h"
#include <stdio.h>

bool ColorIsEqual(Color c1, Color c2){return ColorToInt(c1) == ColorToInt(c2);}

void grid_create(Grid * grid, size_t rows, size_t cols, int tilesize){
    grid->grid = (Color**)malloc(sizeof(Color*)*rows);
    for (int row = 0; row < rows; ++row){
        grid->grid[row] = malloc(sizeof(Color)*cols);
        for (int col = 0; col < cols; ++col){
            grid->grid[row][col] = BLANK;
        }
    }
    grid->rows = rows;
    grid->cols = cols;
    grid->tilesize = tilesize;
}

void grid_add_point(Grid * grid, int row, int col, Color color){
    if (ColorIsEqual(grid_get_point(grid, row, col), color)){return;}
    grid->grid[row][col] = color;
}

Color grid_get_point(Grid * grid, int row, int col){
    return grid->grid[row][col];
}

void grid_fill_color(Grid * grid, int row, int col, Color color, Color replacedColor, bool firstRender){
    if (ColorIsEqual(color, replacedColor)){return;}
    grid->grid[row][col] = color;

    if (color.a != 255){
        BeginScissorMode(col*grid->tilesize, row*grid->tilesize, grid->tilesize, grid->tilesize);
        ClearBackground(color);
        EndScissorMode();
    }
    else{
        DrawRectangle(col*grid->tilesize, row*grid->tilesize, grid->tilesize, grid->tilesize, color);
    }

    int rowStart = row;
    int rowEnd = row;
    int colStart = col;
    int colEnd = col;

    if (row != 0){
        rowStart = row-1;
    } 
    if (row != grid->rows-1){
        rowEnd = row+1;
    }
    if (col != 0){
        colStart = col-1;
    }
    if (col != grid->cols-1){
        colEnd = col+1;
    }

    for (int r = rowStart; r <= rowEnd; ++r){
        for (int c = colStart; c <= colEnd; ++c){
            Color neighbor = grid->grid[r][c];
            if ((r != row || c != col) && ColorIsEqual(neighbor, replacedColor)){
                grid_fill_color(grid, r, c, color, replacedColor, false);
            }
        }
    }
}



void grid_create_texture(Grid * grid, RenderTexture2D * texture){
    int width = grid->rows*grid->tilesize;
    int height = grid->cols*grid->tilesize;
    *texture = LoadRenderTexture(width, height);

    BeginTextureMode(*texture);
    for (int row = 0; row <= grid->rows; ++row){
        int lineStartY = grid->tilesize*row;
        int lineEndX = grid->tilesize*grid->cols;
        DrawLineEx((Vector2){0, lineStartY}, (Vector2){lineEndX, lineStartY}, 2, BLACK);
    }
    for (int col = 0; col <= grid->cols; ++col){
        int lineStartX = grid->tilesize*col;
        int lineEndY = grid->tilesize*grid->rows;
        DrawLineEx((Vector2){lineStartX, 0}, (Vector2){lineStartX, lineEndY}, 2, BLACK);
    }
    EndTextureMode();
}

void grid_clear(Grid * grid){
    for (size_t row = 0; row < grid->rows; ++row){
        for (size_t col = 0; col < grid->cols; ++col){
            grid->grid[row][col] = BLANK;
        }
    }
}

void grid_delete(Grid * grid){
    for (size_t row = 0; row < grid->rows; ++row){
        free(grid->grid[row]);
    }
    free(grid->grid);
}

