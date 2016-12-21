#include "InputPad.h"
USING_NS_CC;
using namespace extension;

InputPad::InputPad(std::string bkPath, int touchPriority)
: m_touchPriority(touchPriority)
, m_bkPath(bkPath)
{

}

InputPad *InputPad::create(std::string bkPath, int touchPriority)
{
	InputPad *pad = new InputPad(bkPath, touchPriority);
	pad->init();
	pad->autorelease();
	return pad;
}

bool InputPad::init()
{
	initEditBox();
	setContentSize(m_editBox->getContentSize());
	return true;
}

void InputPad::initEditBox()
{
	CCSize editSize = CCSprite::create(m_bkPath.c_str())->getContentSize();
	m_editBox = CCEditBox::create(editSize, CCScale9Sprite::create(m_bkPath.c_str()));
	m_editBox->setDelegate(this);
	m_editBox->setAnchorPoint(ccp(0, 0));
	m_editBox->setTouchPriority(m_touchPriority);
	addChild(m_editBox);
}

std::string InputPad::getText()
{
	return m_editBox->getText();
}

static int _calcCharCount(const char * text)
{
	int n = 0;
	char ch = 0;
	while ((ch = *text))
	{
		CC_BREAK_IF(!ch);
		if (0x80 != (0xC0 & ch))
		{
			++n;
		}
		++text;
	}
	return n;
}

void InputPad::setLengthLimit(int num)
{
	m_editBox->setMaxLength(num);
}

void InputPad::editBoxEditingDidBegin(CCEditBox *editBox)
{
	CCLOG("start edit");
}

void InputPad::editBoxEditingDidEnd(CCEditBox *editBox)
{
	CCLOG("end edit");
}

void InputPad::editBoxTextChanged(CCEditBox *editBox, const std::string &text)
{
	CCLOG("textchanged");
}

void InputPad::editBoxReturn(CCEditBox *editBox)
{
	CCLOG("editboxreturn");
}