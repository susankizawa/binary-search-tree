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

enum SystemScreen {
    NONE = -1,
    MENU,
    EXIT_REQUESTED,
    STORE_FILE,
    FIND_FILE,
    REMOVE_FILE,
    TREE
};

struct MenuButton {
    const char* label;
    SystemScreen screen;
    Rectangle rect;
};

// const variables
int const NUM_BUTTONS = 5;

// variables that need to persist
MenuButton menuButtons[] = {
    { "Store file", STORE_FILE, { 0 } },
    { "Find file", FIND_FILE, { 0 } },
    { "Remove file", REMOVE_FILE, { 0 } },
    { "View tree", TREE, { 0 } },
    { "Exit", EXIT_REQUESTED, { 0 } }
};
SystemScreen currentScreen;
int selectedButton, mouseHoverRec;
Node* root;
bool exitWindow;

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

void initMenuScreen(){
    currentScreen = MENU;
    selectedButton = 0;
    mouseHoverRec = NONE;
    for(int i = 0; i < NUM_BUTTONS; i++)
        menuButtons[i].rect = (Rectangle) { GetScreenWidth() / 2 - 75, (float)(GetScreenHeight() * 2 / 6 + 50*i), 150.0f, FONT_SIZE + 20 };
    return;
}

void initExitScreen(){
    exitWindow = false;    // Flag to set window to exit
}

void handleMenuScreen(){
    Vector2 mouse = GetMousePosition();

    if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE))
        currentScreen = EXIT_REQUESTED;

    // Mouse hover & selection
    for (int i = 0; i < NUM_BUTTONS; i++) {
        if (CheckCollisionPointRec(mouse, menuButtons[i].rect)) {
            mouseHoverRec = i;
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                selectedButton = i;
                currentScreen = menuButtons[selectedButton].screen;
            }
            break;
        }
    }

    // Keyboard toggle group logic
    if (IsKeyPressed(KEY_DOWN))
        selectedButton++;
    if (IsKeyPressed(KEY_UP))
        selectedButton--;

    selectedButton = (selectedButton + NUM_BUTTONS) % NUM_BUTTONS;

    // Button activation
    if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = menuButtons[selectedButton].screen;
    }

    return;
}

void handleExitScreen(){
    if (currentScreen == EXIT_REQUESTED){
        if (IsKeyPressed(KEY_Y))
            exitWindow = true;
        else if (IsKeyPressed(KEY_N))
            currentScreen = MENU;
    }

    return;
}

void handleStoreFileScreen(){
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = MENU;
    }

    return;
}

void handleFindFileScreen(){
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = MENU;
    }

    return;
}

void handleRemoveFileScreen(){
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = MENU;
    }

    return;
}

void handleTreeScreen(){
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = MENU;
    }

    return;
}

void drawMenuScreen(){
    char* title = "AVL File System";
    int titleSize = FONT_SIZE * 2;
    int titleWidth = MeasureText(title, titleSize);

    DrawText(title, GetScreenWidth() / 2 - titleWidth / 2, GetScreenHeight() * 1 / 6, titleSize, DARKGRAY);

    // Draw rectangles
    for(int i = 0; i < NUM_BUTTONS; i++){
        // selected button, regardless of mouse hover -> skyblue background, blue outline, dark blue text
        // hovered button -> blue background, darkblue outline, black text
        // unfocused button -> light gray background, gray outline, dark gray text
        MenuButton button = menuButtons[i];
        DrawRectangleRec(button.rect, (i == selectedButton) ? SKYBLUE : (i == mouseHoverRec) ? BLUE : LIGHTGRAY);
        DrawRectangleLines((int) button.rect.x,
                           (int) button.rect.y,
                           (int) button.rect.width,
                           (int) button.rect.height,
                           (i == selectedButton) ? BLUE : (i == mouseHoverRec) ? DARKBLUE : GRAY);
        DrawText(button.label,
                (int) (button.rect.x + button.rect.width / 2 - MeasureText(button.label, FONT_SIZE) / 2),
                (int) button.rect.y + 10,
                FONT_SIZE,
                (i == selectedButton) ? DARKBLUE : (i == mouseHoverRec) ? BLACK : DARKGRAY);
    }

    return;
}

void drawExitScreen(){
    DrawRectangle(0, 100, GetScreenWidth(), 200, BLACK);
    DrawText("Are you sure you want to exit program? [Y/N]", 40, 180, 30, WHITE);

    return;
}

void drawStoreFileScreen(){
    char* returnText = "Press ESC to return to main menu";

    char* text = "Store File Screen";

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);

    DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, FONT_SIZE) / 2, 20, FONT_SIZE, DARKGRAY);
    DrawText(text, GetScreenWidth() / 2 - MeasureText(text, FONT_SIZE) / 2, GetScreenHeight() / 2, FONT_SIZE, DARKGREEN);
    return;
}

void drawFindFileScreen(){
    char* returnText = "Press ESC to return to main menu";

    char* text = "Find File Screen";

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), YELLOW);

    DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, FONT_SIZE) / 2, 20, FONT_SIZE, DARKGRAY);
    DrawText(text, GetScreenWidth() / 2 - MeasureText(text, FONT_SIZE) / 2, GetScreenHeight() / 2, FONT_SIZE, DARKBROWN);
    return;
}

void drawRemoveFileScreen(){
    char* returnText = "Press ESC to return to main menu";

    char* text = "Remove File Screen";

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RED);

    DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, FONT_SIZE) / 2, 20, FONT_SIZE, DARKGRAY);
    DrawText(text, GetScreenWidth() / 2 - MeasureText(text, FONT_SIZE) / 2, GetScreenHeight() / 2, FONT_SIZE, MAROON);

    return;
}

void drawTreeScreen(){
    char* returnText = "Press ESC to return to main menu";
    char* title = "View Tree";

    drawBST(root, GetScreenWidth() / 2, 140, 110, 110, 10);

    DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, FONT_SIZE) / 2, 20, FONT_SIZE, DARKGRAY);
    DrawText(title, GetScreenWidth() / 2 - MeasureText(title, FONT_SIZE) / 2, 50, FONT_SIZE, DARKGRAY);

    return;
}

int main(){

    /*
    Node* root = createNode(1, createFile("bom.txt", 0));

    root = insertNodeNonRecur(root, 3, createFile("dia.png", 20));
    root = insertNodeNonRecur(root, 5, createFile("povo.json", 20));
    root = insertNodeNonRecur(root, 4, createFile("batata.cpp", 45));

    */

    root = createNode(1, createFile("never.txt", 0));

    root = insertNodeNonRecur(root, 2, createFile("gonna.txt", 10));
    root = insertNodeNonRecur(root, 3, createFile("give.png", 20));
    root = insertNodeNonRecur(root, 5, createFile("up.json", 20));
    root = insertNodeNonRecur(root, 4, createFile("you.cpp", 45));

    Node* node = findNode(root, 5);

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "binary search tree");
    SetExitKey(KEY_NULL);  // disables automatic ESC exit

    initMenuScreen();
    initExitScreen();

    SetTargetFPS(60);

    while (!exitWindow){

        switch(currentScreen){
            case MENU:
                handleMenuScreen();
                break;
            case EXIT_REQUESTED:
                handleExitScreen();
                break;
            case STORE_FILE:
                handleStoreFileScreen();
                break;
            case FIND_FILE:
                handleFindFileScreen();
                break;
            case REMOVE_FILE:
                handleRemoveFileScreen();
                break;
            case TREE:
                handleTreeScreen();
                break;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            switch(currentScreen){
                case MENU:
                    drawMenuScreen();
                    break;
                case EXIT_REQUESTED:
                    drawExitScreen();
                    break;
                case STORE_FILE:
                    drawStoreFileScreen();
                    break;
                case FIND_FILE:
                    drawFindFileScreen();
                    break;
                case REMOVE_FILE:
                    drawRemoveFileScreen();
                    break;
                case TREE:
                    drawTreeScreen();
                    break;
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;

}
