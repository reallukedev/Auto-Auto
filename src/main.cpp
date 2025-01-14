#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/LoadingCircle.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/MusicDownloadManager.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include "functions.hpp"
#include "popup.hpp"
#include "vars.hpp"

std::list<int> levelIds = {};
bool autoModeEnabled = false;

class $modify(MyEndLevelLayer, EndLevelLayer) {
	void customSetup() {
		EndLevelLayer::customSetup();

		if (autoModeEnabled) {
			auto menu = m_mainLayer->getChildByID("button-menu");

			auto myButton = CCMenuItemSpriteExtra::create(
				CCSprite::create("stopAuto.png"_spr), this,
				menu_selector(MyEndLevelLayer::onMyButton));

			myButton->setID("stop-auto-auto-end-button");
			myButton->setPosition({180, -125});
			menu->addChild(myButton);

			handleTouchPriority(this);
		}
	}

	void onMyButton(CCObject *) {
		geode::createQuickPopup(
			"Stop Auto Auto",
			"Are you sure that you would like to stop \"Auto Auto\"?",
			"Continue Running", "Stop \"Auto Auto\"", [this](auto, bool btn2) {
				if (btn2) {
					autoModeEnabled = false;

					auto btn = m_mainLayer->getChildByID("button-menu")
								   ->getChildByID("stop-auto-auto-end-button");
					btn->removeFromParent();
				}
			});
	}
};

class $modify(MyPlayLayer, PlayLayer) {
	void playTheNextLevel() { playNextLevel(); }

	void levelComplete() {
		PlayLayer::levelComplete();

		this->runAction(CCSequence::create(
			CCDelayTime::create(5.0f),
			CCCallFunc::create(
				this, callfunc_selector(MyPlayLayer::playTheNextLevel)),
			nullptr));
	}

	void onQuit() {
		PlayLayer::onQuit();

		autoModeEnabled = false;
	}

	void destroyPlayer(PlayerObject *p0, GameObject *p1) {
		PlayLayer::destroyPlayer(p0, p1);

		if (p0->m_isDead) {
			if (autoModeEnabled) {
				PlayLayer::pauseGame(true);

				MyPlayLayer::playTheNextLevel();
			}
		}
	}
};

// https://github.com/Weebifying/lvlinfo-in-pause-geode/blob/main/src/main.cpp
class $modify(MyPauseLayer, PauseLayer) {
	static void onModify(auto &self) {
		Result<> pauseLayerCustomSetup =
			self.setHookPriority("PauseLayer::customSetup", INT_MIN);
	}

	void customSetup() {
		PauseLayer::customSetup();

		if (autoModeEnabled) {
			auto menu = this->getChildByID("left-button-menu");

			auto myButton = CCMenuItemSpriteExtra::create(
				CCSprite::create("stopAuto.png"_spr), this,
				menu_selector(MyPauseLayer::onMyButton));

			myButton->setID("stop-auto-auto-button");
			myButton->setPosition({menu->getContentSize().width / 2,
								   myButton->getContentSize().height / 2});
			menu->addChild(myButton);

			menu->setLayout(ColumnLayout::create()
								->setGap(4.f)
								->setAutoScale(true)
								->setAxisAlignment(AxisAlignment::Start)
								->setCrossAxisAlignment(AxisAlignment::Center));
			menu->updateLayout();
		}
	}

	void onMyButton(CCObject *) {
		geode::createQuickPopup(
			"Stop Auto Auto",
			"Are you sure that you would like to stop \"Auto Auto\"?",
			"Continue Running", "Stop \"Auto Auto\"", [this](auto, bool btn2) {
				if (btn2) {
					autoModeEnabled = false;

					auto btn = this->getChildByID("left-button-menu")
								   ->getChildByID("stop-auto-auto-button");
					btn->removeFromParent();
				}
			});
	}
};

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::create("startAuto.png"_spr), this,
			menu_selector(MyMenuLayer::onMyButton));

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);

		myButton->setID("my-button");

		menu->updateLayout();

		return true;
	}

	void onMyButton(CCObject *) { MyPopup::create(false, "")->show(); }
};

class $modify(MyMusicDownloadManager, MusicDownloadManager) {
	void onDownloadSongCompleted(cocos2d::extension::CCHttpClient *p0,
								 cocos2d::extension::CCHttpResponse *p1) {
		MusicDownloadManager::onDownloadSongCompleted(p0, p1);

		log::info("Downloaded Song!");

		auto songID = atoi(p1->getHttpRequest()->getTag());

		log::info("Downloaded SongID: {}", songID);

		songsToDownload.remove(songID);

		if (songsToDownload.empty()) {
			log::info("All Songs Downloaded!");

			if (autoModeEnabled) {
				playNextLevel();
			}
		}

		// if (currentLevel) {
		// 	auto layer = PlayLayer::create(currentLevel, false, false);
		// 	auto scene = CCScene::create();
		// 	scene->addChild(layer);

		// 	cocos2d::CCDirector::sharedDirector()->replaceScene(
		// 		CCTransitionFade::create(0.5f, scene));

		// 	layer->stopAllActions();
		// 	layer->startGame();
		// 	layer->resetLevelFromStart();
		// }
	}
};
