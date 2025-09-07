#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "raylib.h"
#include "config.h"
#include "bst_non_recur.h"

// Todo

// getHeight ok
// getBalanceFactor ok
// rotateLeft
// rotateRight
// doubleRotateLeft
// doubleRotateRight

void drawNode(Node* node, int posX, int posY, int fontSize){
    char valString[20];
    snprintf(valString, sizeof(valString), "%d", node->value);
    int textHeight = fontSize;
    int textWidth = MeasureText(valString, fontSize);
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
    Node* root = createNode(50);

    insertNodeNonRecur(root, 10);
    insertNodeNonRecur(root, 90);
    insertNodeNonRecur(root, 20);
    insertNodeNonRecur(root, 30);
    insertNodeNonRecur(root, 80);
    insertNodeNonRecur(root, 60);
    insertNodeNonRecur(root, 40);
    insertNodeNonRecur(root, 70);

    /*
    rotateLeft(findNodeNonRecur(root, 20));
    rotateLeft(findNodeNonRecur(root, 10));

    rotateLeft(findNodeNonRecur(root, 60));
    rotateRight(findNodeNonRecur(root, 80));
    rotateRight(findNodeNonRecur(root, 90));
    */

    rotateLeft(findNodeNonRecur(root, 20));
    rotateLeft(findNodeNonRecur(root, 30));

    std::string message = "Batata waz here";

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "binary search tree");

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(RAYWHITE);
            drawBST(root, screenWidth / 2, screenHeight * 1 / 5, 100, 80, 20);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

