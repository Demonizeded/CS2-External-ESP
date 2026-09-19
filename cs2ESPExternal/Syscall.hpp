#pragma once
#include <Windows.h>

namespace sys {
	class NtWrap{
        typedef NTSTATUS(NTAPI* pNtReadVirtualMemory)(
            HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer,
            SIZE_T NumberOfBytesToRead, PSIZE_T NumberOfBytesRead
            );

        typedef NTSTATUS(NTAPI* pNtDuplicateObject)(
            HANDLE SourceProcessHandle, HANDLE SourceHandle, HANDLE TargetProcessHandle,
            PHANDLE TargetHandle, ACCESS_MASK DesiredAccess, ULONG Attributes, ULONG Options
            );

        typedef NTSTATUS(NTAPI* pNtQuerySystemInformation)(
            ULONG SystemInformationClass, PVOID SystemInformation,
            ULONG SystemInformationLength, PULONG ReturnLength
            );
	public:
        pNtReadVirtualMemory readMemory = nullptr;
        pNtDuplicateObject duplicateHandle = nullptr;
        pNtQuerySystemInformation querySystemInfo = nullptr;

        bool resolveNtdll();
	};
}