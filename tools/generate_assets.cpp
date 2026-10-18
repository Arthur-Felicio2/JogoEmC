#include "raylib.h"
#include <iostream>
#include <string>

void generateHands() {
    std::cout << "Generating hand assets..." << std::endl;
    const int W = 512;
    const int H = 512;

    Color colBase = Color{255, 255, 255, 255};
    Color colShadow = Color{0, 0, 0, 60};
    Color colDeepShadow = Color{0, 0, 0, 110};
    Color colOutline = Color{35, 25, 20, 255};

    for (int poseIndex = 0; poseIndex < 6; ++poseIndex) {
        Image imgBase = GenImageColor(W, H, Color{0, 0, 0, 0});
        Image imgShadow = GenImageColor(W, H, Color{0, 0, 0, 0});
        Image imgOutline = GenImageColor(W, H, Color{0, 0, 0, 0});

        // Forearm
        ImageDrawRectangle(&imgBase, 200, 340, 120, 172, colBase);
        ImageDrawRectangle(&imgShadow, 200, 340, 25, 172, colShadow);
        ImageDrawLine(&imgOutline, 200, 340, 200, 512, colOutline);
        ImageDrawLine(&imgOutline, 320, 340, 320, 512, colOutline);

        // Palm base
        ImageDrawCircle(&imgBase, 260, 300, 85, colBase);
        ImageDrawCircle(&imgShadow, 270, 305, 75, colShadow);
        ImageDrawCircleLines(&imgOutline, 260, 300, 85, colOutline);

        // Palm crease lines
        ImageDrawLine(&imgShadow, 220, 280, 290, 320, colDeepShadow);
        ImageDrawLine(&imgShadow, 230, 320, 280, 340, colDeepShadow);

        // Thumb
        bool thumbExtended = (poseIndex == 5);
        if (thumbExtended) {
            ImageDrawCircle(&imgBase, 170, 310, 26, colBase);
            ImageDrawCircle(&imgBase, 140, 280, 24, colBase);
            ImageDrawCircle(&imgShadow, 140, 280, 22, colShadow);
            ImageDrawCircleLines(&imgOutline, 170, 310, 26, colOutline);
            ImageDrawCircleLines(&imgOutline, 140, 280, 24, colOutline);
        } else {
            ImageDrawCircle(&imgBase, 210, 310, 28, colBase);
            ImageDrawCircle(&imgBase, 245, 305, 24, colBase);
            ImageDrawCircle(&imgShadow, 210, 310, 26, colShadow);
            ImageDrawCircleLines(&imgOutline, 210, 310, 28, colOutline);
            ImageDrawCircleLines(&imgOutline, 245, 305, 24, colOutline);
            ImageDrawCircle(&imgShadow, 250, 305, 9, Color{255, 255, 255, 70});
        }

        struct FingerInfo {
            int x;
            int yFolded;
            int yExtended;
            bool extended;
        };

        FingerInfo fingers[4] = {
            { 210, 245, 120, false }, // Index
            { 250, 235, 100, false }, // Middle
            { 290, 240, 125, false }, // Ring
            { 325, 255, 160, false }  // Pinky
        };

        if (poseIndex >= 1) fingers[0].extended = true;
        if (poseIndex >= 2) fingers[1].extended = true;
        if (poseIndex >= 3) fingers[2].extended = true;
        if (poseIndex >= 4) fingers[3].extended = true;
        // Pose 5 is already all extended + thumb

        for (int f = 0; f < 4; ++f) {
            int fx = fingers[f].x;
            int fy = fingers[f].extended ? fingers[f].yExtended : fingers[f].yFolded;
            int r = 18;

            if (fingers[f].extended) {
                int topY = fy;
                int botY = 250;
                ImageDrawCircle(&imgBase, fx, topY, r, colBase);
                ImageDrawRectangle(&imgBase, fx - r, topY, r * 2, botY - topY, colBase);
                ImageDrawCircle(&imgShadow, fx + 4, topY + 4, r - 3, colShadow);
                ImageDrawRectangle(&imgShadow, fx + 5, topY, r - 5, botY - topY, colShadow);

                ImageDrawLine(&imgShadow, fx - 10, topY + 50, fx + 10, topY + 50, colDeepShadow);
                ImageDrawLine(&imgShadow, fx - 10, topY + 90, fx + 10, topY + 90, colDeepShadow);

                ImageDrawCircle(&imgBase, fx, topY, 8, Color{255, 255, 255, 220});
                ImageDrawCircleLines(&imgOutline, fx, topY, 8, colDeepShadow);

                ImageDrawCircleLines(&imgOutline, fx, topY, r, colOutline);
                ImageDrawLine(&imgOutline, fx - r, topY, fx - r, botY, colOutline);
                ImageDrawLine(&imgOutline, fx + r, topY, fx + r, botY, colOutline);
            } else {
                ImageDrawCircle(&imgBase, fx, fy, r + 2, colBase);
                ImageDrawCircle(&imgShadow, fx, fy + 4, r, colShadow);
                ImageDrawCircleLines(&imgOutline, fx, fy, r + 2, colOutline);
                ImageDrawLine(&imgShadow, fx - 10, fy, fx + 10, fy, colDeepShadow);
            }
        }

        std::string basePath = "assets/hands/pose_" + std::to_string(poseIndex) + "_base.png";
        std::string shadowPath = "assets/hands/pose_" + std::to_string(poseIndex) + "_shadow.png";
        std::string outlinePath = "assets/hands/pose_" + std::to_string(poseIndex) + "_outline.png";

        ExportImage(imgBase, basePath.c_str());
        ExportImage(imgShadow, shadowPath.c_str());
        ExportImage(imgOutline, outlinePath.c_str());

        UnloadImage(imgBase);
        UnloadImage(imgShadow);
        UnloadImage(imgOutline);
    }
}

void generateFlappy() {
    std::cout << "Generating flappy assets..." << std::endl;
    // Bird frames (3 frames)
    for (int frame = 0; frame < 3; ++frame) {
        Image bird = GenImageColor(64, 64, Color{0, 0, 0, 0});
        ImageDrawCircle(&bird, 32, 32, 20, Color{244, 185, 66, 255});
        ImageDrawCircleLines(&bird, 32, 32, 20, Color{180, 120, 20, 255});
        ImageDrawCircle(&bird, 42, 26, 6, WHITE);
        ImageDrawCircle(&bird, 44, 26, 3, BLACK);
        // Beak
        ImageDrawLine(&bird, 48, 30, 60, 35, Color{228, 87, 46, 255});
        ImageDrawLine(&bird, 48, 40, 60, 35, Color{228, 87, 46, 255});
        // Wing
        int wingY = 32 + (frame == 0 ? -8 : (frame == 1 ? 0 : 8));
        ImageDrawCircle(&bird, 24, wingY, 10, Color{255, 225, 120, 255});
        ImageDrawCircleLines(&bird, 24, wingY, 10, Color{180, 120, 20, 255});

        std::string path = "assets/flappy/bird_" + std::to_string(frame) + ".png";
        ExportImage(bird, path.c_str());
        UnloadImage(bird);
    }

    // Pipe
    Image pipe = GenImageColor(90, 400, Color{34, 139, 94, 255});
    ImageDrawRectangle(&pipe, 10, 0, 15, 400, Color{56, 189, 130, 255});
    ImageDrawRectangleLines(&pipe, Rectangle{0, 0, 90, 400}, 3, Color{20, 80, 55, 255});
    ExportImage(pipe, "assets/flappy/pipe.png");
    UnloadImage(pipe);

    // Ground
    Image ground = GenImageColor(1280, 70, Color{30, 22, 18, 255});
    ImageDrawRectangle(&ground, 0, 0, 1280, 12, Color{52, 211, 153, 255});
    ExportImage(ground, "assets/flappy/ground.png");
    UnloadImage(ground);
}

void generateUi() {
    std::cout << "Generating UI assets..." << std::endl;
    // Trophy
    Image trophy = GenImageColor(256, 256, Color{0, 0, 0, 0});
    ImageDrawCircle(&trophy, 128, 120, 55, Color{244, 185, 66, 255});
    ImageDrawRectangle(&trophy, 116, 175, 24, 30, Color{217, 145, 32, 255});
    ImageDrawRectangle(&trophy, 80, 205, 96, 24, Color{70, 45, 30, 255});
    ExportImage(trophy, "assets/ui/trofeu.png");
    UnloadImage(trophy);

    // Logo with star and shapes
    Image logo = GenImageColor(512, 128, Color{0, 0, 0, 0});
    ImageDrawCircle(&logo, 64, 64, 40, Color{244, 185, 66, 255});
    ImageDrawCircle(&logo, 64, 64, 25, Color{20, 90, 67, 255});
    ImageDrawRectangle(&logo, 120, 50, 360, 28, Color{244, 185, 66, 255});
    ExportImage(logo, "assets/ui/logo.png");
    UnloadImage(logo);
}

int main() {
    generateHands();
    generateFlappy();
    generateUi();
    std::cout << "All assets successfully generated!" << std::endl;
    return 0;
}
