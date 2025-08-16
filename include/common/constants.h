#pragma once

namespace Constants {
    constexpr int GAP = 5;          // Pixel gap for selection
    constexpr int DEFAULT_WIDTH = 100;
    constexpr int DEFAULT_HEIGHT = 150;
    constexpr double PI = 3.14159265358979323846;
    
    // UI Constants
    constexpr int CANVAS_WIDTH = 1024;
    constexpr int CANVAS_HEIGHT = 768;
    constexpr int TOOLBAR_SIZE = 48;
    
    // File format
    constexpr const char* FILE_SIGNATURE = "PAINTER";
    constexpr uint32_t FILE_VERSION = 0x0101; // Version 1.1
}