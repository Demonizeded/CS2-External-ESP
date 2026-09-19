#pragma once
#include <vector>
#include <mutex>
#include "Vec.hpp"
#include "Process.hpp"
#include "Syscall.hpp"
#include "offsets.hpp"
#include "dataMagazine.hpp"

namespace core {
	class readData
	{
		math::gameData m_readyData = {};
	public:
		readData(const readData&) = default;

		readData() = default;

		bool readMemory(sys::Process* prcObj, sys::NtWrap* ntObj, dataMagazine* dataMag);
	};
}
