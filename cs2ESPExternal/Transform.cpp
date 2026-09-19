#include "Transform.hpp"
#include "Window.hpp"
#include "Vec.hpp"

namespace math {
	bool WorldToScreen(Vec3 pos, Vec2* scr, float vm[4][4]) {
        Vec4 clipPos;   
        clipPos.x = pos.x * vm[0][0] + pos.y * vm[0][1] + pos.z * vm[0][2] + vm[0][3];
        clipPos.y = pos.x * vm[1][0] + pos.y * vm[1][1] + pos.z * vm[1][2] + vm[1][3];
        clipPos.z = pos.x * vm[2][0] + pos.y * vm[2][1] + pos.z * vm[2][2] + vm[2][3];
        clipPos.w = pos.x * vm[3][0] + pos.y * vm[3][1] + pos.z * vm[3][2] + vm[3][3];

        if (clipPos.w < 0.1f) return false;

        Vec3 NDC;
        NDC.x = clipPos.x / clipPos.w;
        NDC.y = clipPos.y / clipPos.w;
        NDC.z = clipPos.z / clipPos.w;

        scr->x = (graphic::Window::getWidth() / 2 * NDC.x) + (NDC.x + graphic::Window::getWidth() / 2);
        scr->y = -(graphic::Window::getHeight() / 2 * NDC.y) + (NDC.y + graphic::Window::getHeight() / 2);

        return true;
	}
}