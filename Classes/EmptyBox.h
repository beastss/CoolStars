#ifndef __EMPTYBOX_H__
#define __EMPTYBOX_H__

#include "cocos2d.h"
class EmptyBox : public cocos2d::CCNode
{
public:
    static EmptyBox *create();
    EmptyBox();

    virtual bool init();
    void setNode(cocos2d::CCNode* node);
    void removeNode();
	
	void setMaskVisible(bool isVisible);
private:
    cocos2d::CCNode *m_node;
	cocos2d::CCLayerColor *m_maskNode;
};


#endif 
