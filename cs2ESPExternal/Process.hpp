#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <vector>
#include <iostream>
#include "Syscall.hpp"


namespace sys {
    class Process {
        DWORD m_pid = NULL;
        uintptr_t m_modBase = NULL;
        HANDLE m_procHan = nullptr;

        typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO {
            USHORT UniqueProcessId;
            USHORT CreatorBackTraceIndex;
            UCHAR ObjectTypeIndex;
            UCHAR HandleAttributes;
            USHORT HandleValue;
            PVOID Object;
            ACCESS_MASK GrantedAccess;
        } SYSTEM_HANDLE_TABLE_ENTRY_INFO, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

        typedef struct _SYSTEM_HANDLE_INFORMATION {
            ULONG NumberOfHandles;
            SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
        } SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

        struct HandleInfo {
            ULONG pid;
            USHORT handleValue;
            ACCESS_MASK access;
        };


    public:
        Process(const Process&) = delete;
        Process& operator=(const Process&) = delete;

        Process();
        ~Process();

        template<typename T>
        bool rpm(T* buffer, uintptr_t addr, NtWrap* ntObj, SIZE_T size = sizeof(T)) const {
            if (!ntObj->readMemory || !m_procHan) return false;
            SIZE_T read = 0;
            NTSTATUS status = ntObj->readMemory(m_procHan, (PVOID)addr, buffer, size, &read);
            return (status >= 0);
        }

        DWORD findProcessID();
        uintptr_t findBaseModule();

        std::vector<HandleInfo> findHandles(NtWrap* nt);
        HANDLE verifyAndDuplicateHandle(NtWrap* nt, HandleInfo* hi);
        HANDLE hijackHandle(NtWrap* ntObj);

        DWORD getTargetPid() const {
            return m_pid;
        }

        HANDLE getProcHandle() const {
            return m_procHan;
        }

        uintptr_t getModBase() const {
            return m_modBase;
        }

        void setProcHandle(HANDLE newHan) {
            m_procHan = newHan;
        }
    };
}