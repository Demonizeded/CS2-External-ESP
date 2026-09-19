#pragma once
#include <iostream>
#include <mutex>
#include "Transform.hpp"
#include "readData.hpp"
#include "handleVisual.hpp"
#include "dataMagazine.hpp"
#include "Window.hpp"
#include "D3D11Ctx.hpp"
#include "ImGuiUtils.hpp"
#include "config.hpp"

namespace core {
	class Engine {
		sys::Process m_prcObj;
		sys::NtWrap m_ntObj;
		graphic::Window m_winObj;
		graphic::D3D11 m_dx11Obj;
		graphic::Render m_imGuiObj;

		dataMagazine m_dataMagazine = {};

		std::atomic_bool m_runningThread{ false };
		std::thread m_memoryThread;
	public:
		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		Engine() = default;
		~Engine();

        bool attach();
		const bool initServices();
		void stopServices();
		void readMemory();
        void handleMain();

	};
}