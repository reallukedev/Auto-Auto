#include "delegates.hpp"
#include "popup.hpp"
#include "vars.hpp"

using namespace geode::prelude;

void playLevel(int levelId) {
	log::info("Downloading level {}", levelId);

	MyPopup::create(true, "Downloading level " + std::to_string(levelId))
		->show();

	log::info("Popup Created");

	auto glm = GameLevelManager::get();

	glm->m_levelDownloadDelegate = new MyLevelDownloadDelegate();

	log::info("Delegate Assigned");

	glm->downloadLevel(levelId, false);

	log::info("Level Downloaded");
};

void getNextList() {
	// log::info("Getting next list");

	// MyPopup::create(true, "Fetching Uncompleted Auto Levels")->show();

	// log::info("Popup Created");

	// auto glm = GameLevelManager::get();

	// glm->m_levelManagerDelegate = new MyLevelManagerDelegate();

	// log::info("Delegate Created");

	// // SearchType searchType , gd :: string searchQuery , gd ::
	// // string difficulty, gd ::string length, int page, bool star,
	// // 	bool uncompleted, bool featured, int songID, bool original,
	// // 	bool twoPlayer, bool customSong, bool songFilter, bool noStar,
	// // 	bool coins, bool epic, bool legendary, bool mythic, bool
	// // // onlyCompleted, 	int demonFilter, int folder,
	// glm->getOnlineLevels(GJSearchObject::create(
	// 	SearchType::MostLiked, "", "-3", "-", 0, true, true, false, 1, false,
	// 	false, false, false, false, false, false, false, false, false, 0, 0,
	// 	0));

	// log::info("Levels Fetched");

	levelIds = {110236560};
};

void playNextLevel() {
	if (!autoModeEnabled) {
		return;
	}

	if (levelIds.empty()) {
		getNextList();
		return;
	}

	auto levelId = levelIds.front();

	playLevel(levelId);

	levelIds.pop_front();
};
