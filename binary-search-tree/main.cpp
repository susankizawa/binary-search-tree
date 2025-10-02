#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    TREE,
    FOUND_FILE,
    REMOVED_FILE,
    FILE_NOT_FOUND,
    FILE_STORED,
    DUPLICATE
};

typedef struct MenuButton {
    const char* label;
    SystemScreen screen;
    Rectangle rect;
} MenuButton;

typedef struct TextInput {
    const char* label;
    char buffer[128];
    int length;
    int maxLength;
    Rectangle rect;
    bool mouseOnText;
} TextInput;

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
TextInput findFileIdInput, removeFileIdInput, storeFileIdInput, storeFileFilenameInput, storeFileSizeInput;
int framesCounter;

void drawNode(Node* node, int posX, int posY, Anchor anchor = CENTER, int fontSize = FONT_SIZE){
    int width, height, anchorX, anchorY, boxX, boxY;
    int paddingX = 10;
    int paddingY = 10;
    int spacing = 5;
    int lineHeight = fontSize + spacing;

    if(node == NULL){
        const char* msg = "Error 404: node was not found";
        width = MeasureText(msg, fontSize) + paddingX * 2;
        height = FONT_SIZE + paddingY * 2;

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

        DrawRectangle(anchorX - paddingX, anchorY - paddingY, width, height, LIGHTGRAY);
        DrawRectangleLines(anchorX - paddingX, anchorY - paddingY, width, height, GRAY);
        DrawText(msg, anchorX, anchorY, fontSize, RED);
        return;
    }

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
    if(root == NULL){
        drawNode(root, posX, posY, CENTER, fontSize);
        return;
    }

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

TextInput createTextInput(char* label, int maxLength, int x, int y, int width, int height){
    TextInput newTextInput = {
        label,
        "",
        0,
        maxLength,
        { x, y, width, height },
        false
    };

    return newTextInput;
}

void drawTextInput(TextInput input) {
    if(input.mouseOnText){
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    } else{
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    // draw the label to the left of the box
    DrawText(input.label,
             input.rect.x - MeasureText(input.label, FONT_SIZE) - 10,  // place 10px left of box
             input.rect.y + (input.rect.height / 2 - 10),        // vertical align
             FONT_SIZE,
             DARKGRAY);

    // draw the input rectangle
    DrawRectangleRec(input.rect, input.mouseOnText ? RAYWHITE : LIGHTGRAY);
    DrawRectangleLinesEx(input.rect, 2, GRAY);

    // draw the text inside the box
    DrawText(input.buffer, input.rect.x + 5, input.rect.y + 10, FONT_SIZE, DARKGRAY);

    // draw cursor if active
    if (input.mouseOnText) {
        // Draw blinking underscore char
        if (((framesCounter / 20) % 2) == 0)
            DrawText("|", (int)input.rect.x + 8 + MeasureText(input.buffer, FONT_SIZE), (int) input.rect.y + 12, FONT_SIZE, BLUE);
    }

    return;
}

void updateTextInput(TextInput* input) {
    if (CheckCollisionPointRec(GetMousePosition(), input->rect))
        input->mouseOnText = true;
    else
        input->mouseOnText = false;

    if(input->mouseOnText){
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125 && input->length < input->maxLength) {
                input->buffer[input->length] = (char) key;
                input->buffer[input->length + 1] = '\0';
                input->length++;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && input->length > 0) {
            input->length--;
            if (input->length < 0)
                input->length = 0;
            input->buffer[input->length] = '\0';
        }
    }

    return;
}

void initMenuScreen(){
    framesCounter = 0;
    currentScreen = MENU;
    selectedButton = 0;
    mouseHoverRec = NONE;
    for(int i = 0; i < NUM_BUTTONS; i++)
        menuButtons[i].rect = (Rectangle) { GetScreenWidth() / 2 - 75, (float)(GetScreenHeight() * 2 / 6 + 50*i), 150.0f, FONT_SIZE + 20 };
    return;
}

void initExitScreen(){
    exitWindow = false;    // Flag to set window to exit
    return;
}

void initFindFileScreen(){
    int width = 200;

    findFileIdInput = createTextInput(
                                      "File id",
                                      10,
                                      GetScreenWidth() / 2 - width / 2,
                                      GetScreenHeight() / 2 - FONT_SIZE / 2,
                                      width,
                                      FONT_SIZE + 20
                                     );

    return;
}

void initRemoveFileScreen(){
    int width = 200;

    removeFileIdInput = createTextInput(
                                      "File id",
                                      10,
                                      GetScreenWidth() / 2 - width / 2,
                                      GetScreenHeight() / 2 - FONT_SIZE / 2,
                                      width,
                                      FONT_SIZE + 20
                                     );

    return;
}

void initStoreFileScreen(){
    int width = 400;
    int lineHeight = FONT_SIZE + 50;

    storeFileIdInput = createTextInput(
                                      "File id",
                                      10,
                                      GetScreenWidth() / 2 - width / 2,
                                      GetScreenHeight() * 1 / 3,
                                      width,
                                      FONT_SIZE + 20
                                     );
    storeFileFilenameInput = createTextInput(
                                      "File name",
                                      30,
                                      GetScreenWidth() / 2 - width / 2,
                                      GetScreenHeight() * 1 / 3 + lineHeight,
                                      width,
                                      FONT_SIZE + 20
                                     );
    storeFileSizeInput = createTextInput(
                                      "File size",
                                      10,
                                      GetScreenWidth() / 2 - width / 2,
                                      GetScreenHeight() * 1 / 3 + lineHeight * 2,
                                      width,
                                      FONT_SIZE + 20
                                     );

    return;
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
    updateTextInput(&storeFileIdInput);
    updateTextInput(&storeFileFilenameInput);
    updateTextInput(&storeFileSizeInput);

    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = MENU;
    }

    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)){
        if(storeFileIdInput.length == 0){
            printf("error: no id number was input\n");
            return;
        }
        if(storeFileFilenameInput.length == 0){
            printf("error: no file name was input\n");
            return;
        }
        if(storeFileSizeInput.length == 0){
            printf("error: no file size was input\n");
            return;
        }

        char* end;

        int id = (int) strtol(storeFileIdInput.buffer, &end, 10);

        if(*end != '\0' ){
            printf("error: id needs to be a number\n");
            return;
        }

        int fileSize = (int) strtol(storeFileSizeInput.buffer, &end, 10);

        if(*end != '\0' ){
            printf("error: size needs to be a number\n");
            return;
        }

        if(findNode(root, id)){
            currentScreen = DUPLICATE;
            return;
        }

        File* newFile = createFile(storeFileFilenameInput.buffer, fileSize);

        root = insertNode(root, id, newFile);

        currentScreen = FILE_STORED;
    }

    return;
}

Node* handleFindFileScreen(){
    updateTextInput(&findFileIdInput);

    if(IsKeyPressed(KEY_ESCAPE)){
        currentScreen = MENU;
    }

    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)){
        if(findFileIdInput.length == 0){
            printf("error: no number was input\n");
            return NULL;
        }

        char* end;
        int id = (int) strtol(findFileIdInput.buffer, &end, 10);

        if(*end != '\0' ){
            printf("error: id needs to be a number\n");
            return NULL;
        }

        Node* node = findNode(root, id);

        if(node){
            currentScreen = FOUND_FILE;
            return findNode(root, id);
        } else{
            currentScreen = FILE_NOT_FOUND;
            return NULL;
        }
    }

    return NULL;
}

void handleRemoveFileScreen(){
    updateTextInput(&removeFileIdInput);

    if(IsKeyPressed(KEY_ESCAPE)){
        currentScreen = MENU;
    }

    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)){
        if(removeFileIdInput.length == 0){
            printf("error: no number was input\n");
            return;
        }

        char* end;
        int id = (int) strtol(removeFileIdInput.buffer, &end, 10);

        if(*end != '\0' ){
            printf("error: id needs to be a number\n");
            return;
        }

        Node* node = findNode(root, id);

        if(node){
            root = removeNode(root, id);
            currentScreen = REMOVED_FILE;
        } else{
            currentScreen = FILE_NOT_FOUND;
        }
    }

    return;
}

void handleTreeScreen(){
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = MENU;
    }

    return;
}

void handleFoundFileScreen(){
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = FIND_FILE;
    }

    return;
}

void handleRemovedFileScreen(){
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = REMOVE_FILE;
    }

    return;
}

void handleFileNotFoundScreen(){
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = MENU;
    }

    return;
}

void handleFileStoredScreen(){
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = MENU;
    }

    return;
}

void handleDuplicateScreen(){
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = STORE_FILE;
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
    char* title = "Store file";
    char* storeFileText = "Press ENTER to store file";

    DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, FONT_SIZE) / 2, 20, FONT_SIZE, DARKGRAY);
    DrawText(title, GetScreenWidth() / 2 - MeasureText(title, FONT_SIZE) / 2, 50, FONT_SIZE, DARKGRAY);

    drawTextInput(storeFileIdInput);
    drawTextInput(storeFileFilenameInput);
    drawTextInput(storeFileSizeInput);

    DrawText(storeFileText, GetScreenWidth() / 2 - MeasureText(storeFileText, FONT_SIZE) / 2, GetScreenHeight() - 50, FONT_SIZE, DARKGRAY);

    return;
}

void drawFindFileScreen(){
    char* returnText = "Press ESC to return to main menu";
    char* title = "Find file";
    char* findFileText = "Press ENTER to find file";

    DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, FONT_SIZE) / 2, 20, FONT_SIZE, DARKGRAY);
    DrawText(title, GetScreenWidth() / 2 - MeasureText(title, FONT_SIZE) / 2, 50, FONT_SIZE, DARKGRAY);

    drawTextInput(findFileIdInput);

    DrawText(findFileText, GetScreenWidth() / 2 - MeasureText(findFileText, FONT_SIZE) / 2, GetScreenHeight() - 50, FONT_SIZE, DARKGRAY);

    return;
}

void drawRemoveFileScreen(){
    char* returnText = "Press ESC to return to main menu";
    char* title = "Remove file";
    char* removeFileText = "Press ENTER to remove file";

    DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, FONT_SIZE) / 2, 20, FONT_SIZE, DARKGRAY);
    DrawText(title, GetScreenWidth() / 2 - MeasureText(title, FONT_SIZE) / 2, 50, FONT_SIZE, DARKGRAY);

    drawTextInput(removeFileIdInput);

    DrawText(removeFileText, GetScreenWidth() / 2 - MeasureText(removeFileText, FONT_SIZE) / 2, GetScreenHeight() - 50, FONT_SIZE, DARKGRAY);

    return;
}

void drawTreeScreen(){
    char* returnText = "Press ESC to return to main menu";
    char* title = "View tree";

    drawBST(root, GetScreenWidth() / 2, 140, 110, 110, 10);

    DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, FONT_SIZE) / 2, 20, FONT_SIZE, DARKGRAY);
    DrawText(title, GetScreenWidth() / 2 - MeasureText(title, FONT_SIZE) / 2, 50, FONT_SIZE, DARKGRAY);

    return;
}

void drawFoundFileScreen(Node* foundFile){
    char* returnText = "Press ESC to return to the previous screen";
    char* title = "Found file";

    drawNode(foundFile, GetScreenWidth() / 2, GetScreenHeight() / 2, CENTER, FONT_SIZE);

    DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, FONT_SIZE) / 2, 20, FONT_SIZE, DARKGRAY);
    DrawText(title, GetScreenWidth() / 2 - MeasureText(title, FONT_SIZE) / 2, 50, FONT_SIZE, DARKGRAY);

    return;
}

void drawRemovedFileScreen(){
    char* returnText = "Press ESC to return to previous screen";

    char* text = "Successfully removed file!";

    DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, FONT_SIZE) / 2, 20, FONT_SIZE, DARKGRAY);

    DrawText(text, GetScreenWidth() / 2 - MeasureText(text, FONT_SIZE) / 2, GetScreenHeight() / 2, FONT_SIZE, DARKGREEN);
    return;
}

void drawFileNotFoundScreen(){
    char* returnText = "Press ESC to return to main menu";

    char* text = "Error 404: file not found";

    DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, FONT_SIZE) / 2, 20, FONT_SIZE, DARKGRAY);
    DrawText(text, GetScreenWidth() / 2 - MeasureText(text, FONT_SIZE) / 2, GetScreenHeight() / 2, FONT_SIZE, RED);

    return;
}

void drawFileStoredScreen(){
    char* returnText = "Press ESC to return to main menu";
    char* text = "Successfully stored file";

    DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, FONT_SIZE) / 2, 20, FONT_SIZE, DARKGRAY);
    DrawText(text, GetScreenWidth() / 2 - MeasureText(text, FONT_SIZE) / 2, GetScreenHeight() / 2, FONT_SIZE, GREEN);

    return;
}

void drawDuplicateScreen(){
    char* returnText = "Press ESC to return to previous screen";

    char* text = "File with that id already exists";

    DrawText(returnText, GetScreenWidth() / 2 - MeasureText(returnText, FONT_SIZE) / 2, 20, FONT_SIZE, DARKGRAY);
    DrawText(text, GetScreenWidth() / 2 - MeasureText(text, FONT_SIZE) / 2, GetScreenHeight() / 2, FONT_SIZE, RED);

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

    Node* foundFile = NULL;

    InitWindow(screenWidth, screenHeight, "AVL File System");
    SetExitKey(KEY_NULL);  // disables automatic ESC exit

    initMenuScreen();
    initExitScreen();
    initFindFileScreen();
    initRemoveFileScreen();
    initStoreFileScreen();

    SetTargetFPS(60);

    while (!exitWindow){
        framesCounter = (framesCounter + 1) % 60;

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
                foundFile = handleFindFileScreen();
                break;
            case REMOVE_FILE:
                handleRemoveFileScreen();
                break;
            case TREE:
                handleTreeScreen();
                break;
            case FOUND_FILE:
                handleFoundFileScreen();
                break;
            case REMOVED_FILE:
                handleRemovedFileScreen();
                break;
            case FILE_NOT_FOUND:
                handleFileNotFoundScreen();
                break;
            case FILE_STORED:
                handleFileStoredScreen();
                break;
            case DUPLICATE:
                handleDuplicateScreen();
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
                case FOUND_FILE:
                    drawFoundFileScreen(foundFile);
                    break;
                case REMOVED_FILE:
                    drawRemovedFileScreen();
                    break;
                case FILE_NOT_FOUND:
                    drawFileNotFoundScreen();
                    break;
                case FILE_STORED:
                    drawFileStoredScreen();
                    break;
                case DUPLICATE:
                    drawDuplicateScreen();
                    break;
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;

}
