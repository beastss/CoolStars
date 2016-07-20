#ifndef __PETSKILL_H__
#define __PETSKILL_H__

enum SkillType
{
	kEraseOnRow = 1,
	kEraseOnColumn,
	kAddSteps,
	kChangeColor,

};
class PetSkill
{
public:
	virtual ~PetSkill(){}
	static PetSkill *petSkillFactory(int skillId);
	void doMagic(){}
protected:
	PetSkill(int id){}
	PetSkill(){}
private:
};

class PetSkillEraseOnRow : public PetSkill
{

};

class PetSkillEraseOnColumn : public PetSkill
{

};

class PetSkillAddSteps : public PetSkill
{
public:
	PetSkillAddSteps(){}
	~PetSkillAddSteps(){}
private:

};

class PetSkillChangeColor : public PetSkill
{
public:
	PetSkillChangeColor(){}
	~PetSkillChangeColor(){}
private:

};





/*

class PetSkill : PetSkill
{
public:
PetSkill(){}
~PetSkill(){}
private:

};

*/

#endif