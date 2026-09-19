#include "Process.hpp"

namespace sys {
    Process::Process() {
        m_pid = NULL;
        m_modBase = NULL;
        m_procHan = nullptr;
    }

    Process::~Process() {
        m_pid = NULL;
        m_modBase = NULL;
        CloseHandle(m_procHan);
    }
    
    DWORD Process::findProcessID() {
        HANDLE snapProc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32W pe;
        pe.dwSize = sizeof(pe);
        DWORD pid = NULL;
        if (Process32FirstW(snapProc, &pe)) {
            if (!_wcsicmp(L"cs2.exe", pe.szExeFile)) {
                pid = pe.th32ProcessID;
                CloseHandle(snapProc);
                m_pid = pid;
                return pid;
            }
        }
        while (pid == NULL && Process32NextW(snapProc, &pe)) {
            if (!_wcsicmp(L"cs2.exe", pe.szExeFile)) {
                pid = pe.th32ProcessID;
                CloseHandle(snapProc);
                m_pid = pid;
                return pid;
            }
        }
        CloseHandle(snapProc);
        m_pid = pid;
        return pid;
    }

    uintptr_t Process::findBaseModule() {
        HANDLE snapMod = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, Process::getTargetPid());
        MODULEENTRY32W me;
        me.dwSize = sizeof(me);
        uintptr_t modBase = NULL;
        if (Module32FirstW(snapMod, &me)) {
            if (!_wcsicmp(L"client.dll", me.szModule)) {
                modBase = (uintptr_t)me.modBaseAddr;
                CloseHandle(snapMod);
                m_modBase = modBase;
                return modBase;
            }
        }
        while (modBase == NULL && Module32NextW(snapMod, &me)) {
            if (!_wcsicmp(L"client.dll", me.szModule)) {
                modBase = (uintptr_t)me.modBaseAddr;
                CloseHandle(snapMod);
                m_modBase = modBase;
                return modBase;
            }
        }
        CloseHandle(snapMod);
        m_modBase = modBase;
        return modBase;
    }

    std::vector<Process::HandleInfo> Process::findHandles(NtWrap* nt) {
        if (!nt->querySystemInfo) return {};
        std::vector<HandleInfo> results = {};

        PVOID buffer = nullptr;
        ULONG size = 0x10000;
        NTSTATUS status;
        do {
            buffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
            status = nt->querySystemInfo(0x10, buffer, size, &size);
            if (status == 0xC0000004) {
                HeapFree(GetProcessHeap(), 0, buffer);
                buffer = nullptr;
                size *= 2;
            }

        } while (status == 0xC0000004 && size < 0x1000000);

        if (buffer == nullptr || status < 0) return results;

        PSYSTEM_HANDLE_INFORMATION info = (PSYSTEM_HANDLE_INFORMATION)buffer;
        for (ULONG i = 0; i < info->NumberOfHandles; i++) {
            if (info->Handles[i].UniqueProcessId == Process::getTargetPid()) continue;
            if (info->Handles[i].GrantedAccess & (PROCESS_VM_READ | PROCESS_QUERY_INFORMATION)) {
                HandleInfo hi;
                hi.access = info->Handles[i].GrantedAccess;
                hi.handleValue = info->Handles[i].HandleValue;
                hi.pid = info->Handles[i].UniqueProcessId;
                results.push_back(hi);
            }
        }
        HeapFree(GetProcessHeap(), 0, buffer);
        return results;
    }

    HANDLE Process::verifyAndDuplicateHandle(NtWrap* nt, HandleInfo* hi) {
        if (!nt->duplicateHandle) return NULL;
        HANDLE srcHan = OpenProcess(PROCESS_DUP_HANDLE, 0, hi->pid);
        HANDLE hDup;
        NTSTATUS status = nt->duplicateHandle(srcHan, (HANDLE)(uintptr_t)hi->handleValue, GetCurrentProcess(), &hDup, (PROCESS_VM_READ | PROCESS_QUERY_LIMITED_INFORMATION), 0, 0);
        if (status < 0) {//TODO desc
            CloseHandle(srcHan);
            return nullptr;
        }
        CloseHandle(srcHan);

        DWORD hDupPid = GetProcessId(hDup);
        if (hDupPid != getTargetPid()) {
            CloseHandle(hDup);
            return nullptr;
        }
        return hDup;
    }

    HANDLE Process::hijackHandle(NtWrap* ntObj) {
        std::vector<HandleInfo> contestants = findHandles(ntObj);
        std::vector<HandleInfo> prioritorized;
        if (contestants.empty()) return NULL;
        for (const auto& c : contestants) {
            HANDLE srcHan = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, 0, c.pid);
            wchar_t path[MAX_PATH];
            DWORD pathSize = MAX_PATH;
            if (GetProcessImageFileNameW(srcHan, path, pathSize)) {
                if (wcsstr(path, L"explorer.exe") || wcsstr(path, L"svchost.exe") || wcsstr(path, L"steam.exe") || wcsstr(path, L"dmw.exe")) {
                    prioritorized.insert(prioritorized.begin(), c);
                }
                else {
                    prioritorized.push_back(c);
                }
            }
            CloseHandle(srcHan);
        }

        for (auto& p : prioritorized) {
            HANDLE hDup = verifyAndDuplicateHandle(ntObj, &p);
            if (hDup) return hDup;
            CloseHandle(hDup);
        }
        return nullptr;
    }

}