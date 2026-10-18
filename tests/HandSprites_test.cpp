#include <iostream>
#include <cassert>
#include "render/HandSprites.h"

int main() {
    std::cout << "[TEST] Running HandSprites_test..." << std::endl;

    assert(sl::HAND_WIDTH == 48);
    assert(sl::HAND_HEIGHT == 64);
    assert(sl::POSE_COUNT == 6);
    assert(sl::SKIN_COUNT == 4);
    assert(sl::SLEEVE_COUNT == 2);
    assert(sl::PIVOT_X == 24);
    assert(sl::PIVOT_Y == 60);

    for (int p = 0; p < sl::POSE_COUNT; ++p) {
        int nonZeroPixels = 0;
        int sleevePixels = 0;
        for (int y = 0; y < sl::HAND_HEIGHT; ++y) {
            for (int x = 0; x < sl::HAND_WIDTH; ++x) {
                uint8_t val = sl::kPosePixels[p][y * sl::HAND_WIDTH + x];
                assert(val <= 8); // Valid indexed palette ranges
                if (val > 0) {
                    nonZeroPixels++;
                }
                if (val >= 5 && val <= 8) {
                    sleevePixels++;
                    // Sleeve should be in the lower area (wrist)
                    assert(y >= 48);
                }
            }
        }
        // Every pose must contain visible hand pixels and sleeve pixels
        assert(nonZeroPixels > 200);
        assert(sleevePixels > 40);
        std::cout << "  -> Pose " << p << " validated with " << nonZeroPixels << " pixels." << std::endl;
    }

    // Verify 4 skin tones
    for (int s = 0; s < sl::SKIN_COUNT; ++s) {
        assert(sl::kSkins[s].name != nullptr);
        assert(sl::kSkins[s].outline.a == 255);
        assert(sl::kSkins[s].base.a == 255);
    }

    // Verify 2 sleeves (Azul / Vermelha)
    for (int slv = 0; slv < sl::SLEEVE_COUNT; ++slv) {
        assert(sl::kSleeves[slv].name != nullptr);
        assert(sl::kSleeves[slv].base.a == 255);
    }

    std::cout << ">> HandSprites_test PASSED successfully!\n" << std::endl;
    return 0;
}
