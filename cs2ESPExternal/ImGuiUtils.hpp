#pragma once
#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include "Window.hpp"
#include "D3D11Ctx.hpp"
#include "Vec.hpp"
#include "config.hpp"
#include "Transform.hpp"

namespace graphic {
	class Render {
		bool m_contextInitalized = false;
		bool m_dx11Initalized = false;
		bool m_win32Initalized = false;

		//ImDrawList* drawList;

	public:
		Render(const Render&) = delete;
		Render& operator=(const Render&) = delete;

		Render() = default;
		~Render();

		bool initImGui(D3D11* dxObj, Window* winObj);

		void startDrawing() const;
		void stopDrawingAndRender(D3D11* dxObj) const;
		void drawRect(float l, float r, float t, float b, ::ImColor color) const;
		void drawStats(float centerX, float l, float r, float t, float b, int health) const;
		void drawSnaplines(Window* winObj, float x, float y, ::ImColor color) const;
		void drawSkeleton(const math::Player& p, float currentVm[4][4], ::ImColor color) const;
		void drawCrosshair(Window* winObj) const;
		void drawMenu(Window* winObj) const;
	};
}