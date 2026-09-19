#pragma once
#include <mutex>
#include "Vec.hpp"

namespace core {
	class dataMagazine
	{
		std::mutex dataMagazineMutex;
		math::gameData m_nextData = {};

	public:
		void saveGameData(math::gameData& dataToSave);
		math::gameData getGameData();

	};
}