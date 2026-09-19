#include "Syscall.hpp"

namespace sys {
	bool NtWrap::resolveNtdll() {
		HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");

		if (!ntdll) return false;
		readMemory = (pNtReadVirtualMemory)GetProcAddress(ntdll, "NtReadVirtualMemory");
		duplicateHandle = (pNtDuplicateObject)GetProcAddress(ntdll, "NtDuplicateObject");
		querySystemInfo = (pNtQuerySystemInformation)GetProcAddress(ntdll, "NtQuerySystemInformation");

		return readMemory && duplicateHandle && querySystemInfo;
	}

}