#pragma once
#include "raylib.h"
#include <string>

namespace Starlight {

class AssetManager {
public:
    static std::string resolvePath(const std::string& relativePath) {
        if (FileExists(relativePath.c_str()) || DirectoryExists(relativePath.c_str())) {
            return relativePath;
        }

        std::string up1 = "../" + relativePath;
        if (FileExists(up1.c_str()) || DirectoryExists(up1.c_str())) {
            return up1;
        }

        std::string up2 = "../../" + relativePath;
        if (FileExists(up2.c_str()) || DirectoryExists(up2.c_str())) {
            return up2;
        }

        return relativePath;
    }
};

} // namespace Starlight
