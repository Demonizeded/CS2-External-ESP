#include "Application.hpp"

namespace core {
    Engine::~Engine() {
        stopServices();
    }

    bool Engine::attach() {
        if(!m_prcObj.findProcessID()) return false;
        if(!m_prcObj.findBaseModule()) return false;
        if (!m_ntObj.resolveNtdll()) return false;
        
        HANDLE procHan = m_prcObj.hijackHandle(&m_ntObj);
        if (procHan == nullptr || procHan == INVALID_HANDLE_VALUE) return false;

        m_prcObj.setProcHandle(procHan);

        HWND GHwnd = m_winObj.getWindowByPID(m_prcObj.getTargetPid());
        if (GHwnd) m_winObj.setGHWND(GHwnd);
        else return false;
        return true;
    }

    const bool Engine::initServices() {
        if (!m_winObj.createOverlay()) return false;
        if (!m_dx11Obj.initializeDX11(&m_winObj)) return false;
        if (!m_imGuiObj.initImGui(&m_dx11Obj, &m_winObj)) return false;

        m_runningThread = true;
        m_memoryThread = std::thread(&Engine::readMemory, this);
        return true;
    }


    void Engine::stopServices() {
        m_runningThread = false;

        if (m_memoryThread.joinable()) {
            m_memoryThread.join();
       }
    }

    void Engine::readMemory() {
        readData readDataObj;
        while (m_runningThread.load()) {
            if (!readDataObj.readMemory(&m_prcObj, &m_ntObj, &m_dataMagazine)) {
               
                m_runningThread = false;
                return;
            }
            Sleep(5);
        }

    }

    void Engine::handleMain() {
        handleVisual hV;
        bool lastWasDown = false;
        while (m_runningThread.load()) {
            if (!m_winObj.checkMessages()) {
                stopServices();
                break;
            }
            bool isKeyDown = false;
            if ((GetAsyncKeyState(VK_INSERT) & 0x8000) != 0) isKeyDown = true;
            if (!lastWasDown && isKeyDown) config::showMenu = !config::showMenu;
            lastWasDown = isKeyDown;

            if ((GetAsyncKeyState(VK_END) & 0x8000) != 0) {
                stopServices();
                break;
            }

            math::gameData readyData = m_dataMagazine.getGameData();
            hV.handleDrawing(&m_winObj, &m_dx11Obj, &m_imGuiObj, readyData);
        }
    }
}