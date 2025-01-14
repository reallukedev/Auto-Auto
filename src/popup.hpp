class MyPopup : public geode::Popup<bool, std::string> {
  protected:
	bool setup(bool, std::string) override;

  public:
	static MyPopup *create(bool, std::string);

	void onMyButton(CCObject *);

	void onClose(CCObject *) override;

	void closeWithoutPopup();

	void closeOpenPopup();
};
