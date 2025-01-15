#include "delegates.hpp"
#include "Geode/utils/string.hpp"
#include "functions.hpp"
#include "vars.hpp"
#include <Geode/modify/MusicDownloadManager.hpp>

using namespace geode::prelude;

void MyLevelManagerDelegate::loadLevelsFinished(cocos2d::CCArray *p0,
												char const *p1) {
	GameLevelManager::get()->m_levelManagerDelegate = nullptr;
	delete this;

	log::info("Running LevelManager Delegate!");

	for (int i = 0; i < p0->count(); i++) {
		auto obj = p0->objectAtIndex(i);
		auto level = reinterpret_cast<GJGameLevel *>(obj);
		levelIds.push_back(level->m_levelID.value());
	}

	if (levelIds.empty()) {
		geode::createQuickPopup("No Levels Found",
								"Could not find any uncompleted auto "
								"levels. Would you like to try again?",
								"Try Again", "Stop \"Auto Auto\"",
								[this](auto, bool btn2) {
									if (btn2) {
										autoModeEnabled = false;
									} else {
										getNextList();
									}
								});
	} else {
		if (autoModeEnabled) {
			playNextLevel();
		}
	}

	log::info("Fetched Levels!");
}

void MyLevelManagerDelegate::loadLevelsFailed(char const *p0) {
	GameLevelManager::get()->m_levelManagerDelegate = nullptr;
	delete this;

	log::info("Level load failed :c");
}

void MyLevelManagerDelegate::loadLevelsFinished(cocos2d::CCArray *p0,
												char const *p1, int p2) {
	GameLevelManager::get()->m_levelManagerDelegate = nullptr;
	delete this;

	log::info("Finished! (but the weird one :c)");
}

void MyLevelManagerDelegate::loadLevelsFailed(char const *p0, int p1) {
	GameLevelManager::get()->m_levelManagerDelegate = nullptr;
	delete this;

	log::info("Level load failed :c");
}

void MyLevelDownloadDelegate::levelDownloadFinished(GJGameLevel *level) {
	GameLevelManager::get()->m_levelDownloadDelegate = nullptr;
	delete this;

	if (level) {
		currentAutoLevel = typeinfo_cast<GJGameLevel *>(level);

		auto songIDs = level->m_songIDs;
		auto sfxIDs = level->m_sfxIDs;

		auto songIDSplit = string::split(songIDs, ",");
		auto sfxIDSplit = string::split(sfxIDs, ",");

		for (auto &songID : songIDSplit) {
			auto songIDInt = std::stoi(songID);

			if (!MusicDownloadManager::sharedState()->isSongDownloaded(
					songIDInt)) {
				songsToDownload.push_back(songIDInt);
			}

			log::info("Found SongID: {}", songID);
		}

		for (auto &sfxID : sfxIDSplit) {
			auto sfxIDInt = std::stoi(sfxID);

			if (!MusicDownloadManager::sharedState()->isSFXDownloaded(
					sfxIDInt)) {
				sfxToDownload.push_back(sfxIDInt);

				// MusicDownloadManager::sharedState()->downloadSFX(sfxIDInt);
			}

			log::info("Found SFX: {}", sfxID);
		}

		downloadNextSongSfx();
	} else {
		playNextLevel();
	}
}

void MyLevelDownloadDelegate::levelDownloadFailed(int p0) {
	GameLevelManager::get()->m_levelDownloadDelegate = nullptr;
	delete this;
}
