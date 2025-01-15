#include "delegates.hpp"
#include "popup.hpp"
#include "vars.hpp"

using namespace geode::prelude;

void playLevel(int levelId) {
	if (autoModeEnabled) {
		log::info("Downloading level {}", levelId);

		MyPopup::create(true, "Downloading level " + std::to_string(levelId))
			->show();

		log::info("Popup Created");

		auto glm = GameLevelManager::get();

		glm->m_levelDownloadDelegate = new MyLevelDownloadDelegate();

		log::info("Delegate Assigned");

		glm->downloadLevel(levelId, false);

		log::info("Level Downloaded");
	}
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

	levelIds = {113873694};
};

void playNextLevel() {
	if (autoModeEnabled) {
		if (levelIds.empty()) {
			getNextList();
			return;
		}

		auto levelId = levelIds.front();

		playLevel(levelId);

		levelIds.pop_front();
	}
};

void playCurrentLevel() {
	log::info("Playing {}", currentAutoLevel);

	if (autoModeEnabled) {
		if (currentAutoLevel) {
			auto layer = PlayLayer::create(currentAutoLevel, false, false);
			auto scene = CCScene::create();
			scene->addChild(layer);

			cocos2d::CCDirector::sharedDirector()->replaceScene(
				CCTransitionFade::create(0.5f, scene));

			layer->stopAllActions();
			layer->startGame();
			layer->resetLevelFromStart();
		} else {
			playNextLevel();
		}
	}
}

void downloadNextSongSfx() {
	if (!songsToDownload.empty()) {
		MusicDownloadManager::sharedState()->downloadSong(
			songsToDownload.front());

		MyPopup::create(true, "Downloading Song " +
								  std::to_string(songsToDownload.front()))
			->show();
	} else if (!sfxToDownload.empty()) {
		MusicDownloadManager::sharedState()->downloadSFX(sfxToDownload.front());

		MyPopup::create(true, "Downloading SFX " +
								  std::to_string(songsToDownload.front()))
			->show();
	} else {
		log::info("All Songs and SFX Downloaded!");

		playCurrentLevel();
	}
}
