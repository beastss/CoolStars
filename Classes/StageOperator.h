#ifndef __STAGEOPERATOR_H__
#define __STAGEOPERATOR_H__
#include "cocos2d.h"
#include "StarsController.h"
#include "StarNode.h"
#include <functional>

#define StageOp StageOperator::theOperator()
class ActionRunner;
class StageOperator
{
public:
	static StageOperator *theOperator();
	~StageOperator();
public:
	void eraseStars(std::vector<LogicGrid> grids);
	void petScaleErase(int petId, const LogicGrid &center, int xRadius, int yRadius);
	void petRandomErase(int petId, int num);

	void addSteps(int amount);

	void changeColor(const StarAttr &attr);
	void gameOverRandomReplace();
	void randomReplaceStars(int petId, int starType, int color, int num);

	void addPetEnergy(int petId, int value);
	void removePetDebuff(int who);
	void chageStarType(int type);
	void reOrderStars(std::function<void()> callback = std::function<void()>());
	void loadDesignatedStar(int color, int rounds);

	std::vector<LogicGrid> getRandomColorGrids(int num, bool hasExceptColor = false, int exceptColor = kColorRed);
	std::vector<LogicGrid> getRandomActiveGrids(int num);//·Ç¿ÕÐÇÐÇ
private:
	StageOperator();
private:
	ActionRunner *m_runner;
};
#endif