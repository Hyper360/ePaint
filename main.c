// #include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#define E_LIST_IMPLEMENTATION
#include "layer.h"
#include "palette.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <float.h>

int main(int argc, char ** argv){
    if (argc != 6){
        printf("USAGE: epaint {rows} {cols} {output image width} {output image height} {output filename}\n");
        return 1;
    }
    int inputRows = atoi(argv[1]);
    int inputCols = atoi(argv[2]);
    int outputSizeX = atoi(argv[3]);
    int outputSizeY = atoi(argv[4]);
    const char * filename = argv[5];
    printf("Desired Dimensions: %d x %d\n", inputRows, inputCols);

    const int WIDTH = 800;
    const int HEIGHT = 600;
    const int TILESIZE = 32;

    InitWindow(WIDTH, HEIGHT, "ePAINT");
    SetTargetFPS(120);
    SetTraceLogLevel(LOG_NONE);
    
    eList layers;
    elist_create(&layers, sizeof(Layer));

    Layer firstLayer;
    layer_create(&firstLayer, inputRows, inputCols, TILESIZE);
    elist_add(&layers, &firstLayer);
    
    Layer * curLayer = (Layer*)elist_get(&layers, 0);
    
    Palette palette;
    palette_load(&palette);
    
    bool colorPickerMode = false;
    Color currentColor = BLACK;
    float alphaFloat = 1.0f;
    
    Camera2D camera;
    camera.target = Vector2Zero();
    camera.offset = Vector2Zero();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    Texture2D background = LoadTexture("checkered.png");
    
    while (!WindowShouldClose()){
        
        Vector2 relMousePos = GetScreenToWorld2D(GetMousePosition(), camera);
        if (CheckCollisionPointRec(relMousePos, (Rectangle){0, 0, curLayer->width, curLayer->height})){
            Vector2 mouseGridPos = {(relMousePos.x/TILESIZE)*TILESIZE, (relMousePos.y/TILESIZE)*TILESIZE};
            
            if (!colorPickerMode){
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                    if (IsKeyDown(KEY_F)) layer_fill_color(curLayer, mouseGridPos,currentColor);
                    else layer_add_point(curLayer, mouseGridPos, currentColor);
                }
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
                    if (IsKeyDown(KEY_F)) layer_fill_color(curLayer, mouseGridPos, BLANK);
                    else layer_add_point(curLayer, mouseGridPos, BLANK);
                }
            }
        }
        if (IsKeyPressed(KEY_DELETE)){
            layer_clear(curLayer);
        }
        if (IsKeyPressed(KEY_D)){
            export_image(&layers, outputSizeX, outputSizeY, filename);
        }
        if (IsKeyPressed(KEY_C)){
            colorPickerMode = !colorPickerMode;
        }
        if (IsKeyPressed(KEY_Q)){
            palette_add_color(&palette, currentColor);
        }
        if (IsKeyDown(KEY_S)){
            palette.showColors = true;
        }
        else{
            palette.showColors = false;
        }
        if (IsKeyPressed(KEY_RIGHT)){
            palette.currentColorID = (palette.currentColorID+1) % 23;
            currentColor = palette_get_color(&palette);
        }
        if (IsKeyPressed(KEY_LEFT)){
            palette.currentColorID = (palette.currentColorID-1) % 23;
            currentColor = palette_get_color(&palette);
        }
        if (palette.currentColorID < 0){
            palette.currentColorID = 23 + palette.currentColorID;
            currentColor = palette_get_color(&palette);
        }
        
        camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove()*0.1f)); // Derived from camera example on raylib site
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)){
            camera.offset = Vector2Add(camera.offset, GetMouseDelta());
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);
        
        BeginMode2D(camera);
        layer_draw(curLayer);
        EndMode2D();
        
        if (palette.showColors){palette_show_colors(&palette);}
        DrawText(TextFormat("ZOOM: %.2f", camera.zoom), 0, 0, 24, RED);

        if (colorPickerMode){
            GuiColorPicker((Rectangle){WIDTH-230, 0, 200, 200}, NULL, &currentColor);

            GuiColorBarAlpha((Rectangle){WIDTH-230, 210, 200, 30}, NULL, &alphaFloat);
            currentColor.a = 255*alphaFloat;

            int redVal = currentColor.r;
            int greenVal = currentColor.g;
            int blueVal = currentColor.b;
            int alphaVal = currentColor.a;
            DrawRectangle(WIDTH-230, 260, 80, 160, RAYWHITE); // For some reason, the ValueBoxes dont have default backgrounds??
            GuiValueBox((Rectangle){WIDTH-230, 260, 80, 40}, "RED", &redVal, 0, 255, IsKeyDown(KEY_R));
            GuiValueBox((Rectangle){WIDTH-230, 300, 80, 40}, "GREEN", &greenVal, 0, 255, IsKeyDown(KEY_G));
            GuiValueBox((Rectangle){WIDTH-230, 340, 80, 40}, "BLUE", &blueVal, 0, 255, IsKeyDown(KEY_B));
            GuiValueBox((Rectangle){WIDTH-230, 380, 80, 40}, "ALPHA", &alphaVal, 0, 255, IsKeyDown(KEY_A));

            currentColor = (Color){redVal, greenVal, blueVal, alphaVal};
            DrawRectangle(WIDTH-115, 260, 80, 160, currentColor);
        }
        
        EndDrawing();
    }
    
    UnloadTexture(background);
    layer_delete(curLayer);
    CloseWindow();
    return 0;
}