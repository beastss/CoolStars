#include "PetView.h"
#include "PetEntity.h"
#include "PetManager.h"
#include "UiLayout.h"
using namespace std;
USING_NS_CC;

PetView::PetView(int petId)
	: m_petId(petId)
{
	m_model = PetManager::petMgr()->getPetById(petId);
	assert(m_model);
}

PetView *PetView::create(int petId)
{
	auto view = new PetView(petId);
	view->init();
	view->autorelease();
	return view;
}

bool PetView::init()
{
	m_layout = UiLayout::create("layout/pet_node.xml");
	initLayout();
	
	setContentSize(m_layout->getContentSize());
	addChild(m_layout);
	return true;
}

void PetView::initLayout()
{
	CCArmature *pet = dynamic_cast<CCArmature *>(m_layout->getChildById(3));
	
	string path = m_model->getPetData().petAnimationRes;
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(path.c_str());

	int pos1 = path.rfind("/");
	int pos2 = path.rfind(".");
	string armatureName = path.substr(pos1 + 1, pos2 - pos1 - 1);

	pet->init(armatureName.c_str());
	pet->getAnimation()->play("standby");

}