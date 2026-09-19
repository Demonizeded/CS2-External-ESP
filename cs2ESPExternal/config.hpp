#pragma once
#include "imgui.h"

namespace config {
    inline float head_offset = 72.0f;
    inline float box_ratio = 0.50f;

    inline bool showMenu = false;

    inline bool draw_boxes = true;
    inline bool draw_skeleton = true;
    inline bool draw_snaplines = true;

    inline ::ImColor friendColor = { 0.0f, 0.0f, 1.0f };
    inline ::ImColor enemyColor = { 1.0f, 0.0f, 0.0f };
}