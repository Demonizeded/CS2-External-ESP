#pragma once
#include "Vec.hpp"

namespace math {
	bool WorldToScreen(Vec3 pos, Vec2* scr, float vm[4][4]);
}