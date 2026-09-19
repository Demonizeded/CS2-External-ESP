#include "ImGuiUtils.hpp"
#include <iostream>


namespace graphic {
	Render::~Render() {
		if (m_dx11Initalized) ImGui_ImplDX11_Shutdown();
		if (m_win32Initalized) ImGui_ImplWin32_Shutdown();
		if (m_contextInitalized) ImGui::DestroyContext();
	}

	bool Render::initImGui(D3D11* dxObj, Window* winObj) {
		if (!dxObj || !dxObj->getDxDevice() || !dxObj->getDxDeviceCtx()) {
			return false;
		}

		HWND hwnd = winObj->getOHwnd();
		if (!hwnd || !IsWindow(hwnd)) {
			return false;
		}

		IMGUI_CHECKVERSION();
		m_contextInitalized = (ImGui::CreateContext() != nullptr);

		if (!m_contextInitalized) return false;

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		if (!ImGui_ImplWin32_Init(hwnd)) return false;
		else m_win32Initalized = true;

		if (!ImGui_ImplDX11_Init(dxObj->getDxDevice().Get(), dxObj->getDxDeviceCtx().Get())) return false;
		else m_dx11Initalized = true;

		return true;
	}

	void Render::startDrawing() const {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void Render::stopDrawingAndRender(D3D11* dxObj) const {
		ImGui::Render();
		float clear[4] = { 0,0,0,0 };
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = dxObj->getDxRTV();
		dxObj->getDxDeviceCtx()->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
		dxObj->getDxDeviceCtx()->ClearRenderTargetView(rtv.Get(), clear);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		dxObj->getSc()->Present(1, 0);
	}

	void Render::drawRect(float l, float r, float t, float b, ImColor color) const {
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		drawList->AddRect(ImVec2(l, t), ImVec2(r, b), ImColor(0.0f, 0.01f, 0.0f), 0, 0, 3.5f);
		drawList->AddRect(ImVec2(l, t), ImVec2(r, b), color, 0, 0, 3.0f);
	}
		
	void Render::drawSkeleton(const math::Player& p, float currentVm[4][4], ImColor color) const {
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		for (auto b : math::boneConnections) {
			math::Vec2 bA, bB;
			if (!math::WorldToScreen(p.bonePos[b.bone1], &bA, currentVm)) continue;
			if (!math::WorldToScreen(p.bonePos[b.bone2], &bB, currentVm)) continue;

			if (b.bone1 == 7 && b.bone2 == 6) {
				drawList->AddCircle(ImVec2(bA.x, bA.y), std::sqrtf(std::powf(bA.x - bB.x, 2) + std::powf(bA.y - bB.y, 2)), color, 36);
			}

			drawList->AddLine(ImVec2(bA.x, bA.y), ImVec2(bB.x, bB.y), color);
		}
	}

	void Render::drawStats(float centerX, float l, float r, float t, float b, int health) const {
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		ImColor color = { (100 - health) * 2, health * 2, 0 };
		drawList->AddRectFilled(ImVec2(l - 5.0f, b), ImVec2(l, t + (t - b) * (health - 100) / 100), color, 0, 0);
	}

	void Render::drawSnaplines(Window* winObj, float x, float y, ::ImColor color) const {
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		drawList->AddLine(ImVec2(winObj->getWidth() / 2, winObj->getHeight() - 1), ImVec2(x, y), ImColor(0.01f, 0.0f, 0.0f), 2.0f);
		drawList->AddLine(ImVec2(winObj->getWidth() / 2, winObj->getHeight() - 1), ImVec2(x, y), color, 1.0f);
	}

	void Render::drawCrosshair(Window* winObj) const {
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		drawList->AddRectFilled(ImVec2(winObj->getWidth() / 2 - 3, winObj->getHeight() / 2 - 3), ImVec2(winObj->getWidth() / 2 + 3, winObj->getHeight() / 2 + 3), ImColor(0.0f, 0.0f, 1.0f), 1.0f);
	}

	void Render::drawMenu(Window* winObj) const {
		winObj->switchTransparent();
		if (!config::showMenu) return;

		ImGui::Begin("Made By Demonized, Enjoy your Notepad");
		ImGui::SetWindowSize(ImVec2(450, 250));
		ImGui::Checkbox("Draw Skeleton", &config::draw_skeleton);
		ImGui::Checkbox("Draw Boxes", &config::draw_boxes);
		ImGui::Checkbox("Draw Snaplines", &config::draw_snaplines);

		ImGui::ColorEdit3("Enemy Team Color", &config::enemyColor.Value.x);
		ImGui::ColorEdit3("Team Color", &config::friendColor.Value.x);

		ImGui::End();
	}
	
}