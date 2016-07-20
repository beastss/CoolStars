#include "EmptyBox.h"
using namespace std;
USING_NS_CC;

EmptyBox *EmptyBox::create()
{
    EmptyBox *node = new EmptyBox();
    node->init();
    node->autorelease();
    return node;
}

bool EmptyBox::init()
{
	m_maskNode = CCLayerColor::create(ccc4(255, 255, 0, 75));
	m_maskNode->setContentSize(CCSize(50, 50));
	addChild(m_maskNode);
	m_maskNode->setVisible(false);

    return true;
}

EmptyBox::EmptyBox()
:m_node(NULL)
, m_maskNode(NULL)
{
    
}

void EmptyBox::setNode(cocos2d::CCNode* node)
{
    removeNode();
    addChild(node);
	auto size = node->getContentSize();
	auto anchorPt = node->getAnchorPoint();
	setContentSize(size);

	node->setPosition(ccp(size.width * anchorPt.x, size.height * anchorPt.y));
	m_node = node;
}

void EmptyBox::removeNode()
{
    if(m_node)
    {
        setContentSize(CCSize(0, 0));
        m_node->removeFromParent();
        m_node = NULL;
    }
}

void EmptyBox::setMaskVisible(bool isVisible)
{
	//m_maskNode->setContentSize(getContentSize());
	m_maskNode->setVisible(isVisible);
}

