#include "raylib.h"
#include <iostream>

int main() {
    InitWindow(200, 200, "Test");
    std::cout << "Raylib initialized successfully!" << std::endl;
    CloseWindow();
    return 0;
}
