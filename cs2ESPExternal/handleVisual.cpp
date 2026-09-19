#include "handleVisual.hpp"

namespace core {
	void handleVisual::handleDrawing(graphic::Window* winObj, graphic::D3D11* dx11Obj, graphic::Render* imGuiObj, math::gameData& gD) const {
        imGuiObj->startDrawing();

        for (const auto& p : gD.players) {
            if (p.health <= 0) continue;

            math::Vec2 feetScr, headScr;
            if (!math::WorldToScreen(p.pos, &feetScr, gD.vm)) continue;

            math::Vec3 headPos = p.pos + math::Vec3{ 0.0f, 0.0f, config::head_offset };
            if (!math::WorldToScreen(headPos, &headScr, gD.vm)) continue;

            float boxHeight = feetScr.y - headScr.y;
            float boxWidth = boxHeight * config::box_ratio;

            math::Vec2 center = (feetScr + headScr) * 0.5f;

            float left = center.x - (boxWidth * 0.5f);
            float right = center.x + (boxWidth * 0.5f);
            float top = headScr.y;
            float bot = feetScr.y;

            if (config::draw_boxes)imGuiObj->drawRect(left, right, top, bot, p.teamNum == gD.teamNum ? ::config::friendColor : ::config::enemyColor);
            if (config::draw_skeleton)imGuiObj->drawSkeleton(p, gD.vm, p.teamNum == gD.teamNum ? ::config::friendColor : ::config::enemyColor);
            if (config::draw_snaplines)imGuiObj->drawSnaplines(winObj, center.x, feetScr.y, p.teamNum == gD.teamNum ? ::config::friendColor : ::config::enemyColor);
            imGuiObj->drawStats(center.x, left, right, top, bot, p.health);
        }
        imGuiObj->drawCrosshair(winObj);
        imGuiObj->drawMenu(winObj);
        imGuiObj->stopDrawingAndRender(dx11Obj);
	}
}
