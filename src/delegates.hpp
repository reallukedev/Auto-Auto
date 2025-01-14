class MyLevelManagerDelegate : public LevelManagerDelegate {
  private:
	void loadLevelsFinished(cocos2d::CCArray *, char const *) override;

	void loadLevelsFailed(char const *p0) override;

	void loadLevelsFinished(cocos2d::CCArray *, char const *, int) override;

	void loadLevelsFailed(char const *, int) override;
};

class MyLevelDownloadDelegate : public LevelDownloadDelegate {
  private:
	void levelDownloadFinished(GJGameLevel *) override;

	void levelDownloadFailed(int) override;
};
