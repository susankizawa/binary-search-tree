#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "raylib.h"
#include "config.h"
#include "bst.h"
#include "file_system.h"

// Todo

// store file
// search file
// remove file
// show tree

enum Anchor {
    TOP_LEFT,
    CENTER
};

void drawNode(Node* node, int posX, int posY, Anchor anchor = CENTER, int fontSize = FONT_SIZE){
    int width, height, anchorX, anchorY, boxX, boxY;
    int paddingX = 10;
    int paddingY = 10;
    int spacing = 5;
    int lineHeight = fontSize + spacing;

    const char* lines[4] = {
        TextFormat("id: %d", node->id),
        TextFormat("name: %s", node->file->name.c_str()),
        TextFormat("extension: %s", node->file->extension.c_str()),
        TextFormat("size: %zu", node->file->size)
    };

    // Find max text width
    int maxTextWidth = 0;
    for(int i = 0; i < 4; i++){
        int w = MeasureText(lines[i], fontSize);
        if (w > maxTextWidth)
            maxTextWidth = w;
    }

    width = maxTextWidth + 2 * paddingX;
    height = 4 * lineHeight + 2 * paddingY;

    switch(anchor){
        case TOP_LEFT:
            anchorX = posX;
            anchorY = posY;
            break;
        case CENTER:
            anchorX = posX - width / 2;
            anchorY = posY - height / 2;
            break;
    }

    boxX = anchorX - paddingX;
    boxY = anchorY - paddingY;

    // draws rectangle outline
    DrawRectangle(boxX,
                  boxY,
                  width,
                  height,
                  LIGHTGRAY);
    DrawRectangleLines(boxX,
                       boxY,
                       width,
                       height,
                       BLACK);

    // converts ints into strings
    char idStr[20], sizeStr[20];
    snprintf(idStr, sizeof(idStr), "%d", node->id);
    snprintf(sizeStr, sizeof(sizeStr), "%d", node->file->size);

    // writes node data
    for(int i = 0; i < 4; i++){
        if(i == 0){
            DrawText(lines[i],
             anchorX,
             anchorY + lineHeight * i,
             fontSize,
             BLACK);
        } else{
            DrawText(lines[i],
             anchorX,
             anchorY + lineHeight * i + paddingY,
             fontSize,
             BLACK);
        }
    }

    DrawLine(boxX,
             boxY + lineHeight + paddingY,
             boxX + width,
             boxY + lineHeight + paddingY,
             BLACK);

    /*
    char valString[20];
    char heightString[20];
    snprintf(valString, sizeof(valString), "%d", node->id);
    snprintf(heightString, sizeof(heightString), "%d", node->height);
    int textHeight = fontSize;
    int textWidth = MeasureText(valString, fontSize);
    int heightTextWidth = MeasureText(heightString, fontSize);
    int textMax = std::max(textWidth, textHeight);
    int radius = (textMax / 2) + 10; // radius is half of the larger dimension + some padding

    // draws circle
    DrawCircle(posX, posY, radius, LIGHTGRAY);
    DrawCircleLines(posX, posY, radius, BLACK);

    // draws centered text
    DrawText(valString,
             posX - (textWidth / 2),
             posY - (textHeight / 2),
             fontSize,
             BLACK);

    // draws height
    DrawText(heightString,
             posX - (heightTextWidth / 2),
             posY - 25 - (textHeight),
             fontSize,
             BLACK);
    */

    return;
}

void drawBST(Node* root, int posX, int posY, int offsetX, int offsetY, int fontSize){

    if(root->left){
        int leftChildX = posX - offsetX;
        int leftChildY = posY + offsetY;

        DrawLine(posX, posY, leftChildX, leftChildY, BLACK);
        drawBST(root->left, leftChildX, leftChildY, offsetX / 2, offsetY, fontSize);
    }
    if(root->right){
        int rightChildX = posX + offsetX;
        int rightChildY = posY + offsetY;

        DrawLine(posX, posY, rightChildX, rightChildY, BLACK);
        drawBST(root->right, rightChildX, rightChildY, offsetX / 2, offsetY, fontSize);
    }

    drawNode(root, posX, posY, CENTER, fontSize);

    return;
}

int main(){

    /*
    Node* root = createNode(1, createFile("bom.txt", 0));

    root = insertNodeNonRecur(root, 3, createFile("dia.png", 20));
    root = insertNodeNonRecur(root, 5, createFile("povo.json", 20));
    root = insertNodeNonRecur(root, 4, createFile("batata.cpp", 45));

    */

    Node* root = createNode(1, createFile("never.txt", 0));

    root = insertNodeNonRecur(root, 2, createFile("gonna.txt", 10));
    root = insertNodeNonRecur(root, 3, createFile("give.png", 20));
    root = insertNodeNonRecur(root, 5, createFile("up.json", 20));
    root = insertNodeNonRecur(root, 4, createFile("you.cpp", 45));



    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "binary search tree");

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(RAYWHITE);
            drawBST(root, screenWidth / 2, screenHeight * 1 / 5, 200, 160, 20);
        EndDrawing();
    }

    CloseWindow();

    return 0;

}
