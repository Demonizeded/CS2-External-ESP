#pragma once
#include "Process.hpp"
#include "ImGuiUtils.hpp"
#include "config.hpp"

namespace core {
	class handleVisual
	{
	public:
		void handleDrawing(graphic::Window* winObj, graphic::D3D11* dx11Obj, graphic::Render* imGuiObj, math::gameData& gD) const;
	};
}

