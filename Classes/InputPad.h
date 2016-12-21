#ifndef _INPUTPAD_H_
#define _INPUTPAD_H_

#include "cocos2d.h"
#include "GUI/CCEditBox/CCEditBox.h"

class InputPad
	: public cocos2d::CCNode
	, public cocos2d::extension::CCEditBoxDelegate
{
public:
	static InputPad *create(std::string bkPath, int touchPriority);
	std::string getText();
	void setLengthLimit(int num);
private:
	InputPad(std::string bkPath, int touchPriority);
	virtual bool init();
	void initEditBox();
private:
	virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox);
	virtual void editBoxTextChanged(cocos2d::extension::CCEditBox *editBox, const std::string &text);
	virtual void editBoxReturn(cocos2d::extension::CCEditBox *editBox);
private:
	cocos2d::extension::CCEditBox *m_editBox;
	int m_touchPriority;
	std::string m_bkPath;
};

#endif