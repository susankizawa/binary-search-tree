#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "raylib.h"
#include "config.h"
#include "bst.h"
#include "file_system.h"

// Todo

// getHeight ok
// getBalanceFactor ok
// rotateLeft
// rotateRight
// doubleRotateLeft
// doubleRotateRight

void drawNode(Node* node, int posX, int posY, int fontSize){
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

    drawNode(root, posX, posY, fontSize);

    return;
}

int main(){

    File* f1 = createFile("", -21);
    File* f2 = createFile("batata", 10);
    File* f3 = createFile("pao de queijo.txt", 0);

    printFile(f1);
    printFile(f2);
    printFile(f3);

    /*
    Node* root = createNode(1, createFile());

    root = insertNodeNonRecur(root, 10);
    root = insertNodeNonRecur(root, 2);
    root = insertNodeNonRecur(root, 9);
    root = insertNodeNonRecur(root, 3);
    root = insertNodeNonRecur(root, 8);
    root = insertNodeNonRecur(root, 4);
    root = insertNodeNonRecur(root, 7);
    root = insertNodeNonRecur(root, 6);
    root = insertNodeNonRecur(root, 5);

    */


    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "binary search tree");

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(RAYWHITE);
            //drawBST(root, screenWidth / 2, screenHeight * 1 / 5, 100, 80, 20);
        EndDrawing();
    }

    CloseWindow();

    return 0;

}
