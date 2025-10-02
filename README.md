# 📂 File Management System (AVL Tree)

## 📌 Description  
This project implements a **file management system** using an **AVL Tree** in **C++**, with a graphical interface built on **raylib**.  

The system allows storing, searching, removing, and visualizing files efficiently while keeping the tree balanced.  

Each file contains:  
- **Unique identifier (id)** → integer, used as the search key  
- **Name** → string (without extension)  
- **Extension** → string (e.g., `.txt`, `.jpg`)  
- **Size** → non-negative integer  

## ⚙️ Features  
✅ Insert files into an AVL tree  
✅ Search for files by identifier  
✅ Remove files from the tree  
✅ View the entire tree structure (visualized with Raylib)  
✅ Simple graphical interface with:  
   - Main menu  
   - Store file screen  
   - Find file screen  
   - Remove file screen  
   - Tree visualization screen  
   - Text input boxes and custom buttons  
   - Visual feedback for hover, selection, and errors  

## 🖥️ Project Structure  

All files are in the same folder:  

- **`bst.cpp` / `bst.h`** → AVL tree implementation (binary search tree + balancing logic)  
- **`config.h`** → Global constants (e.g., `MAX_SIZE`, `FONT_SIZE`)  
- **`file_system.cpp` / `file_system.h`** → `File` struct definition and file-related functions (`createFile`, etc.)  
- **`main.cpp`** → Main program loop, screen handling, and integration with Raylib  
- **`queue.cpp` / `queue.h`** → Queue implementation (used for level-order traversal, etc.)  
- **`stack.cpp` / `stack.h`** → Stack implementation (used for iterative traversal, etc.)  

## 🛠️ Requirements  

- [**Raylib**](https://www.raylib.com/)  
- **mingw-w64** compiler (included in Raylib’s `w64devkit`)  
- (Optional) [**Code::Blocks**](https://www.codeblocks.org/downloads) IDE for project management  

## 🔧 Setup Instructions  

### 1. Install dependencies  
- Download and install **Raylib**  
- Install **w64devkit** compiler (comes bundled with Raylib)  

### 2. Configure environment variables  
- Add Raylib and `w64devkit` to your system **PATH**  

### 3. Project configuration (if using Code::Blocks)  
1. Open the project in **Code::Blocks**  
2. Ensure all `.cpp` and `.h` files are included in the project  
3. Configure the global compiler path to Raylib  
   - Example: `C:\raylib\w64devkit`  
4. Under **Build options → Linker settings**, add:  
   - Library:  
     ```
     C:\raylib\w64devkit\x86_64-w64-mingw32\lib\libraylib.a
     ```  
   - Additional linker options:  
     ```
     -lraylib -lgdi32 -lopengl32 -lwinmm
     ```  

### 4. Build and run  
- Compile the project  
- Run the generated executable  

## 🎮 Usage  

- **Main menu** → navigate with arrow keys or mouse click  
- **Store file** → enter id, name, extension, and size, then press **Enter**  
- **Find file** → enter id and press **Enter**  
- **Remove file** → enter id and press **Enter**  
- **View tree** → opens a visualization of the AVL tree using Raylib

## 🚀 Future Improvements  
- Implement input focus for text boxes  
- Improve graphical interface (ready-made buttons, popups, etc.)  
- Save/load AVL tree from external files  
