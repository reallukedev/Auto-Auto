#include "popup.hpp"
#include "functions.hpp"
#include "vars.hpp"

using namespace geode::prelude;

bool MyPopup::setup(bool loading, std::string labelText) {
	this->setTitle("Auto Auto");

	auto m_listMenu = CCMenu::create();

	m_listMenu->setPosition(0, 0);

	m_mainLayer->addChildAtPosition(m_listMenu, Anchor::Center);

	if (loading) {
		auto loadingCircle = CCSprite::create();
		loadingCircle->initWithFile("loadingCircle.png");

		loadingCircle->setBlendFunc({GL_SRC_ALPHA, GL_ONE});

		auto *rotateAction = cocos2d::CCRepeatForever::create(
			cocos2d::CCRotateBy::create(1.f, 360.f));
		loadingCircle->runAction(rotateAction);

		m_listMenu->addChild(loadingCircle);

		auto label =
			CCLabelBMFont::create(labelText.c_str(), "bigFont.fnt", 240.f,
								  CCTextAlignment::kCCTextAlignmentCenter);

		label->setScale(.5f);
		label->setLayoutOptions(
			AxisLayoutOptions::create()->setAutoScale(false));

		m_listMenu->addChild(label);

		m_listMenu->setLayout(
			ColumnLayout::create()
				->setGap(4.f)
				->setAutoScale(true)
				->setAxisAlignment(AxisAlignment::Center)
				->setCrossAxisAlignment(AxisAlignment::Center));

		m_listMenu->updateLayout();
	} else {
		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png"), this,
			menu_selector(MyPopup::onMyButton));

		m_listMenu->addChild(myButton);

		myButton->setID("auto-auto-start-button");

		auto label = CCLabelBMFont::create(
			"Automatically\nPlay Uncompleted\nAuto Levels", "bigFont.fnt",
			240.f, CCTextAlignment::kCCTextAlignmentCenter);

		label->setScale(.5f);
		label->setLayoutOptions(
			AxisLayoutOptions::create()->setAutoScale(false));

		m_listMenu->addChild(label);

		m_listMenu->setLayout(
			ColumnLayout::create()
				->setGap(4.f)
				->setAutoScale(true)
				->setAxisAlignment(AxisAlignment::Center)
				->setCrossAxisAlignment(AxisAlignment::Center));

		m_listMenu->updateLayout();
	}

	handleTouchPriority(this);

	return true;
}

MyPopup *MyPopup::create(bool loading, std::string label) {
	auto ret = new MyPopup();
	if (ret->initAnchored(240.f, 240.f, loading, label)) {
		ret->autorelease();
		return ret;
	}

	delete ret;
	return nullptr;
}

void MyPopup::onMyButton(CCObject *) {
	closeWithoutPopup();

	autoModeEnabled = true;

	playNextLevel();
}

void MyPopup::onClose(CCObject *) {
	if (autoModeEnabled) {
		geode::createQuickPopup(
			"Stop Auto Auto",
			"Are you sure that you would like to stop \"Auto Auto\"?",
			"Continue Running", "Stop \"Auto Auto\"", [this](auto, bool btn2) {
				if (btn2) {
					autoModeEnabled = false;

					closeWithoutPopup();
				}
			});
	} else {
		closeWithoutPopup();
	}
}

void MyPopup::closeWithoutPopup() { Popup::onClose(nullptr); }
