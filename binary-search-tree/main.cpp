#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "raylib.h"
#include "config.h"
#include "bst.h"

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
    /*
    Node* rootRec = createNode(10);
    Node* rootNonRec = createNode(10);

    printf("--- Building trees ---\n");

    // Build recursive tree
    insertNode(rootRec, 5);
    insertNode(rootRec, 15);
    insertNode(rootRec, 3);   // leaf
    insertNode(rootRec, 7);   // will have one child
    insertNode(rootRec, 6);   // child of 7
    insertNode(rootRec, 13);
    insertNode(rootRec, 17);

    // Build non-recursive tree (same structure)
    insertNodeNonRecur(rootNonRec, 5);
    insertNodeNonRecur(rootNonRec, 15);
    insertNodeNonRecur(rootNonRec, 3);
    insertNodeNonRecur(rootNonRec, 7);
    insertNodeNonRecur(rootNonRec, 6);
    insertNodeNonRecur(rootNonRec, 13);
    insertNodeNonRecur(rootNonRec, 17);

    printf("\nRecursive tree (in-order): ");
    inOrderPrint(rootRec);
    printf("\nNon-recursive tree (in-order): ");
    inOrderPrintNonRecur(rootNonRec);
    printf("\n\n");

    printf("Tree built.\n");

    // --- Duplicate insert test ---
    printf("Recursive insert duplicate (10):\n");
    insertNode(rootRec, 10);   // test duplicate insert
    inOrderPrint(rootRec);
    printf("\n");

    printf("Non recursively insert duplicate (10):\n");
    insertNodeNonRecur(rootNonRec, 10);   // test duplicate insert
    inOrderPrintNonRecur(rootNonRec);
    printf("\n");

    // --- Height tests ---
    printf("Height (recursive): %d\n", getHeight(rootRec));
    printf("Height (non-recursive): %d\n\n", getHeightNonRecur(rootNonRec));

    printf("Empty tree height (recursive): %d\n", getHeight(NULL));
    printf("Empty tree height (non-recursive): %d\n\n", getHeightNonRecur(NULL));

    // --- Traversal tests ---
    printf("PreOrder (recursive): ");
    preOrderPrint(rootRec);
    printf("\nPreOrder (non-recursive): ");
    preOrderPrintNonRecur(rootNonRec);

    printf("\n\nInOrder (recursive): ");
    inOrderPrint(rootRec);
    printf("\nInOrder (non-recursive): ");
    inOrderPrintNonRecur(rootNonRec);

    printf("\n\nPostOrder (recursive): ");
    postOrderPrint(rootRec);
    printf("\nPostOrder (non-recursive): ");
    postOrderPrintNonRecur(rootNonRec);

    printf("\n\nLevelOrder (recursive tree only): ");
    levelOrderPrint(rootRec);
    printf("\n\n");

    // --- Deletion tests ---
    printf("Recursive deletions:\n");
    removeNode(rootRec, 3);   // leaf
    removeNode(rootRec, 7);   // one child
    removeNode(rootRec, 15);  // two children
    removeNode(rootRec, 99);  // not found
    levelOrderPrint(rootRec);
    printf("\n");

    printf("Non-recursive deletions:\n");
    removeNodeNonRecur(rootNonRec, 3);   // leaf
    removeNodeNonRecur(rootNonRec, 7);   // one child
    removeNodeNonRecur(rootNonRec, 15);  // two children
    removeNodeNonRecur(rootNonRec, 99);  // not found
    levelOrderPrint(rootNonRec);
    printf("\n");

    printf("\nPress any button to conclude the test");
    getchar();

    return 0;

    */

    Node* root = createNode(50);

    insertNode(root, 40);
    insertNode(root, 10);
    insertNode(root, 30);
    insertNode(root, 60);
    insertNode(root, 20);
    insertNode(root, 80);
    insertNode(root, 70);
    insertNode(root, 90);

    balanceTree(root);

    //Node* n;

    // do this to update node parent after rotating it
    /*
    Node* n = findNode(root, 20);
    Node* p = n->parent;

    if(p->left == n)
       p->left = rotateLeft(n);
    else
        p->right = rotateLeft(n);
    */

    /*
    n = findNode(root, 30);
    p = n->parent;

    if(p->left == n)
       p->left = rotateLeft(n);
    else
        p->right = rotateLeft(n);
    */

    // do this if rotated node is root
    /*
    n = findNode(root, 50);
    p = n->parent;

    root = rotateLeft(n);
    */

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
