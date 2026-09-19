#include "dataMagazine.hpp"

namespace core {
	void dataMagazine::saveGameData(math::gameData& dataToSave) {
		std::lock_guard<std::mutex> lock(dataMagazineMutex);
		m_nextData = dataToSave;
	}

	math::gameData dataMagazine::getGameData() {
		std::lock_guard<std::mutex> lock(dataMagazineMutex);
		return m_nextData;
	}
}